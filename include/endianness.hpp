#pragma once
#include <bit>
#include <cstdint>

constexpr bool IS_BIG_ENDIAN = (std::endian::native == std::endian::big);

template <typename T>
T swap_endian(T val){
    uint8_t* valPtr = reinterpret_cast<uint8_t*>(&val);
    uint8_t temp; 
    for(int i=0; i<sizeof(T)/2; ++i){
        temp = valPtr[i];
        valPtr[i] = valPtr[sizeof(T)-1-i];
        valPtr[sizeof(T)-1-i] = temp;
    }
    return val;
}

template <typename T>
void printBytes(T val){
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&val);
    auto flags = std::cout.flags();
    std::cout << std::hex;
    for(int i=0; i<sizeof(T); ++i){
       std::cout << "0x" << (int)ptr[i] << ' ';
    }
    std::cout.flags(flags);
}