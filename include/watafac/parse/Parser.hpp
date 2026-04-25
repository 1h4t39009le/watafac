#ifndef WATAFAC_PARSE_PARSER_HPP
#define WATAFAC_PARSE_PARSER_HPP
#include <watafac/Source.hpp>
#include <watafac/lex/Lexer.hpp>
#include <watafac/ast/Expr.hpp>
#include <memory_resource>
#include <optional>
#include <functional>
namespace wfac::parse {
    namespace lex = wfac::lex;
    namespace ast = wfac::ast;
    using TKind = lex::Token::Kind;
    class Parser {
    public:
        explicit Parser(std::shared_ptr<wfac::Source> source);
        ast::Expr *parse_expr(); // REMOVE FROM HERE
        std::optional<ast::ProgramGroup> parse();
        ~Parser();
    private:
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
        ast::Declarator *parse_declarator();
        ast::PrimitiveTypeSpec *parse_type_spec();
        ast::VarDecl *parse_var_decl();
        bool parse_program(ast::ProgramGroup &program);
        */
        lex::Token next_token();
        bool match_kind(TKind kind, bool eat = true);
        
        
        ast::Expr *parse_assignment();
        ast::Expr *parse_equality();
        ast::Expr *parse_comparison();
        ast::Expr *parse_term();
        ast::Expr *parse_factor();
        ast::Expr *parse_unary();
        ast::Expr *parse_primary();

        
        
        lex::Lexer lexer_;
        std::shared_ptr<wfac::Source> source_;
        lex::Token rdtoken_;
        std::pmr::monotonic_buffer_resource arena_;
        std::vector<std::function<void()>> cleanup_;
    };
}
#endif
