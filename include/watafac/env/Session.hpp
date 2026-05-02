#ifndef WATAFAC_ENV_SESSION_HPP
#define WATAFAC_ENV_SESSION_HPP
#include <watafac/src/Source.hpp>
#include <watafac/diag/Diagnostics.hpp>
namespace wfac::env{
    namespace diag = wfac::diag;
    namespace src = wfac::src;
    struct Session {
        Session(const Session&) = delete;
        Session& operator=(const Session&) = delete;
        Session() = default;

        void info(src::SourceId source_id, src::SourceLocation loc, std::string msg);
        void error(src::SourceId source_id, src::SourceLocation loc, std::string msg);
        void warning(src::SourceId source_id, src::SourceLocation loc, std::string msg);
        
        src::SourceManager sources;
        diag::DiagnosticsEngine diags;
    };
}
#endif
