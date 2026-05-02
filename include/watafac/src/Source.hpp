#ifndef WATAFAC_SRC_SOURCE_HPP
#define WATAFAC_SRC_SOURCE_HPP
#include <string>
#include <memory>
#include <optional>
#include <istream>
#include <unordered_map>
#include <string_view>
#include <vector>
#include <algorithm>
namespace wfac::src{
    class Source {
    public:
        virtual std::unique_ptr<std::istream> get_istream() = 0;
        virtual std::string get_name() const = 0;
        virtual ~Source() = default;
    };
    struct SourceLocation{
        std::size_t start, end;
    };
    class FileSource: public Source{
    public:
        explicit FileSource(std::string filepath);
        std::unique_ptr<std::istream> get_istream() override;
        std::string get_name() const override;
    private:
        std::string filepath_;
    };
    class CachedSource: public Source {
    public:
        template<typename Src, typename = std::enable_if_t<!std::is_same_v<std::decay_t<Src>, CachedSource>>>
        explicit CachedSource(Src &&source) : name_{source.get_name()}{
            auto is = source.get_istream();
            content_.assign(std::istreambuf_iterator<char>(*is),
                            std::istreambuf_iterator<char>());
            std::string_view view = content_;
            for(auto it=view.begin(); it!=view.end();){
                line_offsets_.push_back(std::distance(view.begin(), it));
                it = std::find(it, view.end(), '\n');
                if(it != view.end()) ++it;
            }
        }
        std::unique_ptr<std::istream> get_istream() override;
        std::string get_name()const override;
        std::size_t get_line_offset(std::size_t line) const;
        std::size_t get_line_number(std::size_t offset) const;
        std::string_view get_line(std::size_t line) const;
        std::string_view get_content() const;
    private:
        std::string content_;
        std::string name_;
        std::vector<std::size_t> line_offsets_;
    };
    using SourceId = size_t;
    class SourceManager{
    public:
        template<typename Src>
        SourceId add_source(Src &&source){
            SourceId id = nextid_++;
            sources_.emplace(id, std::forward<Src>(source));
            return id;
        }
        CachedSource *get_source(SourceId id);
    private:
        SourceId nextid_{};
        std::unordered_map<SourceId, CachedSource> sources_;
    };
}
#endif
