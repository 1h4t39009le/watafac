#include <watafac/parse/Parser.hpp>
#include <utility>
#include <charconv>
namespace wfac::parse {
    Parser::Parser(std::shared_ptr<wfac::Source> source)
        : lexer_(source),
          source_{std::move(source)}
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
    ast::Expr *Parser::parse_assignment(){
        ast::Expr *left = parse_equality(), *right = nullptr;
        if(!left) return nullptr;
        if(match_kind(TKind::Equals)){
            right = parse_expr();
            if(!right) return nullptr;
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
            if(!right) return nullptr;

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
            if(!right) return nullptr;

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
            if(!right) return nullptr;

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
            if(!right) return nullptr;

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
                return nullptr;
            }
            return expr;
        }
        return nullptr;
    }
}
