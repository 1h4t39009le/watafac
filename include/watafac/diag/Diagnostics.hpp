#ifndef WATAFAC_DIAG_DIAGNOSTICS_HPP
#define WATAFAC_DIAG_DIAGNOSTICS_HPP
#include <watafac/src/Source.hpp>
#include <string>
#include <vector>
namespace wfac::diag {
    namespace src = wfac::src;
    class DiagnosticsEngine {
    public:
        enum class SeverityLevel {
            Info,
            Warning,
            Error
        };
        static std::string to_string(SeverityLevel level);
        struct Record {
            SeverityLevel level;
            std::string message;
            src::SourceLocation location;
            src::SourceId source_id;

            std::string format(src::SourceManager &srcman) const;
        };
        void add_record(Record rec);
        void print_all(std::ostream &out,src::SourceManager &srcman);
        bool has_errors() const;
    private:
        std::vector<Record> records_;
    };
}
#endif
