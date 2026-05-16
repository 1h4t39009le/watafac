#include <watafac/ast/Program.hpp>

namespace wfac::ast {
    void Program::add_item(BlockItem item) {
        items_.push_back(std::move(item));
    }
    
    Program::Items::const_iterator Program::begin() const {
        return items_.begin();
    }
    
    Program::Items::const_iterator Program::end() const {
        return items_.end();
    }
}
