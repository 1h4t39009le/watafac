#include <watafac/parse/Parser.hpp>
#include <utility>
#include <charconv>
using namespace std::string_literals;
namespace wfac::parse {
    Parser::Parser(env::Session &session, src::SourceId source_id)
        : session_(session), source_id_(source_id),
          lexer_(session, source_id)
    {
        next_token();
    }
    Parser::~Parser(){
        for(auto it = cleanup_.rbegin(); it != cleanup_.rend(); ++it){
            if(*it){
                (*it)();
            }
        }
    }

    lex::Token Parser::next_token(){
        return std::exchange(rdtoken_, lexer_.next_token());
    }
    bool Parser::match_kind(TKind kind, bool eat){
        if(rdtoken_.get_kind() == kind){
            if(eat) next_token();
            return true;
        }
        return false;
    }
    
    ast::Expr *Parser::parse_expr(){
        return parse_assignment();
    }

    ast::Stmt *Parser::parse_stmt(){
        if(match_kind(TKind::LBrace)){
            return parse_compound_stmt();
        }else if(match_kind(TKind::KwIf)){
            return parse_if_stmt();
        }else if(match_kind(TKind::KwWhile)){
            return parse_while_stmt();
        }else if(match_kind(TKind::KwFor)){
            return parse_for_stmt();
        }else if(match_kind(TKind::KwReturn)){
            return parse_return_stmt();
        }else {
            return parse_expr_stmt();
        }
    }

    ast::Stmt *Parser::parse_block_item(){
        return parse_stmt();
    }
    ast::Stmt *Parser::parse_expr_stmt(){
        ast::Expr *expr = parse_expr();
        if(!match_kind(TKind::Semicolon)){
            return error_here_expected<ast::Stmt>(TKind::Semicolon);
        }
        return alloc<ast::ExprStmt>(expr);
    }
    ast::Stmt *Parser::parse_compound_stmt(){
        auto *compound_stmt = alloc<ast::CompoundStmt>();
        while(!match_kind(TKind::RBrace)){
            if(match_kind(TKind::Eof)){
                return error_here_expected<ast::Stmt>(TKind::RBrace);
            }
            if(ast::Stmt *stmt = parse_stmt()){
                compound_stmt->add_item(stmt);
            }
        }
        return compound_stmt;
    }
    ast::Stmt *Parser::parse_if_stmt(){
        if(!match_kind(TKind::LParen)){
            return error_here_expected<ast::Stmt>(TKind::LParen);
        }
        ast::Expr *cond = parse_expr();
        if(!cond) return nullptr;
        
        if(!match_kind(TKind::RParen)){
            return error_here_expected<ast::Stmt>(TKind::RParen);
        }
        ast::Stmt *then_stmt = parse_stmt();
        if(!then_stmt) return nullptr;

        ast::Stmt *else_stmt = nullptr;
        if(match_kind(TKind::KwElse)){
            else_stmt = parse_stmt();
            if(!else_stmt) return nullptr;
        }

        return alloc<ast::IfStmt>(cond, then_stmt, else_stmt);
    }
    ast::Stmt *Parser::parse_while_stmt(){
        if(!match_kind(TKind::LParen)){
            return error_here_expected<ast::Stmt>(TKind::LParen);
        }
        ast::Expr *cond = parse_expr();
        if(!cond) return nullptr;
        
        if(!match_kind(TKind::RParen)){
            return error_here_expected<ast::Stmt>(TKind::RParen);
        }
        ast::Stmt *body = parse_stmt();
        if(!body) return nullptr;

        return alloc<ast::WhileStmt>(cond, body);
    }
    ast::Stmt *Parser::parse_for_stmt(){
        if(!match_kind(TKind::LParen)){
            return error_here_expected<ast::Stmt>(TKind::LParen);
        }
       ast::Expr *exprs[3] = {};
       for (int i = 0; i < 3; i++) {
           exprs[i] = parse_expr();
           if (!exprs[i]) return nullptr;
           if (i < 2 && !match_kind(TKind::Semicolon)) {
               return error_here_expected<ast::Stmt>(TKind::Semicolon);
           }
       }
        if(!match_kind(TKind::RParen)){
            return error_here_expected<ast::Stmt>(TKind::RParen);
        }
        ast::Stmt *body = parse_stmt();
        if(!body) return nullptr;

        return alloc<ast::ForStmt>(exprs[0], exprs[1], exprs[2], body);
    }
    ast::Stmt *Parser::parse_return_stmt(){
        ast::Expr *expr = nullptr;
        if(!match_kind(TKind::Semicolon)){
            expr = parse_expr();
            if(!expr) return nullptr;
            if(!match_kind(TKind::Semicolon)){
                return error_here_expected<ast::Stmt>(TKind::Semicolon);
            }
        }
        return alloc<ast::ReturnStmt>(expr);
    }
    
    ast::Expr *Parser::parse_assignment(){
        ast::Expr *left = parse_equality(), *right = nullptr;
        if(!left) return nullptr;
        if(match_kind(TKind::Equals)){
            right = parse_expr();
            if(!right){
                return error_here<ast::Expr>("expected expression after '='");
            }
            auto *assign = alloc<ast::AssignExpr>(left, right);
            return assign;
        }
        return left;
    }
    ast::Expr *Parser::parse_equality(){
        ast::Expr *left = parse_comparison();
        if(!left) return nullptr;
        for(;;){
            ast::BinopExpr::Kind kind;
            if(match_kind(TKind::EqualsEq)){
                kind = ast::BinopExpr::Kind::Equals;
            } else if(match_kind(TKind::NotEquals)){
                kind = ast::BinopExpr::Kind::NotEquals;
            } else break;
        
            ast::Expr *right = parse_comparison();
            if(!right){
                return error_here<ast::Expr>("expected expression after binary operator");
            }

            left = alloc<ast::BinopExpr>(left, kind, right);
        }
        return left;
    }
    ast::Expr *Parser::parse_comparison(){
        ast::Expr *left = parse_term();
        if(!left) return nullptr;
        for(;;){
            ast::BinopExpr::Kind kind;
            if(match_kind(TKind::Less)){
                kind = ast::BinopExpr::Kind::Less;
            } else if(match_kind(TKind::Greater)){
                kind = ast::BinopExpr::Kind::Greater;
            } else if(match_kind(TKind::LessOrEq)){
                kind = ast::BinopExpr::Kind::LessOrEq;
            } else if(match_kind(TKind::GreaterOrEq)){
                kind = ast::BinopExpr::Kind::GreaterOrEq;
            } else break;
        
            ast::Expr *right = parse_term();
            if(!right){
                return error_here<ast::Expr>("expected expression after binary operator");
            }

            left = alloc<ast::BinopExpr>(left, kind, right);
        }
        return left;
    }
    ast::Expr *Parser::parse_term(){
        ast::Expr *left = parse_factor();
        if(!left) return nullptr;
        for(;;){
            ast::BinopExpr::Kind kind;
            if(match_kind(TKind::Plus)){
                kind = ast::BinopExpr::Kind::Add;
            } else if(match_kind(TKind::Minus)){
                kind = ast::BinopExpr::Kind::Sub;
            } else break;
        
            ast::Expr *right = parse_factor();
            if(!right){
                return error_here<ast::Expr>("expected expression after binary operator");
            }

            left = alloc<ast::BinopExpr>(left, kind, right);
        }
        return left;
    }
    ast::Expr *Parser::parse_factor(){
        ast::Expr *left = parse_unary();
        if(!left) return nullptr;
        for(;;){
            ast::BinopExpr::Kind kind;
            if(match_kind(TKind::Star)){
                kind = ast::BinopExpr::Kind::Mul;
            } else if(match_kind(TKind::Slash)){
                kind = ast::BinopExpr::Kind::Div;
            } else if(match_kind(TKind::Percent)){
                kind = ast::BinopExpr::Kind::Mod;
            } else break;
        
            ast::Expr *right = parse_unary();
            if(!right){
                return error_here<ast::Expr>("expected expression after binary operator");
            }

            left = alloc<ast::BinopExpr>(left, kind, right);
        }
        return left;
    }
    ast::Expr *Parser::parse_unary(){
        ast::UnaryExpr::Kind kind;
        if(match_kind(TKind::Bang)){
            kind = ast::UnaryExpr::Kind::Not;
        } else if(match_kind(TKind::Minus)){
            kind = ast::UnaryExpr::Kind::Negative;
        } else if(match_kind(TKind::Ampersand)){
            kind = ast::UnaryExpr::Kind::AddrOf;
        } else if(match_kind(TKind::Star)){
            kind = ast::UnaryExpr::Kind::Deref;
        } else return parse_primary();
        ast::Expr *inner = parse_unary();
        if(!inner){
            return error_here<ast::Expr>("expected expression after unary operator");
        }
        return alloc<ast::UnaryExpr>(inner, kind);
    }
    ast::Expr *Parser::parse_primary(){
        lex::Token tk = rdtoken_;
        if(match_kind(TKind::Int)) {
            int value;
            std::string lexeme = lexer_.get_lexeme(tk.get_loc());
            std::from_chars(lexeme.data(), lexeme.data()+lexeme.size(), value);
            return alloc<ast::IntExpr>(value);
        }
        if(match_kind(TKind::Ident)) {
            std::string value = lexer_.get_lexeme(tk.get_loc());
            return alloc<ast::VarExpr>(std::move(value));
        }
        if(match_kind(TKind::LParen)){
            ast::Expr *expr = parse_expr();
            if(!match_kind(TKind::RParen)){
                return error_here_expected<ast::Expr>(TKind::RParen);
            }
            return expr;
        }
        return error_here<ast::Expr>("expected expression, got '"s + lex::Token::kind_name(rdtoken_.get_kind()) + "'");
    }
}
