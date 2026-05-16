#ifndef WATAFAC_LEX_TOKEN_HPP
#define WATAFAC_LEX_TOKEN_HPP
#include <cstddef>
#include <watafac/src/Source.hpp>
namespace wfac::lex {
    namespace src = wfac::src;
    
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
            LParen, RParen,
            LBrace, RBrace,
            Comma,
            
            //LITERALS
            IntLit,
            CharLit,
            StringLit,
            //KEYWORDS
            KwInt,
            KwChar,
            KwExtrn,
            KwIf,
            KwElse,
            KwWhile,
            KwFor,
            KwReturn,
            
            

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
            case Kind::LBrace:       return "{";
            case Kind::RBrace:       return "}";
            case Kind::Comma:        return ",";
            case Kind::IntLit:       return "INT";
            case Kind::CharLit:      return "CHAR";
            case Kind::StringLit:    return "STRING";
            case Kind::KwExtrn:      return "extrn";
            case Kind::KwInt:        return "int";
            case Kind::KwChar:       return "char";
            case Kind::KwIf:         return "if";
            case Kind::KwElse:       return "else";
            case Kind::KwWhile:      return "while";
            case Kind::KwFor:        return "for";
            case Kind::KwReturn:     return "return";
            case Kind::Ident:        return "IDENT";
            }
            return "???";
        }
        explicit Token(Kind kind, src::SourceLocation loc);
        Token() = default;
        Kind get_kind() const;
        src::SourceLocation get_loc() const;
    private:
        Kind kind_ = Kind::Invalid;
        src::SourceLocation loc_;
    };
}
#endif
