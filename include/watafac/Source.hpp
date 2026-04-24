#ifndef WATAFAC_SOURCE_HPP
#define WATAFAC_SOURCE_HPP
#include <string>
#include <memory>
#include <optional>
namespace wfac {
    class Source {
    public:
        virtual std::unique_ptr<std::istream> get_istream() = 0;
        virtual ~Source() = default;
    };
    class FileSource: public Source{
    public:
        explicit FileSource(std::string filepath);
        std::unique_ptr<std::istream> get_istream() override;
    private:
        static constexpr std::size_t CONTENT_THRESHOLD = 1024 * 1024; //1MB
        void load_content();
        std::string filepath_;
        std::optional<std::string> content_;
    };
}
#endif
