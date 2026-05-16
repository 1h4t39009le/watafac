#ifndef WATAFAC_AST_DECL_HPP
#define WATAFAC_AST_DECL_HPP
#include <watafac/ast/Arche.hpp>
#include <watafac/ast/Type.hpp>
#include <watafac/ast/Expr.hpp>
#include <string>
namespace wfac::ast {
    
    class Decl : public ArcheNode {
    public:
        virtual ~Decl() = default;
    };
    
    class VarDecl : public NodeLink<Decl, VarDecl> {
    public:
        explicit VarDecl(const Type *type, std::string name, const Expr *init = nullptr);
        const Type *type() const;
        const std::string &name() const;
        const Expr *init() const;
    private:
        const Type *type_;
        std::string name_;
        const Expr *init_ = nullptr;
    };
    
    class ExternDecl : public NodeLink<Decl, ExternDecl> {
    public:
        explicit ExternDecl(std::string name);
        const std::string &name() const;
    private:
        std::string name_;
    };
}
#endif
