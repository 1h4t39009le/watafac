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
        
        std::optional<ast::Program> parse();
        ~Parser();

    private:
        struct ParsePoint {
            lex::Lexer::Snapshot lex_snap;
            lex::Token rdtoken;
        };

        template<typename T, typename... Args>
        T *alloc(Args&&... args) {
            void *ptr = arena_.allocate(sizeof(T), alignof(T));
            T *node = new (ptr) T(std::forward<Args>(args)...);
            if constexpr (!std::is_trivially_destructible_v<T>) {
                cleanup_.push_back([node]{ std::destroy_at(node); });
            }
            return node;
        }

        lex::Token next_token();
        bool match_kind(TKind kind, bool eat = true);

        template<typename T>
        T *error_here(std::string msg) {
            session_.error(source_id_, rdtoken_.get_loc(), std::move(msg));
            return nullptr;
        }
        template<typename T>
        T *error_here_expected(TKind exp) {
            return error_here<T>(std::string("expected '") + lex::Token::kind_name(exp) + "' got '" + lex::Token::kind_name(rdtoken_.get_kind()) + "'");
        }

        ParsePoint save_point();
        void restore_point(ParsePoint point);
        void skip_to_recovery();
        // Types
        ast::Type *parse_type();
        ast::Type *parse_type_with_star();

        // Declarations
        ast::VarDecl *try_parse_var_decl();
        ast::ExternDecl *parse_extern_decl();

        // Statements
        ast::Stmt *parse_stmt();
        std::optional<ast::BlockItem> parse_block_item();
        ast::Stmt *parse_expr_stmt();
        ast::Stmt *parse_compound_stmt();
        ast::Stmt *parse_if_stmt();
        ast::Stmt *parse_while_stmt();
        ast::Stmt *parse_for_stmt();
        ast::Stmt *parse_return_stmt();
        ast::Stmt *parse_extern_call_stmt();

        // Expressions
        ast::Expr *parse_expr();
        ast::Expr *parse_assignment();
        ast::Expr *parse_equality();
        ast::Expr *parse_comparison();
        ast::Expr *parse_term();
        ast::Expr *parse_factor();
        ast::Expr *parse_unary();
        ast::Expr *parse_primary();

        // Helpers
        std::string unescape(const std::string &s) const;
        size_t add_string(const std::string &s);

        env::Session &session_;
        src::SourceId source_id_;
        lex::Lexer lexer_;
        lex::Token rdtoken_;
        std::pmr::monotonic_buffer_resource arena_;
        std::vector<std::function<void()>> cleanup_;
        std::vector<std::string> string_pool_;
    };
}
#endif
