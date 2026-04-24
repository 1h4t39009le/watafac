#ifndef WATAFAC_AST_EXPR_HPP
#define WATAFAC_AST_EXPR_HPP
#include <watafac/ast/Arche.hpp>

namespace wfac::ast {
    class Expr: public ArcheNode {
        virtual int eval() = 0;
        virtual ~Expr() = default;
    };
    class IntExpr: public NodeLink<Expr, IntExpr> {
        virtual int eval() override;
    };
    class BinopExpr: public NodeLink<Expr, BinopExpr> {
        Expr &left_, &right_;
        virtual int eval() override;
    };
}
#endif
