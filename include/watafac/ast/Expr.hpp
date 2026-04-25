#ifndef WATAFAC_AST_EXPR_HPP
#define WATAFAC_AST_EXPR_HPP
#include <watafac/ast/Arche.hpp>
#include <string>
namespace wfac::ast {
    class Expr: public ArcheNode {
    public:
        virtual int eval() = 0;
        virtual ~Expr() = default;
    };
    class IntExpr: public NodeLink<Expr, IntExpr> {
    public:
        virtual int eval() override;
        explicit IntExpr(int value);
    private:
        int value_;
    };
    class BinopExpr: public NodeLink<Expr, BinopExpr> {
    public:
        enum class Kind {
            Add,
            Sub,
            Mul,
            Div,
            Mod,
            Equals,
            NotEquals,
            Less,
            Greater,
            LessOrEq,
            GreaterOrEq,
            LogicAnd,
            LogicOr
        };
        virtual int eval() override;
        explicit BinopExpr(Expr *left, Kind kind, Expr *right);
    private:
        Expr *left_, *right_;
        Kind kind_;
    };
    class VarExpr: public NodeLink<Expr, VarExpr>{
    public:
        virtual int eval() override;
        explicit VarExpr(std::string ident);
    private:
        std::string ident_;
    };
    class UnaryExpr: public NodeLink<Expr, UnaryExpr>{
    public:
        enum class Kind {
            Not,
            Negative,
            AddrOf,
            Deref
        };
        virtual int eval() override;
        explicit UnaryExpr(Expr *inner, Kind kind);
    private:
        Expr *inner_;
        Kind kind_;
    };
    class AssignExpr: public NodeLink<Expr, AssignExpr>{
    public:
        virtual int eval() override;
        explicit AssignExpr(Expr *left, Expr *right);
    private:
        Expr *left_, *right_;
    };
}
#endif
