#include <iostream>
#include <memory>
#include <fstream>
#include <watafac/Source.hpp>
#include <watafac/lex/Lexer.hpp>
#include <watafac/parse/Parser.hpp>
int main(int argc, char **argv){
    if(argc <= 1) return 1;
    char *source = argv[1];
    auto shrsrc = std::make_shared<wfac::FileSource>(source);
    auto parser = wfac::parse::Parser(shrsrc);
    auto *expr = parser.parse_expr();
    
    std::cout << "Hello, World! " << expr << "\n";
    if(expr){
        std::cout << expr->eval() << "\n";
    }
}
