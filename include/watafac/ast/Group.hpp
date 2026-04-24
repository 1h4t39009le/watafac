#ifndef WATAFAC_AST_GROUP_HPP
#define WATAFAC_AST_GROUP_HPP
#include <watafac/ast/Arche.hpp>
#include <vector>
#include <functional>
namespace wfac::ast {
    class ProgramGroup: public NodeLink<ArcheNode, ProgramGroup>{
    public:
        void add_decl(VarDecl &decl);
        auto begin() const ;
        auto end() const ;
    private:
        std::vector<std::reference_wrapper<VarDecl>> decls_;
    };
}
#endif
