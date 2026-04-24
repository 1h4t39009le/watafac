#ifndef WATAFAC_AST_EXPR_HPP
#define WATAFAC_AST_EXPR_HPP
#include <watafac/ast/Arche.hpp>

namespace wfac::ast {
    class Expr: public ArcheNode {
    public:
        virtual int eval() = 0;
        virtual ~Expr() = default;
    };
    class IntExpr: public NodeLink<Expr, IntExpr> {
    public:
        virtual int eval() override;
    private:
        int value_;
    };
    class BinopExpr: public NodeLink<Expr, BinopExpr> {
    public:
        virtual int eval() override;
    private:
        Expr &left_, &right_;
    };
}
#endif
