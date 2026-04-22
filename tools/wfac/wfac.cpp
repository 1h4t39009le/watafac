#include <iostream>
#include <memory>
#include <fstream>
#include <watafac/Source.hpp>

int main(){
    auto shrsrc = std::make_shared<wfac::FileSource>("a.txt");
    if(auto stream = shrsrc->get_istream()){
        std::cout << "File found!\n";
    }
    std::cout << "Hello, World!\n";
}
