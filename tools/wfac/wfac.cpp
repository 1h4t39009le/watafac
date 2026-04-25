#include <iostream>
#include <memory>
#include <fstream>
#include <watafac/Source.hpp>
#include <watafac/lex/Lexer.hpp>
int main(int argc, char **argv){
    if(argc <= 1) return 1;
    char *source = argv[1];
    auto shrsrc = std::make_shared<wfac::FileSource>(source);
    auto lexer = wfac::lex::Lexer(shrsrc);
    wfac::lex::Token tk(wfac::lex::Token::Kind::Invalid, 0, 0);
    for(;;){
        tk = lexer.next_token();
        if(tk.get_kind() == wfac::lex::Token::Kind::Eof){
            std::cout << "EOF\n";
            break;
        } else std::cout<< "TOKEN = " << (int)(tk.get_kind()) << '\n';
    }
    std::cout << "Hello, World!\n";
}
