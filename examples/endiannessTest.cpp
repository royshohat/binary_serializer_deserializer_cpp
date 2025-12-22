#include <iostream>
#include <cstdint>
#include "../include/endianness.hpp"

template <typename T>
void printBytes(T val);




int main(){
    int val = 0x12345678;
    printBytes(val);
    val = swap_endian(val);
    std::cout << std::endl;
    std::cout << std::endl;
    printBytes(val);
    val = swap_endian(val);
    std::cout << std::endl;
    std::cout << std::endl;
    printBytes(val);
    
}



