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

        //for non-fundamental types
        //doing that cause struct/class have hidden padding for cpu perposes
        //but this data shell not be stored for security/efficency reasons
        
        template <typename T>
        requires Serializable<T, Serializer>
        void write(T& value){ value.serialize(*this); }

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


        void load_from_file(const std::string& fileName);
        
    };


}