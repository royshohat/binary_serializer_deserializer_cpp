#pragma once

#include <vector>
#include <concepts>
#include <type_traits>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cstring>

// mechanism that will insure that T has a method serialize that takes Serializer&  
template <typename T, typename Serializer>
concept Serializable = requires(T a, Serializer& s){ {a.serialize(s)}; };

template <typename T, typename Deserializer>
concept Deserializable = requires(T a, Deserializer& s){ {a.deserialize(s)}; };


namespace Serialization{

    class Serializer{
        std::vector<uint8_t> buffer;

    public:
        // for fundamental types
        template <typename T>
        requires std::is_fundamental_v<T>
        void write(const T& value) {
            const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&value);
            // .insret(range) already reserves the amount it needs so there is no need to hit .reserve() here
            buffer.insert(buffer.end(), ptr, ptr+sizeof(T));
        }

        //for struct/class types
        //doing that cause struct/class have hidden padding for cpu perposes
        //but this data shell not be stored for security/efficency reasons
        
        template <typename T>
        requires Serializable<T, Serializer>
        void write(T& value){ value.serialize(*this); }

        //for pointer types
        template <typename T>
        requires std::is_pointer_v<T>
        void write_Ptr(T ptr, size_t count){
            // writing the size first so the desrializer knows how much to read
            uint32_t ucount = static_cast<uint32_t>(count);
            write(ucount);

            using baseType = std::remove_pointer_t<T>;
            if(std::is_fundamental_v<baseType>){
                uint8_t* rawData = reinterpret_cast<uint8_t*>(ptr);
                buffer.insert(buffer.end(), rawData, rawData+(count*sizeof(baseType)));
            }
            else{
                for(int i=0; i<count; ++i){
                    write(ptr[i]);
                }
            }
        }

        // now save is file is in here based on oop, but may for more flexable use i will move it to be a non member
        void save_to_file(const std::string&);

    };

    class Deserializer{
        std::vector<uint8_t> buffer;
        uint32_t offset = 0;

    public:
        // for fundamental types
        template <typename T>
        requires std::is_fundamental_v<T>
        void read(T& value){
            if(offset + sizeof(T) > buffer.size()) throw std::runtime_error("Overflow, what a shame :(");

            std::memcpy(&value, &buffer[offset], sizeof(T));
            offset += sizeof(T);
        }

        template <typename T>
        requires Deserializable<T, Deserializer>
        void read(T& value){
            value.deserialize(*this);
        }

        template <typename T>
        requires std::is_pointer_v<T>
        // this function will also allocate space for the data
        void read_ptr(T& ptr, size_t& count){
            uint32_t countRef;
            read(countRef);
            count = countRef;

            using baseType = std::remove_pointer_t<T>;

            ptr = new baseType[count];

            if constexpr (std::is_fundamental_v<baseType>) {
                    // fast path: Block copy
                    size_t totalBytes = count * sizeof(baseType);
                    if (offset + totalBytes > buffer.size()) throw std::runtime_error("Overflow boho");
        
                    std::memcpy(ptr, &buffer[offset], totalBytes);
                    offset += totalBytes;
            }
            else {
                // slow path: Deserialize into each new slot
                for (size_t i = 0; i < count; ++i) {
                    read(ptr[i]);
                }
            }

        }


        void load_from_file(const std::string& fileName);
        
    };


}