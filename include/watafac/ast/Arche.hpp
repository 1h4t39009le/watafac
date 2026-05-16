#ifndef WATAFAC_AST_ARCHE_HPP
#define WATAFAC_AST_ARCHE_HPP
namespace wfac::ast {

    //EXPR
    class IntExpr;
    class CharExpr;
    class StringExpr;
    class BinopExpr;

    class VarExpr;
    class UnaryExpr;
    class AssignExpr;


    //STMT
    class ExternCallStmt;
    class ExprStmt;
    class CompoundStmt;
    class IfStmt;
    class WhileStmt;
    class ForStmt;
    class ReturnStmt;

    //PROGRAM
    class Program;

    //TYPES
    class TermType;
    class PointerType;

    //DECLARATIONS
    class ExternDecl;
    class VarDecl;
    
    class Visitor {
    public:
        //PROGRAM
        virtual void visit(const Program &p) = 0;
        //EXPR
        virtual void visit(const IntExpr &e) = 0;
        virtual void visit(const CharExpr &e) = 0;
        virtual void visit(const StringExpr &e) = 0;
        virtual void visit(const BinopExpr &e) = 0;
        
        virtual void visit(const VarExpr &e) = 0;
        virtual void visit(const UnaryExpr &e) = 0;
        virtual void visit(const AssignExpr &e) = 0;

        //STMT
        virtual void visit(const ExternCallStmt &s) = 0;
        virtual void visit(const ExprStmt &s) = 0;
        virtual void visit(const CompoundStmt &s) = 0;
        virtual void visit(const IfStmt &s) = 0;
        virtual void visit(const WhileStmt &s) = 0;
        virtual void visit(const ForStmt &s) = 0;
        virtual void visit(const ReturnStmt &s) = 0;
        
        //DECLARATIONS
        virtual void visit(const VarDecl &d) = 0;
        virtual void visit(const ExternDecl &d) = 0;

        //TYPES
        virtual void visit(const TermType &t) = 0;
        virtual void visit(const PointerType &t) = 0;
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
