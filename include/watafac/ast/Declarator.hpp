#ifndef WATAFAC_AST_DECLARATOR_HPP
#define WATAFAC_AST_DECLARATOR_HPP
#include <watafac/ast/Arche.hpp>
#include <string>
namespace wfac::ast {
    class Declarator: public ArcheNode {
    public:
        virtual const std::string& get_name() const = 0;
        virtual ~Declarator() = default;
    };
    class TermDeclarator : public NodeLink<Declarator, TermDeclarator> {
    public:
        const std::string& get_name() const override;
        explicit TermDeclarator(std::string name);
    private:
        std::string name_;
    };
}
#endif
