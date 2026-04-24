#ifndef WATAFAC_LEX_LEXER_HPP
#define WATAFAC_LEX_LEXER_HPP
#include <cstddef>
#include <memory>
#include <watafac/Source.hpp>
#include <watafac/lex/Token.hpp>
namespace wfac::lex {
    class Lexer {
    public:
        explicit Lexer(std::shared_ptr<wfac::Source> source);
        Token next_token();
    private:
        int get_char();
        void unget_char();
        void pin_token();
        Token save_token();
        
        std::shared_ptr<wfac::Source> source_;
        char rdchar_ = '\0';
        std::size_t pinpos_ = 0, curpos_ = 0;
    };
}
#endif
