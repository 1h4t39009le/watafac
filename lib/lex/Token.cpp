#include <watafac/lex/Token.hpp>
namespace wfac::lex{
    using TKind = Token::Kind;
    Token::Token(TKind kind, Source::Location loc)
        : kind_(kind), loc_(loc)
    {}
    TKind Token::get_kind() const{ return kind_; }
    Source::Location Token::get_loc() const {return loc_;}
}
