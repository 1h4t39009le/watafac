#include <watafac/ast/Type.hpp>
namespace wfac::ast {
    
    TermType::TermType(Kind kind)
        : kind_(kind)
    {}
    
    TermType::Kind TermType::kind() const { return kind_; }
    
    const char *TermType::kind_name(Kind kind) {
        switch (kind) {
        case Kind::Int:  return "int";
        case Kind::Char: return "char";
        default: return "???";
        }
    }
    
    PointerType::PointerType(Type *pointee)
        : pointee_(pointee)
    {}
    
    const Type *PointerType::pointee() const { return pointee_; }
    
}

