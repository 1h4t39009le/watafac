#ifndef WATAFAC_PARSE_PARSER_HPP
#define WATAFAC_PARSE_PARSER_HPP
#include <watafac/Source.hpp>
#include <watafac/lex/Lexer.hpp>
#include <watafac/ast/Ast.hpp>
#include <memory_resource>
#include <optional>
#include <functional>
namespace wfac::parse {
    namespace lex = wfac::lex;
    namespace ast = wfac::ast;
    class Parser {
    public:
        explicit Parser(std::shared_ptr<wfac::Source> source);
        std::optional<ast::ProgramGroup> parse();
    private:
        template<typename T, typename... Args>
        T *alloc(Args&&.... args){
            void *ptr = arena_.alloc(sizeof(T), alignof(T));
            T *node = new (ptr) T(std::forward<Args>(args)...);
            if constexpr (!std::is_trivially_destructible_v<T>){
                cleanup_.push_back([node]{std::destroy_at(node);});
            }
            return node;
        }
        ~Parser();
        ast::Expr *parse_prim();
        ast::Expr *parse_addition();
        ast::Declarator *parse_declarator();
        ast::PrimitiveTypeSpec *parse_type_spec();
        ast::VarDecl *parse_var_decl();
        bool parse_program(ast::ProgramGroup &program);
        
        std::shared_ptr<wfac::Source> source_;
        lex::Lexer lexer_;
        lex::Token rdtoken_;
        std::pmr::monotonic_buffer_resource arena_;
        std::vector<std::function<void()>> cleanup_;
    };
}
#endif
