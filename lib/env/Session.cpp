#include <watafac/env/Session.hpp>

namespace wfac::env{
    void Session::info(src::SourceId source_id, src::SourceLocation loc, std::string msg){
        diags.add_record({
                diag::DiagnosticsEngine::SeverityLevel::Info, std::move(msg), loc, source_id
        });
    }
    void Session::error(src::SourceId source_id, src::SourceLocation loc, std::string msg){
        diags.add_record({
                diag::DiagnosticsEngine::SeverityLevel::Error, std::move(msg), loc, source_id
        });
    }        
    void Session::warning(src::SourceId source_id, src::SourceLocation loc, std::string msg){
        diags.add_record({
                diag::DiagnosticsEngine::SeverityLevel::Warning, std::move(msg), loc, source_id
        });
    }
}
