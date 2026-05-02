#include <watafac/diag/Diagnostics.hpp>
#include <sstream>
#include <algorithm>
namespace wfac::diag {
    void DiagnosticsEngine::add_record(DiagnosticsEngine::Record rec){
        records_.push_back(std::move(rec));
    }
    void DiagnosticsEngine::print_all(std::ostream &out, src::SourceManager &srcman){
        for(const auto &rec : records_){
            out << rec.format(srcman) << "\n";
        }
    }
    std::string DiagnosticsEngine::to_string(DiagnosticsEngine::SeverityLevel level) {
        switch (level) {
        case SeverityLevel::Info:    return "Info";
        case SeverityLevel::Warning: return "Warning";
        case SeverityLevel::Error:   return "Error";
        default:                     return "Unknown";
        }
    }
    std::string DiagnosticsEngine::Record::format(src::SourceManager &srcman) const{
        std::stringstream ss;
        src::CachedSource *src = srcman.get_source(source_id);
        if(src){
            auto line = src->get_line_number(location.start);
            auto line_offset = src->get_line_offset(line);
            auto column = location.start - line_offset + 1;
            ss << src->get_name() << ':' << line << ':' << column << ":\n";

            ss << "\t" << src->get_line(line) << '\n';
            ss << "\t" << std::string(column-1, ' ') << "^\n";
        }
        ss << DiagnosticsEngine::to_string(level) << ": " << message;
        return ss.str();
    }
    bool DiagnosticsEngine::has_errors() const{
        return std::find_if(records_.begin(), records_.end(),
                            [](const DiagnosticsEngine::Record &r) {
                                return r.level == DiagnosticsEngine::SeverityLevel::Error;
                            }) != records_.end();
    }
}
