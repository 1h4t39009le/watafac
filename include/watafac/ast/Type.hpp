#ifndef WATAFAC_AST_TYPE_HPP
#define WATAFAC_AST_TYPE_HPP
#include <watafac/ast/Arche.hpp>

namespace wfac::ast {
    
    class Type : public ArcheNode {
    public:
        virtual ~Type() = default;
    };
    
    class TermType : public NodeLink<Type, TermType> {
    public:
        enum Kind { Int, Char };
        explicit TermType(Kind kind);
        Kind kind() const;
        static const char *kind_name(Kind kind);
    private:
        Kind kind_;
    };
    
    class PointerType : public NodeLink<Type, PointerType> {
    public:
        explicit PointerType(Type *pointee);
        const Type *pointee() const;
    private:
        Type *pointee_;
    };
    
}
#endif
