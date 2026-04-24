#ifndef WATAFAC_AST_DECL_HPP
#define WATAFAC_AST_DECL_HPP
#include <watafac/ast/Arche.hpp>
#include <watafac/ast/Declarator.hpp>
#include <watafac/ast/TypeSpec.hpp>
#include <watafac/ast/Expr.hpp>
namespace wfac::ast {   
    class VarDecl : public NodeLink<ArcheNode, VarDecl>{
    public:
        explicit VarDecl(const PrimitiveTypeSpec &tspec,const Declarator &declarator,const Expr *init = nullptr);
    private:
        const PrimitiveTypeSpec &tspec_;
        const Declarator &declarator_;
        const Expr *init_ = nullptr;
    };
}
#endif
