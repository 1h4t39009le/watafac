#ifndef WATAFAC_AST_STMT_HPP
#define WATAFAC_AST_STMT_HPP
#include <watafac/ast/Arche.hpp>
#include <watafac/ast/Expr.hpp>
#include <vector>
#include <variant>

namespace wfac::ast {
    class Stmt : public ArcheNode {
    public:
        virtual ~Stmt() = default;
    };

    class ExprStmt: public NodeLink<Stmt, ExprStmt>{
    public:
        explicit ExprStmt(Expr *expr);
        const Expr *expr() const;
    private:
        Expr *expr_;
    };

    using BlockItem = std::variant<Stmt*, VarDecl*>;
    class CompoundStmt: public NodeLink<Stmt, CompoundStmt>{
    public:
        using Items = std::vector<BlockItem>;
        Items::const_iterator begin() const;
        Items::const_iterator end() const;
        void add_item(BlockItem item);
    private:
        Items items_;
    };

    class IfStmt: public NodeLink<Stmt, IfStmt>{
    public:
        explicit IfStmt(Expr *cond, Stmt *then, Stmt *els = nullptr);
        const Expr *cond() const;
        const Stmt *then_stmt() const;
        const Stmt *else_stmt() const;
    private:
        Expr *cond_;
        Stmt *then_;
        Stmt *else_;
    };

    class WhileStmt: public NodeLink<Stmt, WhileStmt>{
    public:
        explicit WhileStmt(Expr *cond, Stmt *body);
        const Expr *cond() const;
        const Stmt *body() const;
    private:
        Expr *cond_;
        Stmt *body_;
    };

    class ForStmt: public NodeLink<Stmt, ForStmt>{
    public:
        explicit ForStmt(Expr *init, Expr *cond, Expr *iter, Stmt *body);
        const Expr *init() const;
        const Expr *cond() const;
        const Expr *iter() const;
        const Stmt *body() const;
    private:
        Expr *init_;
        Expr *cond_;
        Expr *iter_;
        Stmt *body_;
    };

    class ReturnStmt: public NodeLink<Stmt, ReturnStmt>{
    public:
        explicit ReturnStmt(Expr *expr = nullptr);
        const Expr *expr() const;
    private:
        Expr *expr_;
    };
}
#endif
