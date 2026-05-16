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

    Parser::ParsePoint Parser::save_point(){
        return {lexer_.save(),rdtoken_};
    }
    void Parser::restore_point(ParsePoint point){
        lexer_.restore(std::move(point.lex_snap));
        rdtoken_ = std::move(point.rdtoken);
    }
    void Parser::skip_to_recovery() {
        while (!match_kind(TKind::Semicolon) && 
               !match_kind(TKind::RBrace) && 
               !match_kind(TKind::Eof)) {
            next_token();
        }
        if (match_kind(TKind::Semicolon)) {
            next_token();
        }
    }
    std::optional<ast::Program> Parser::parse() {
        auto *program = alloc<ast::Program>();
        while (!match_kind(TKind::Eof)) {
            if (auto item = parse_block_item()) {
                program->add_item(std::move(*item));
            }else skip_to_recovery();
        }
        return *program;
    }
    

    ast::Type *Parser::parse_type() {
        if (match_kind(TKind::KwInt))  return alloc<ast::TermType>(ast::TermType::Int);
        if (match_kind(TKind::KwChar)) return alloc<ast::TermType>(ast::TermType::Char);
        return nullptr;
    }

    ast::Type *Parser::parse_type_with_star() {
        ast::Type *t = parse_type();
        if (!t) return nullptr;
        while (match_kind(TKind::Star)) {
            t = alloc<ast::PointerType>(t);
        }
        return t;
    }
    ast::VarDecl *Parser::try_parse_var_decl() {
        auto pt = save_point();
        ast::Type *type = parse_type_with_star();
        auto recover_path = [&](ast::VarDecl *err) -> ast::VarDecl* {
            restore_point(std::move(pt)); 
            return err;
        };
    
        if (!type) {
            return recover_path(nullptr); // ambigious - recover
        }
    
        std::string name = lexer_.get_lexeme(rdtoken_.get_loc());
        if (!match_kind(TKind::Ident)) {
            return error_here_expected<ast::VarDecl>(TKind::Ident); // if type parsed - 100%
        }
    
        ast::Expr *init = nullptr;
        if (match_kind(TKind::Equals)) {
            init = parse_expr();
            if (!init) {
                return error_here<ast::VarDecl>("expected <expr> after '='");
            }
        }
    
        if (!match_kind(TKind::Semicolon)) {
            return error_here_expected<ast::VarDecl>(TKind::Semicolon);
        }
    
        return alloc<ast::VarDecl>(type, std::move(name), init);
    }
    ast::ExternDecl *Parser::parse_extern_decl() {
        if (!match_kind(TKind::KwExtrn)) return nullptr;
        std::string name = lexer_.get_lexeme(rdtoken_.get_loc());
        if (!match_kind(TKind::Ident)) return error_here_expected<ast::ExternDecl>(TKind::Ident);
        if (!match_kind(TKind::Semicolon)) return error_here_expected<ast::ExternDecl>(TKind::Semicolon);
        return alloc<ast::ExternDecl>(std::move(name));
    }
    ast::Expr *Parser::parse_expr(){
        return parse_assignment();
    }

    ast::Stmt *Parser::parse_stmt() {
        if (match_kind(TKind::LBrace))   return parse_compound_stmt();
        if (match_kind(TKind::KwIf))     return parse_if_stmt();
        if (match_kind(TKind::KwWhile))  return parse_while_stmt();
        if (match_kind(TKind::KwFor))    return parse_for_stmt();
        if (match_kind(TKind::KwReturn)) return parse_return_stmt();
        if (match_kind(TKind::Ident, false)){
            auto pt = save_point();
            auto *call = parse_extern_call_stmt();
            if (call) return call;
            restore_point(pt);
        }
            return parse_extern_call_stmt();
        return parse_expr_stmt();
    }

    ast::Stmt *Parser::parse_extern_call_stmt() {
        std::string name = lexer_.get_lexeme(rdtoken_.get_loc());
        match_kind(TKind::Ident);
    
        match_kind(TKind::LParen);
        std::vector<ast::Expr*> args;
        if (!match_kind(TKind::RParen)) {
            do {
                args.push_back(parse_expr());
            } while (match_kind(TKind::Comma));
            match_kind(TKind::RParen);
        }
        if (!match_kind(TKind::Semicolon)) return error_here_expected<ast::Stmt>(TKind::Semicolon);
        return alloc<ast::ExternCallStmt>(std::move(name), std::move(args));
    }

    std::optional<ast::BlockItem> Parser::parse_block_item(){
        if (auto *extern_decl = parse_extern_decl()) {
            return extern_decl;
        }
        if (auto *var_decl = try_parse_var_decl()) {
            return var_decl;
        }
        if (auto *stmt = parse_stmt()) {
            return stmt;
        }
        return std::nullopt;
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
            if(auto item = parse_block_item()){
                compound_stmt->add_item(std::move(*item));
            }else skip_to_recovery();
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
    ast::Expr *Parser::parse_primary() {
        lex::Token tk = rdtoken_;
    
        if (match_kind(TKind::IntLit)) {
            int value;
            std::string lexeme = lexer_.get_lexeme(tk.get_loc());
            std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), value);
            return alloc<ast::IntExpr>(value);
        }
        if (match_kind(TKind::CharLit)) {
            std::string lexeme = lexer_.get_lexeme(tk.get_loc());
            std::string raw = lexeme.substr(1, lexeme.size() - 2);
            std::string unesc = unescape(raw);
            char c = unesc.empty() ? '\0' : unesc[0];
            return alloc<ast::CharExpr>(c);
        }
        if (match_kind(TKind::StringLit)) {
            std::string lexeme = lexer_.get_lexeme(tk.get_loc());
            std::string raw = lexeme.substr(1, lexeme.size() - 2);
            std::string value = unescape(raw);
            size_t id = add_string(value);
            return alloc<ast::StringExpr>(id);
        }
        if (match_kind(TKind::Ident)) {
            std::string value = lexer_.get_lexeme(tk.get_loc());
            return alloc<ast::VarExpr>(std::move(value));
        }
        if (match_kind(TKind::LParen)) {
            ast::Expr *expr = parse_expr();
            if (!match_kind(TKind::RParen))
                return error_here_expected<ast::Expr>(TKind::RParen);
            return expr;
        }

        return error_here<ast::Expr>("expected expression, got '"s + lex::Token::kind_name(rdtoken_.get_kind()) + "'");
    }
    std::string Parser::unescape(const std::string &s) const {
        std::string result;
        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] == '\\' && i + 1 < s.size()) {
                switch (s[++i]) {
                case 'n':  result += '\n'; break;
                case 't':  result += '\t'; break;
                case '\\': result += '\\'; break;
                case '\'': result += '\''; break;
                case '"':  result += '"'; break;
                default:   result += s[i]; break;
                }
            } else {
                result += s[i];
            }
        }
        return result;
    }

    size_t Parser::add_string(const std::string &s) {
        string_pool_.push_back(s);
        return string_pool_.size() - 1;
    }
}
