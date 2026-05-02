#include <watafac/env/Session.hpp>
#include <watafac/parse/Parser.hpp>
#include <watafac/ast/Expr.hpp>
#include <iostream>

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
    auto expr = parser.parse_expr();

    if (expr) {
        std::cout << "Eval: " << expr->eval() << "\n";
    }

    session.diags.print_all(std::cerr, session.sources);
    return session.diags.has_errors() ? 1 : 0;
}
