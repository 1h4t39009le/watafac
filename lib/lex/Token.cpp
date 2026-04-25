#include <watafac/lex/Token.hpp>
namespace wfac::lex{
    using TKind = Token::Kind;
    Token::Token(TKind kind, std::size_t start, std::size_t end)
        : kind_(kind), start_(start), end_(end)
    {}
    TKind Token::get_kind() const{ return kind_; }
}
