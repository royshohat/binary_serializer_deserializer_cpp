#include "../include/serialization.hpp"

void Serialization::Serializer::save_to_file(const std::string& fileName){
    
    std::ofstream outFile(fileName, std::ios::binary);
    if(!outFile){
        std::cerr << "could not write to file" << std::endl;
        return; }
    // buffer is a raper so to get the actual pointer to the data .data()
    // our buffer is of type uint8_t but .write accepts only char* so reinterpret cast is necessary
    outFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}