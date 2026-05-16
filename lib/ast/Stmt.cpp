#include <watafac/ast/Stmt.hpp>

namespace wfac::ast {
    ExprStmt::ExprStmt(Expr *expr)
        :expr_(expr)
    {}
    const Expr *ExprStmt::expr() const { return expr_; }
    
    void CompoundStmt::add_item(BlockItem item){
        items_.push_back(item);
    }
    auto CompoundStmt::begin() const -> CompoundStmt::Items::const_iterator{
        return items_.begin();
    }
    auto CompoundStmt::end() const -> CompoundStmt::Items::const_iterator{
        return items_.end();
    }

    IfStmt::IfStmt(Expr *cond, Stmt *then, Stmt *els)
        :cond_(cond), then_(then), else_(els)
    {}
    
    const Expr *IfStmt::cond() const { return cond_; }
    const Stmt *IfStmt::then_stmt() const { return then_; }
    const Stmt *IfStmt::else_stmt() const { return else_; }

    WhileStmt::WhileStmt(Expr *cond, Stmt *body)
        : cond_(cond), body_(body)
    {}
    const Expr *WhileStmt::cond() const { return cond_; }
    const Stmt *WhileStmt::body() const { return body_; }

    ForStmt::ForStmt(Expr *init, Expr *cond, Expr *iter, Stmt *body)
        : init_(init), cond_(cond), iter_(iter), body_(body)
    {}
    const Expr *ForStmt::init() const { return init_; }
    const Expr *ForStmt::cond() const { return cond_; }
    const Expr *ForStmt::iter() const { return iter_; }
    const Stmt *ForStmt::body() const { return body_; }

    ReturnStmt::ReturnStmt(Expr *expr)
        : expr_(expr)
    {}
    const Expr *ReturnStmt::expr() const { return expr_; }

    ExternCallStmt::ExternCallStmt(std::string name, std::vector<Expr*> args)
        : name_(std::move(name)), args_(std::move(args))
    {}
    const std::string &ExternCallStmt::name() const { return name_; }
    const std::vector<Expr*> &ExternCallStmt::args() const { return args_; }
}
