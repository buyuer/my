/**
 * @author  dingddding@163.com
 * @date    2024.11.02
 * @brief   common struct
 */

module;
#include <complex>
#include <cstddef>
#include <cstdint>
export module myt.common;

export namespace myt {

using sizeT  = std::size_t;
using byteT  = std::uint8_t;
using sint8  = std::int8_t;
using uint8  = std::uint8_t;
using sint16 = std::int16_t;
using uint16 = std::uint16_t;
using sint32 = std::int32_t;
using uint32 = std::uint32_t;
using sint64 = std::int64_t;
using uint64 = std::uint64_t;
using f8     = std::uint8_t;
using f16    = std::uint16_t;
using f32    = float;
using f64    = double;
using cpx8   = std::complex<f8>;
using cpx16  = std::complex<f16>;
using cpx32  = std::complex<f32>;
using cpx64  = std::complex<f64>;

} // namespace myt