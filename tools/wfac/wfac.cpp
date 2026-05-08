#include <watafac/env/Session.hpp>
#include <watafac/parse/Parser.hpp>
#include <watafac/ast/Expr.hpp>
#include <iostream>

class PrintVisitor: public wfac::ast::Visitor {
public:
    virtual void visit(const wfac::ast::IntExpr &e) override{
        print_indent() << "INT: " << e.value() << '\n';
    }
    virtual void visit(const wfac::ast::BinopExpr &e) override{
        print_indent()<< "BINOP: " << wfac::ast::BinopExpr::kind_name(e.kind()) << '\n';
        enter_block();{
            e.left()->accept(*this);
            e.right()->accept(*this);
        } leave_block();
    }
        
    virtual void visit(const wfac::ast::VarExpr &e) override{
        print_indent() << "var(" << e.ident() << ")" << '\n';
    }
    virtual void visit(const wfac::ast::UnaryExpr &e) override{
        print_indent() << "UNARY: " << wfac::ast::UnaryExpr::kind_name(e.kind()) << '\n';
        enter_block();{
            e.inner()->accept(*this);
        } leave_block();
        
    }
    virtual void visit(const wfac::ast::AssignExpr &e) override{
        print_indent() << "ASSIGN =\n";
        enter_block();{
            e.left()->accept(*this);
            e.right()->accept(*this);
        } leave_block();
    }
    virtual void visit(const wfac::ast::ExprStmt &s) override {
        print_indent() << "EXPR_STMT\n";
        enter_block();
        if (s.expr()) s.expr()->accept(*this);
        leave_block();
    }

    virtual void visit(const wfac::ast::CompoundStmt &s) override {
        print_indent() << "{\n";
        enter_block();
        for (const auto *item : s) {
            item->accept(*this);
        }
        leave_block();
        print_indent() << "}\n";
    }

    virtual void visit(const wfac::ast::IfStmt &s) override {
        print_indent() << "IF\n";
        enter_block();
        print_indent() << "COND:\n";
        s.cond()->accept(*this);
        print_indent() << "THEN:\n";
        s.then_stmt()->accept(*this);
        if (s.else_stmt()) {
            print_indent() << "ELSE:\n";
            s.else_stmt()->accept(*this);
        }
        leave_block();
    }

    virtual void visit(const wfac::ast::WhileStmt &s) override {
        print_indent() << "WHILE\n";
        enter_block();
        print_indent() << "COND:\n";
        s.cond()->accept(*this);
        print_indent() << "BODY:\n";
        s.body()->accept(*this);
        leave_block();
    }

    virtual void visit(const wfac::ast::ForStmt &s) override {
        print_indent() << "FOR\n";
        enter_block();
        if (s.init()) {
            print_indent() << "INIT:\n";
            s.init()->accept(*this);
        }
        if (s.cond()) {
            print_indent() << "COND:\n";
            s.cond()->accept(*this);
        }
        if (s.iter()) {
            print_indent() << "ITER:\n";
            s.iter()->accept(*this);
        }
        print_indent() << "BODY:\n";
        s.body()->accept(*this);
        leave_block();
    }

    virtual void visit(const wfac::ast::ReturnStmt &s) override {
        print_indent() << "RETURN\n";
        enter_block();
        if (s.expr()) s.expr()->accept(*this);
        leave_block();
    }

private:
    std::ostream& print_indent(){
        return std::cout << std::string(indent_, ' ');
    }
    void enter_block(){ indent_ += 2; }
    void leave_block(){ indent_ -= 2; }
    int indent_ = 0;
};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: wfac <file>\n";
        return 1;
    }

    wfac::env::Session session;
    auto id = session.sources.add_source(
        wfac::src::FileSource(argv[1])
    );

    wfac::parse::Parser parser(session, id);
    wfac::ast::Stmt *stmt = parser.parse_stmt();

    if (stmt) {
        PrintVisitor printer;
        stmt->accept(printer);
    }

    session.diags.print_all(std::cerr, session.sources);
    return session.diags.has_errors() ? 1 : 0;
}
