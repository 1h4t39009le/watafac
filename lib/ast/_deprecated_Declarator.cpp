#include <watafac/ast/Declarator.hpp>

namespace wfac::ast{
    const std::string& TermDeclarator::name() const {
        return name_;
    }
    TermDeclarator::TermDeclarator(std::string name)
        : name_(std::move(name))
    {}

    PointerDeclarator::PointerDeclarator(const Declarator *inner, std::size_t power)
        : inner_(inner), power_(power)
    {}
    const std::string& PointerDeclarator::name() const {
        return inner_->name();
    }
    const Declarator *PointerDeclarator::inner() const{
        return inner_;
    }
    std::size_t PointerDeclarator::power() const{
        return power_;
    }

}
