#ifndef WATAFAC_LEX_TOKEN_HPP
#define WATAFAC_LEX_TOKEN_HPP
#include <cstddef>
namespace wfac::lex {
    class Token {
    public:
        enum class Kind {
            Eof,
            Invalid,
            Plus,Star,Semicolon,Equals,
            //LITERALS
            Int,
            //KEYWORDS
            ReservedInt,

            //OTHERS
            Ident
        };
        explicit Token(Kind kind, std::size_t start, std::size_t end);
        Kind get_kind() const;
        std::size_t get_start() const;
        std::size_t get_end() const;
    private:
        Kind kind_;
        std::size_t start_, end_;
    };
}
#endif
