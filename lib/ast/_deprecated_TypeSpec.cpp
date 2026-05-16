#include <watafac/ast/TypeSpec.hpp>

namespace wfac::ast{
    PrimitiveTypeSpec::PrimitiveTypeSpec( PrimitiveTypeSpec::Kind kind)
        : kind_(kind)
    {}
    PrimitiveTypeSpec::Kind  PrimitiveTypeSpec::kind() const{
        return kind_;
    }
}
