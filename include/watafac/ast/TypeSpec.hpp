#ifndef WATAFAC_AST_TYPESPEC_HPP
#define WATAFAC_AST_TYPESPEC_HPP
#include <watafac/ast/Arche.hpp>
namespace wfac::ast {
    class PrimitiveTypeSpec : public  NodeLink<ArcheNode, PrimitiveTypeSpec> {
    public:
        enum class Kind {
            Int, Char
        };
        explicit PrimitiveTypeSpec(Kind kind);
        Kind get_kind() const;
    private:
        Kind kind_;
    };
}
#endif
