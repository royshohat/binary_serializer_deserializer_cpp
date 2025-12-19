#include <vector>
#include <concepts>
#include <type_traits>
#include <cstdint>

template <typename T, typename Serializer>
concept Serializable = requires(T a, Serializer& s){ {a.serialize(s)}; };


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

};
