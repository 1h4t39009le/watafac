#include <watafac/lex/Lexer.hpp>
#include <cctype>
namespace wfac::lex {
    using TKind = Token::Kind;
    Lexer::Lexer(std::shared_ptr<wfac::Source> source)
        : source_{std::move(source)}, stream_{source_->get_istream()}
    {}
    Token Lexer::next_token(){
        for(;;){
            pin_token();
            int c = get_char();
            switch(c){
            case EOF:
                return save_token(TKind::Eof);
            case '+':
                return save_token(TKind::Plus);
            case '*':
                return save_token(TKind::Star);
            case '=':
                return save_token(TKind::Equals);
            case ';':
                return save_token(TKind::Semicolon);
            default:
                if(std::isspace(c))
                    continue;
                if(std::isdigit(c)){
                    for(;;){
                        c = get_char();
                        if(!std::isdigit(c)){
                            unget_char();
                            return save_token(TKind::Int);
                        }
                    }
                }
                if(std::isalpha(c)){
                    for(;;){
                        c = get_char();
                        if(!std::isalnum(c)){
                            unget_char();
                            std::string ident_str = "[TODO: implement stream read]";
                            if(ident_str == "int"){
                                return save_token(TKind::ReservedInt);
                            }
                            return save_token(TKind::Ident);
                        }
                    }
                    
                }
            }
        }
    }

    int Lexer::get_char(){
        if((rdchar_ = stream_->get()) != EOF){
            ++curpos_;
        }
        return rdchar_;
    }

    void Lexer::unget_char(){
        if(curpos_ > 0){
            stream_->putback(rdchar_);
            --curpos_;
        }
    }

    void Lexer::pin_token(){
        pinpos_ = curpos_;
    }

    Token Lexer::save_token(TKind kind){
        return Token(kind, pinpos_ , curpos_);
    }
}
