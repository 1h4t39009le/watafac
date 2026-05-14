#include <watafac/ast/Decl.hpp>

namespace wfac::ast {
    VarDecl::VarDecl(const PrimitiveTypeSpec *tspec,const Declarator *declarator,const Expr *init)
        : tspec_(tspec), declarator_(declarator), init_(init)
    {}
    const PrimitiveTypeSpec *VarDecl::type_spec() const{
        return tspec_;
    }
    const Declarator *VarDecl::declarator() const{
        return declarator_;
    }
    const Expr *VarDecl::init() const{
        return init_;
    }
}
