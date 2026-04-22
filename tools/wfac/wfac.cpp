#include <iostream>
#include <memory>
#include <fstream>
namespace wfac {
    class Source {
    public:
        virtual std::unique_ptr<std::istream> get_istream() = 0;
        virtual ~Source() = default;
    };
    class FileSource: public Source{
    public:
        explicit FileSource(std::string filepath)
        : filepath_{std::move(filepath)} {}
        
        std::unique_ptr<std::istream> get_istream() override {
            auto stream = std::make_unique<std::ifstream>(filepath_);
            if(!stream->is_open()) return nullptr;
            return stream;
        }
    private:
        std::string filepath_;
    };
}

int main(){
    auto src = std::make_shared<wfac::FileSource>("a.txt");
    
    std::cout << "Hello, World!\n";
}
