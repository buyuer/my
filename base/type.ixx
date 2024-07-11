module;
#include <complex>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
export module my.base:Type;

export namespace myt::base {

using size_type = std::size_t;
using sint8 = std::int8_t;
using uint8 = std::uint8_t;
using sint16 = std::int16_t;
using uint16 = std::uint16_t;
using sint32 = std::int32_t;
using uint32 = std::uint32_t;
using sint64 = std::int64_t;
using uint64 = std::uint64_t;
using f8 = std::uint8_t;
using f16 = std::uint16_t;
using f32 = float;
using f64 = double;
using cpx8 = std::complex<f8>;
using cpx16 = std::complex<f16>;
using cpx32 = std::complex<f32>;
using cpx64 = std::complex<f64>;

class Type {
public:
    enum ID : uint8 {
        NONE = 0,
        SINT8 = 1,
        UINT8 = 2,
        SINT16 = 3,
        UINT16 = 4,
        SINT32 = 5,
        UINT32 = 6,
        SINT64 = 7,
        UINT64 = 8,
        FLOAT8 = 9,
        FLOAT16 = 10,
        FLOAT32 = 11,
        FLOAT64 = 12,
        COMPLEX8 = 13,
        COMPLEX16 = 14,
        COMPLEX32 = 15,
        COMPLEX64 = 16,
        SUM = 17,
    };

    Type() = default;

    template <typename T>
        requires std::integral<T> or std::same_as<T, ID>
    explicit Type(T id)
        : id_(id)
    {
        if (not(id > NONE and id < SUM)) {
            id = NONE;
        }
    }

    size_type size() const { return info_table[id_].size; }

    const std::string_view& name() { return info_table[id_].name; }

private:
    ID id_ {};
    struct TypeInfo {
        size_type size;
        const std::string_view name {};
    };
    static constexpr TypeInfo info_table[] {
        { 0, "none" },
        { sizeof(sint8), "sint8" },
        { sizeof(uint8), "uint8" },
        { sizeof(sint16), "sint16" },
        { sizeof(uint16), "uint16" },
        { sizeof(sint32), "sint32" },
        { sizeof(uint32), "uint32" },
        { sizeof(sint64), "sint64" },
        { sizeof(uint64), "uint64" },
        { sizeof(f8), "float8" },
        { sizeof(f16), "float16" },
        { sizeof(f32), "float32" },
        { sizeof(f64), "float64" },
        { sizeof(cpx8), "complex8" },
        { sizeof(cpx32), "complex16" },
        { sizeof(cpx32), "complex32" },
        { sizeof(cpx64), "complex64" },
    };
};

} // my::namespace
