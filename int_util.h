#pragma once

#include <cstdint>
#include <limits>

namespace tools {
namespace _int_utils {

template <std::size_t max>
constexpr auto uint_at_least_impl() {
       if constexpr ( max <= std::numeric_limits<std::uint8_t>::max()  ) return std::uint8_t{};
  else if constexpr ( max <= std::numeric_limits<std::uint16_t>::max() ) return std::uint16_t{};
  else if constexpr ( max <= std::numeric_limits<std::uint32_t>::max() ) return std::uint32_t{};
  else                                                                   return std::uint64_t{};
}

}  // _int_utils

template <std::size_t max>
using uint_at_least = decltype(_int_utils::uint_at_least_impl<max>());

}  // namespace tools
