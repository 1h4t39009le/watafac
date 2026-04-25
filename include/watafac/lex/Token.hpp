#ifndef WATAFAC_LEX_TOKEN_HPP
#define WATAFAC_LEX_TOKEN_HPP
#include <cstddef>
#include <watafac/Source.hpp>
namespace wfac::lex {
    using wfac::Source;
    class Token {
    public:
        enum class Kind {
            Eof,
            Invalid,
            // Operators
            Plus, Minus, Star, Slash, Percent,
            Equals, EqualsEq, NotEquals, Less, Greater, LessOrEq, GreaterOrEq,
            Bang, Ampersand, AmpersandAmp, Pipe, PipePip,
            Semicolon,
            LParen, RParen, Comma,
            
            //LITERALS
            Int,
            //KEYWORDS
            ReservedInt,

            //OTHERS
            Ident
        };
        static const char *kind_name(Kind kind){
            switch (kind) {
            case Kind::Eof:          return "EOF";
            case Kind::Invalid:      return "INVALID";
            case Kind::Plus:         return "+";
            case Kind::Minus:        return "-";
            case Kind::Star:         return "*";
            case Kind::Slash:        return "/";
            case Kind::Percent:      return "%";
            case Kind::Equals:       return "=";
            case Kind::EqualsEq:     return "==";
            case Kind::NotEquals:    return "!=";
            case Kind::Less:         return "<";
            case Kind::Greater:      return ">";
            case Kind::LessOrEq:     return "<=";
            case Kind::GreaterOrEq:  return ">=";
            case Kind::Bang:         return "!";
            case Kind::Ampersand:    return "&";
            case Kind::AmpersandAmp: return "&&";
            case Kind::Pipe:         return "|";
            case Kind::PipePip:      return "||";
            case Kind::Semicolon:    return ";";
            case Kind::LParen:       return "(";
            case Kind::RParen:       return ")";
            case Kind::Comma:        return ",";
            case Kind::Int:          return "INT";
            case Kind::ReservedInt:  return "int";
            case Kind::Ident:        return "IDENT";
            }
            return "???";
        }
        explicit Token(Kind kind, Source::Location loc);
        Token() = default;
        Kind get_kind() const;
        Source::Location get_loc() const;
    private:
        Kind kind_ = Kind::Invalid;
        Source::Location loc_;
    };
}
#endif
