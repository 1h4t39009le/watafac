#ifndef WATAFAC_AST_ARCHE_HPP
#define WATAFAC_AST_ARCHE_HPP
namespace wfac::ast {

    //EXPR
    class IntExpr;
    class BinopExpr;

    class VarExpr;
    class UnaryExpr;
    class AssignExpr;

    //GROUPS
    class ProgramGroup;

    //DECLARATORS
    class TermDeclarator;
    class PointerDeclarator;

    //DECLARATIONS
    class VarDecl;

    //TYPE SPECIFIERS
    class PrimitiveTypeSpec;
    
    class Visitor {
    public:
        virtual void visit(IntExpr      &intlit) = 0;
        virtual void visit(BinopExpr    &binop) = 0;
        
        virtual void visit(VarExpr      &var) = 0;
        virtual void visit(UnaryExpr    &unary) = 0;
        virtual void visit(AssignExpr   &assign) = 0;
        
        virtual void visit(ProgramGroup &prg) = 0;
        
        virtual void visit(TermDeclarator    &td) = 0;
        virtual void visit(PointerDeclarator &ptrd) = 0;

        virtual void visit(VarDecl &vardecl) = 0;
        
        virtual void visit(PrimitiveTypeSpec &tspec) = 0;
    };
    class ArcheNode {
    public:
        virtual void accept(Visitor &visitor) = 0;
        virtual ~ArcheNode() = default;
    };
    template<typename Parent, typename Child>
    class NodeLink: public Parent {
    public:
        void accept(Visitor &v) override {
            v.visit(static_cast<Child&>(*this));
        }
    };
    
    
} //namespace wfac
#endif
