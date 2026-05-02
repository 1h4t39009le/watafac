#include <watafac/lex/Token.hpp>
namespace wfac::lex{
    using TKind = Token::Kind;
    Token::Token(TKind kind, src::SourceLocation loc)
        : kind_(kind), loc_(loc)
    {}
    TKind Token::get_kind() const{ return kind_; }
    src::SourceLocation Token::get_loc() const {return loc_;}
}
