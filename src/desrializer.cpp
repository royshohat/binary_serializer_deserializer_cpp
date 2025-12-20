#include "../include/serialization.hpp"

void Serialization::Deserializer::load_from_file(const std::string& fileName){
    // std::ios::ate puting the pointer in the end of the file
    // doing that to check the size the of data
    // with the size of the data we can resize the vector
    // instead of realocting memory multiple times
    std::ifstream inFile(fileName, std::ios::binary | std::ios::ate);
    if (!inFile) std::cerr << "error in loading data! " << std::endl; 

    std::streamsize size = inFile.tellg();
    // moving the file pointer back to the start after we checked the file size
    inFile.seekg(0, std::ios::beg);

    buffer.resize(size);
    inFile.read(reinterpret_cast<char*>(buffer.data()), size);
    offset=0;
}