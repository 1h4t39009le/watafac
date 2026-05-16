#include <watafac/lex/Lexer.hpp>
#include <cctype>
namespace wfac::lex {
    using TKind = Token::Kind;
    Lexer::Lexer(env::Session& session, src::SourceId source_id)
        : session_(session), source_id_{source_id}
    {
        if(auto src = session.sources.get_source(source_id_))
            stream_ = src->get_istream();
    }
    Token Lexer::next_token(){
        for(;;){
            pin_token();
            int c = get_char();
            switch(c){
            case EOF:
                return save_token(TKind::Eof);
            case '+':
                return save_token(TKind::Plus);
            case '-':
                return save_token(TKind::Minus);
            case '*':
                return save_token(TKind::Star);
            case '/':
                return save_token(TKind::Slash);
            case '%':
                return save_token(TKind::Percent);
            case '!':
                if(get_char() == '='){
                    return save_token(TKind::NotEquals);
                }
                unget_char();
                return save_token(TKind::Bang);
            case '<':
                if(get_char() == '='){
                    return save_token(TKind::LessOrEq);
                }
                unget_char();
                return  save_token(TKind::Less);
            case '>':
                if(get_char() == '='){
                    return save_token(TKind::GreaterOrEq);
                }
                unget_char();
                return  save_token(TKind::Greater);
            case '&':
                if(get_char() == '&'){
                    return save_token(TKind::AmpersandAmp);
                }
                unget_char();
                return  save_token(TKind::Ampersand);
            case '|':
                if(get_char() == '|'){
                    return save_token(TKind::PipePip);
                }
                unget_char();
                return  save_token(TKind::Pipe);
            case '(':
                return save_token(TKind::LParen);
            case ')':
                return save_token(TKind::RParen);
            case '{':
                return save_token(TKind::LBrace);
            case '}':
                return save_token(TKind::RBrace);
            case '=':
                if(get_char() == '='){
                    return save_token(TKind::EqualsEq);
                }
                unget_char();
                return  save_token(TKind::Equals);
            case ';':
                return save_token(TKind::Semicolon);
            case ',':
                return save_token(TKind::Comma);
            case '\'':
                {
                    c = get_char();
                    if (c == '\\') get_char();
                    if (get_char() != '\'') {
                        session_.error(source_id_, {pinpos_, curpos_}, "unterminated char literal");
                    }
                    return save_token(TKind::CharLit);
                }

            case '"':
                {
                    for (;;) {
                        c = get_char();
                        if (c == '"') break;
                        if (c == EOF || c == '\n') {
                            session_.error(source_id_, {pinpos_, curpos_}, "unterminated string literal");
                            break;
                        }
                        if (c == '\\') get_char();
                    }
                    return save_token(TKind::StringLit);
                }
            default:
                if(std::isspace(c))
                    continue;
                if(std::isdigit(c)){
                    for(;;){
                        c = get_char();
                        if(!std::isdigit(c)){
                            unget_char();
                            return save_token(TKind::IntLit);
                        }
                    }
                }
                if(std::isalpha(c)){
                    for(;;){
                        c = get_char();
                        if(!std::isalnum(c)){
                            unget_char();
                            std::string ident_str = get_lexeme();
                            if(ident_str == "int"){
                                return save_token(TKind::KwInt);
                            }else if(ident_str == "if"){
                                return save_token(TKind::KwIf);
                            }else if(ident_str == "else"){
                                return save_token(TKind::KwElse);
                            }else if(ident_str == "while"){
                                return save_token(TKind::KwWhile);
                            }else if(ident_str == "for"){
                                return save_token(TKind::KwFor);
                            }else if(ident_str == "return"){
                                return save_token(TKind::KwReturn);
                            }else if(ident_str == "char"){
                                return save_token(TKind::KwChar);
                            }else if(ident_str == "extrn"){
                                return save_token(TKind::KwExtrn);
                            }
                            return save_token(TKind::Ident);
                        }
                    }
                    
                }
                auto invalid = save_token(TKind::Invalid);
                session_.error(source_id_, invalid.get_loc(), "unexpected character: '" + std::string(1, c) + "'");
                return invalid;
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
    std::string Lexer::get_lexeme(src::SourceLocation loc){
        auto src = session_.sources.get_source(source_id_);
        if(!src) return "Unknown source"; //TODO: refactor
        std::string_view content = src->get_content();
        return std::string(content.substr(loc.start, loc.end-loc.start));
    }
    std::string Lexer::get_lexeme(){
        return get_lexeme(src::SourceLocation{pinpos_, curpos_});
    }

    Token Lexer::save_token(TKind kind){
        return Token(kind, {pinpos_, curpos_});
    }

    Lexer::Snapshot Lexer::save(){
        return {
            stream_->tellg(),
            pinpos_, curpos_,
            rdchar_
        };
    }
    void Lexer::restore(Lexer::Snapshot snap){
        stream_->clear();
        stream_->seekg(snap.stream_pos);
        pinpos_ = snap.pinpos;
        curpos_ = snap.curpos;
        rdchar_ = snap.rdchar;
    }
}
 
