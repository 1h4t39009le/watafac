#include <watafac/src/Source.hpp>
#include <istream>
#include <fstream>
#include <sstream>
#include <algorithm>
namespace wfac::src {

    FileSource::FileSource(std::string filepath)
    : filepath_{std::move(filepath)} {}

    std::unique_ptr<std::istream> FileSource::get_istream() {
        auto stream = std::make_unique<std::ifstream>(filepath_, std::ios::binary);
        if (!stream->is_open()) {
            return nullptr;
        }
        return stream;
    }
    std::string FileSource::get_name() const{
        return filepath_;
    }
    
    std::unique_ptr<std::istream> CachedSource::get_istream(){
        return std::make_unique<std::istringstream>(content_);
    }

    std::string CachedSource::get_name() const{
        return name_;
    }

    CachedSource *SourceManager::get_source(SourceId id) {
        auto it = sources_.find(id);
        if(it == sources_.end()) return nullptr;
        return &it->second;
    }
    std::size_t CachedSource::get_line_offset(std::size_t line) const{
        if(line == 0 || line > line_offsets_.size()) return 0;
        return line_offsets_[line-1];
    }
    std::size_t CachedSource::get_line_number(std::size_t offset) const{
        auto it = std::upper_bound(line_offsets_.begin(), line_offsets_.end(), offset);
        return std::distance(line_offsets_.begin(), it);
    }
    std::string_view CachedSource::get_content() const{
        return content_;
    }
    std::string_view CachedSource::get_line(std::size_t line) const {
        if(line == 0 || line > line_offsets_.size()) return {};
        std::size_t start = line_offsets_[line-1],
            end = ((line+1) < line_offsets_.size()) ? line_offsets_[line] - 1 : content_.size();
        return get_content().substr(start, end - start);
        
    }
    
} // namespace wfac
