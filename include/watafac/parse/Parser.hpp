#ifndef WATAFAC_PARSE_PARSER_HPP
#define WATAFAC_PARSE_PARSER_HPP
#include <watafac/Source.hpp>
#include <watafac/lex/Lexer.hpp>
#include <watafac/ast/Ast.hpp>
#include <memory_resource>
#include <optional>
namespace wfac::parse {
    namespace lex = wfac::lex;
    namespace ast = wfac::ast;
    class Parser {
    public:
        explicit Parser(std::shared_ptr<wfac::Source> source);
        std::optional<ast::ProgramGroup> parse();
    private:
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
    };
}
/*
class Parser {
private:
    std::shared_ptr<wfac::Source> src_;
    std::pmr::monotonic_linear_allocator arena_;
};
*/
#endif
