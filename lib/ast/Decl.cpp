#include <watafac/ast/Decl.hpp>

namespace wfac::ast {
    VarDecl::VarDecl(const Type *type, std::string name, const Expr *init)
        : type_(type), name_(std::move(name)), init_(init)
    {}
    const Type *VarDecl::type() const { return type_; }
    const std::string &VarDecl::name() const { return name_; }
    const Expr *VarDecl::init() const { return init_; }
    
    ExternDecl::ExternDecl(std::string name)
        : name_(std::move(name))
    {}
    const std::string &ExternDecl::name() const { return name_; }
}
