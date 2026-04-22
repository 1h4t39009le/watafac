#include <watafac/Source.hpp>
#include <istream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace wfac {

    FileSource::FileSource(std::string filepath)
    : filepath_{std::move(filepath)} {
        std::error_code ec;
        auto size = std::filesystem::file_size(filepath, ec);
        if(!ec && size <= CONTENT_THRESHOLD){
            load_content();
        }
    }

    std::unique_ptr<std::istream> FileSource::get_istream() {
        if(content_){
            return std::make_unique<std::istringstream>(*content_);
        }
        auto stream = std::make_unique<std::ifstream>(filepath_);
        if (!stream->is_open()) {
            return nullptr;
        }
        return stream;
    }

    void FileSource::load_content() {
        std::ifstream file(filepath_);
        content_ = std::string(std::istreambuf_iterator<char>(file), {});
    }

} // namespace wfac
