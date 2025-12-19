#include "../include/Serializer.hpp"

void Serializer::save_to_file(const std::string& fileName){
    std::ofstream outFile(fileName, std::ios::binary);
    if(!outFile){
        std::cerr << "could not write to file" << std::endl;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}