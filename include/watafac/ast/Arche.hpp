#ifndef WATAFAC_AST_ARCHE_HPP
#define WATAFAC_AST_ARCHE_HPP
namespace wfac::ast {

    //EXPR
    class IntExpr;
    class BinopExpr;

    class VarExpr;
    class UnaryExpr;
    class AssignExpr;

    //STMT
    class ExprStmt;
    class CompoundStmt;
    class IfStmt;
    class WhileStmt;
    class ForStmt;
    class ReturnStmt;

    //GROUPS
    class ProgramGroup; //TODO

    //DECLARATORS
    class TermDeclarator; //TODO
    class PointerDeclarator; //TODO

    //DECLARATIONS
    class VarDecl; //TODO

    //TYPE SPECIFIERS
    class PrimitiveTypeSpec; //TODO
    
    class Visitor {
    public:
        //EXPR
        virtual void visit(const IntExpr &e) = 0;
        virtual void visit(const BinopExpr &e) = 0;
        
        virtual void visit(const VarExpr &e) = 0;
        virtual void visit(const UnaryExpr &e) = 0;
        virtual void visit(const AssignExpr &e) = 0;

        //STMT
        virtual void visit(const ExprStmt &s) = 0;
        virtual void visit(const CompoundStmt &s) = 0;
        virtual void visit(const IfStmt &s) = 0;
        virtual void visit(const WhileStmt &s) = 0;
        virtual void visit(const ForStmt &s) = 0;
        virtual void visit(const ReturnStmt &s) = 0;
        
        /*virtual void visit(ProgramGroup &prg) = 0;
        
        virtual void visit(TermDeclarator    &td) = 0;
        virtual void visit(PointerDeclarator &ptrd) = 0;

        virtual void visit(VarDecl &vardecl) = 0;
        
        virtual void visit(PrimitiveTypeSpec &tspec) = 0;*/
    };
    class ArcheNode {
    public:
        virtual void accept(Visitor &visitor) const = 0;
        virtual ~ArcheNode() = default;
    };
    template<typename Parent, typename Child>
    class NodeLink: public Parent {
    public:
        void accept(Visitor &v) const override {
            v.visit(static_cast<const Child&>(*this));
        }
    };
    
    
} //namespace wfac
#endif
