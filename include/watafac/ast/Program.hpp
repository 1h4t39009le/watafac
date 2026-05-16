#ifndef WATAFAC_AST_PROGRAM_HPP
#define WATAFAC_AST_PROGRAM_HPP
#include <watafac/ast/Arche.hpp>
#include <watafac/ast/Decl.hpp>
#include <watafac/ast/Stmt.hpp>
#include <vector>

namespace wfac::ast {
    class Program : public NodeLink<ArcheNode, Program> {
    public:
        using Items = std::vector<BlockItem>;
        
        void add_item(BlockItem item);
        Items::const_iterator begin() const;
        Items::const_iterator end() const;
    private:
        Items items_;
    };
}
#endif
