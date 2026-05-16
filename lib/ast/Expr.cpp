#include <watafac/ast/Expr.hpp>
namespace wfac::ast {
    IntExpr::IntExpr(int value)
        : value_(value)
    {}
    int IntExpr::value() const{ return value_; }

    CharExpr::CharExpr(char value)
        : value_(value)
    {}
    char CharExpr::value() const { return value_; }

    StringExpr::StringExpr(size_t string_id)
        : string_id_(string_id)
    {}
    size_t StringExpr::string_id() const { return string_id_; }

    BinopExpr::BinopExpr(Expr *left, BinopExpr::Kind kind, Expr *right)
        : left_(left), kind_(kind), right_(right)
    {}
    const char *BinopExpr::kind_name(BinopExpr::Kind kind) {
        switch (kind) {
        case BinopExpr::Kind::Add:         return "+";
        case BinopExpr::Kind::Sub:         return "-";
        case BinopExpr::Kind::Mul:         return "*";
        case BinopExpr::Kind::Div:         return "/";
        case BinopExpr::Kind::Mod:         return "%";
        case BinopExpr::Kind::Equals:      return "==";
        case BinopExpr::Kind::NotEquals:   return "!=";
        case BinopExpr::Kind::Less:        return "<";
        case BinopExpr::Kind::Greater:     return ">";
        case BinopExpr::Kind::LessOrEq:    return "<=";
        case BinopExpr::Kind::GreaterOrEq: return ">=";
        case BinopExpr::Kind::LogicAnd:    return "&&";
        case BinopExpr::Kind::LogicOr:     return "||";
        default: return "???";
        }
    }
    const Expr *BinopExpr::left() const { return left_; }
    const Expr *BinopExpr::right() const { return right_; }
    BinopExpr::Kind BinopExpr::kind() const { return kind_; }
    
    VarExpr::VarExpr(std::string ident)
        : ident_{std::move(ident)} {}
    const std::string &VarExpr::ident() const { return ident_; }
    
    UnaryExpr::UnaryExpr(Expr *inner, UnaryExpr::Kind kind)
        : inner_(inner), kind_(kind)
    {}
    const char *UnaryExpr::kind_name(UnaryExpr::Kind kind) {
        switch (kind) {
        case UnaryExpr::Kind::Not:      return "!";
        case UnaryExpr::Kind::Negative: return "-";
        case UnaryExpr::Kind::AddrOf:   return "&";
        case UnaryExpr::Kind::Deref:    return "*";
        default: return "???";
        }
    }
    const Expr *UnaryExpr::inner() const { return inner_; }
    UnaryExpr::Kind UnaryExpr::kind() const { return kind_; }
    
    AssignExpr::AssignExpr(Expr *left, Expr *right)
        : left_(left), right_(right)
    {}
    const Expr *AssignExpr::left() const { return left_; }
    const Expr *AssignExpr::right() const { return right_; }
}
