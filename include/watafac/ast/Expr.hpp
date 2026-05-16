#ifndef WATAFAC_AST_EXPR_HPP
#define WATAFAC_AST_EXPR_HPP
#include <watafac/ast/Arche.hpp>
#include <string>
namespace wfac::ast {
    class Expr: public ArcheNode {
    public:
        virtual ~Expr() = default;
    };
    class IntExpr: public NodeLink<Expr, IntExpr> {
    public:
        explicit IntExpr(int value);
        int value() const;
    private:
        int value_;
    };
    class CharExpr : public NodeLink<Expr, CharExpr> {
    public:
        explicit CharExpr(char value);
        char value() const;
    private:
        char value_;
    };
    
    class StringExpr : public NodeLink<Expr, StringExpr> {
    public:
        explicit StringExpr(size_t string_id);
        size_t string_id() const;
    private:
        size_t string_id_;
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
        static const char *kind_name(Kind kind);
        explicit BinopExpr(Expr *left, Kind kind, Expr *right);
        const Expr *left() const;
        const Expr *right() const;
        Kind kind() const;
    private:
        Expr *left_, *right_;
        Kind kind_;
    };
    class VarExpr: public NodeLink<Expr, VarExpr>{
    public:
        explicit VarExpr(std::string ident);
        const std::string &ident() const;
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
        static const char *kind_name(Kind kind);
        explicit UnaryExpr(Expr *inner, Kind kind);
        const Expr *inner() const;
        Kind kind() const;
    private:
        Expr *inner_;
        Kind kind_;
    };
    class AssignExpr: public NodeLink<Expr, AssignExpr>{
    public:
        explicit AssignExpr(Expr *left, Expr *right);
        const Expr *left() const;
        const Expr *right() const;
    private:
        Expr *left_, *right_;
    };
}
#endif
