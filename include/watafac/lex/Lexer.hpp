#ifndef WATAFAC_LEX_LEXER_HPP
#define WATAFAC_LEX_LEXER_HPP
#include <cstddef>
#include <memory>
#include <watafac/env/Session.hpp>
#include <watafac/lex/Token.hpp>

namespace wfac::lex {
    namespace src = wfac::src;
    namespace diag = wfac::diag;
    class Lexer {
    public:
        explicit Lexer(env::Session &session, src::SourceId source_id);
        Token next_token();
        std::string get_lexeme(src::SourceLocation loc);
    private:
        int get_char();
        void unget_char();
        void pin_token();
        Token save_token(Token::Kind kind);
        std::string get_lexeme();

        env::Session &session_;
        src::SourceId source_id_;
        std::unique_ptr<std::istream> stream_;
        int rdchar_ = EOF;
        std::size_t pinpos_ = 0, curpos_ = 0;
    };
}
#endif
