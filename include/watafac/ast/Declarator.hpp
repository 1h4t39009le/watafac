#ifndef WATAFAC_AST_DECLARATOR_HPP
#define WATAFAC_AST_DECLARATOR_HPP
#include <watafac/ast/Arche.hpp>
#include <string>
#include <cstddef>
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
    class PointerDeclarator : public NodeLink<Declarator, PointerDeclarator>{
    public:
        const std::string& get_name() const override;
        explicit PointerDeclarator(const Declarator &inner, std::size_t power=1);
        const Declarator &get_inner();
        std::size_t get_power();
    private:
        const Declarator &inner_;
        std::size_t power_;
    };
}
#endif
