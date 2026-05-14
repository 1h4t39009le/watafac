#ifndef WATAFAC_PARSE_PARSER_HPP
#define WATAFAC_PARSE_PARSER_HPP
#include <watafac/env/Session.hpp>
#include <watafac/lex/Lexer.hpp>
#include <watafac/ast/Ast.hpp>
#include <memory_resource>
#include <optional>
#include <functional>
namespace wfac::parse {
    namespace lex = wfac::lex;
    namespace ast = wfac::ast;
    namespace env = wfac::env;
    namespace src = wfac::src;
    using TKind = lex::Token::Kind;
    class Parser {
    public:
        explicit Parser(env::Session &session, src::SourceId source_id);
        ast::Expr *parse_expr(); // REMOVE FROM HERE
        ast::Stmt *parse_stmt(); //REMOVE FROM HERE PLS :(
        std::optional<ast::ProgramGroup> parse();
        ~Parser();
    private:
        struct ParsePoint{
            lex::Lexer::Snapshot lex_snap;
            lex::Token rdtoken;
        };
        template<typename T, typename... Args>
        T *alloc(Args&&... args){
            void *ptr = arena_.allocate(sizeof(T), alignof(T));
            T *node = new (ptr) T(std::forward<Args>(args)...);
            if constexpr (!std::is_trivially_destructible_v<T>){
                cleanup_.push_back([node]{std::destroy_at(node);});
            }
            return node;
        }
       
        /*
        ast::Expr *parse_prim();
        ast::Expr *parse_addition();
        
        
        bool parse_program(ast::ProgramGroup &program);
        */
        lex::Token next_token();
        bool match_kind(TKind kind, bool eat = true);
        template<typename T>
        T *error_here(std::string msg){
             session_.error(source_id_, rdtoken_.get_loc(), std::move(msg));
             return nullptr;
        }
        template<typename T>
        T *error_here_expected(TKind exp){
            return error_here<T>(std::string("expected '") + lex::Token::kind_name(exp) + "' got '" + lex::Token::kind_name(rdtoken_.get_kind()) + "'");
        }

        ParsePoint save_point();
        void restore_point(ParsePoint point);
        
        ast::Declarator *parse_declarator();
        ast::PrimitiveTypeSpec *parse_type_spec();
        ast::VarDecl *try_parse_var_decl();
        
        std::optional<ast::BlockItem> parse_block_item();
        ast::Stmt *parse_expr_stmt();
        ast::Stmt *parse_compound_stmt();
        ast::Stmt *parse_if_stmt();
        ast::Stmt *parse_while_stmt();
        ast::Stmt *parse_for_stmt();
        ast::Stmt *parse_return_stmt();
        
        ast::Expr *parse_assignment();
        ast::Expr *parse_equality();
        ast::Expr *parse_comparison();
        ast::Expr *parse_term();
        ast::Expr *parse_factor();
        ast::Expr *parse_unary();
        ast::Expr *parse_primary();
        
        

        env::Session &session_;
        src::SourceId source_id_;
        lex::Lexer lexer_;;
        lex::Token rdtoken_;
        std::pmr::monotonic_buffer_resource arena_;
        std::vector<std::function<void()>> cleanup_;
    };
}
#endif
