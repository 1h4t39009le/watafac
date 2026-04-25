#include <watafac/ast/Expr.hpp>
namespace wfac::ast {
    IntExpr::IntExpr(int value)
        : value_(value)
    {}
    int IntExpr::eval() {
        return value_;
    }

    BinopExpr::BinopExpr(Expr *left, BinopExpr::Kind kind, Expr *right)
        : left_(left), kind_(kind), right_(right)
    {}
    int BinopExpr::eval() {
        switch(kind_){
        case BinopExpr::Kind::Add: return left_->eval() + right_->eval();
        case BinopExpr::Kind::Sub: return left_->eval() - right_->eval();
        case BinopExpr::Kind::Mul: return left_->eval() * right_->eval();
        case BinopExpr::Kind::Div: return left_->eval() / right_->eval();
        case BinopExpr::Kind::Mod: return left_->eval() % right_->eval();
        case BinopExpr::Kind::Equals:      return left_->eval() == right_->eval();
        case BinopExpr::Kind::NotEquals:   return left_->eval() != right_->eval();
        case BinopExpr::Kind::Less:        return left_->eval() < right_->eval();
        case BinopExpr::Kind::Greater:     return left_->eval() > right_->eval();
        case BinopExpr::Kind::LessOrEq:    return left_->eval() <= right_->eval();
        case BinopExpr::Kind::GreaterOrEq: return left_->eval() >= right_->eval();
        case BinopExpr::Kind::LogicAnd:    return left_->eval() && right_->eval();
        case BinopExpr::Kind::LogicOr:     return left_->eval() || right_->eval();
        }
    }
    VarExpr::VarExpr(std::string ident)
        : ident_{std::move(ident)} {}
    int VarExpr::eval() {
        return ident_.length();
    }

    UnaryExpr::UnaryExpr(Expr *inner, UnaryExpr::Kind kind)
        : inner_(inner), kind_(kind)
    {}
    int UnaryExpr::eval() {
        switch(kind_){
        case UnaryExpr::Kind::Not:
            return !(inner_->eval());
        case UnaryExpr::Kind::Negative:
            return -(inner_->eval());
        default:
            return 0;
        }
    }
    
    
    AssignExpr::AssignExpr(Expr *left, Expr *right)
        : left_(left), right_(right)
    {}
    int AssignExpr::eval() {
        return 0;
    }
}
