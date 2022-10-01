#pragma once

#include <cstdint>

namespace tools {

// https://github.com/willwray

template <std::size_t n,
          typename T0 = void,
          typename T1 = void,
          typename T2 = void,
          typename T3 = void,
          typename T4 = void,
          typename T5 = void,
          typename T6 = void,
          typename T7 = void,
          typename T8 = void,
          typename T9 = void,
          typename ... Ts>
constexpr auto get_type_impl() {
  /**/ if constexpr ( n == 0 ) return std::type_identity<T0>{};
  else if constexpr ( n == 1 ) return std::type_identity<T1>{};
  else if constexpr ( n == 2 ) return std::type_identity<T2>{};
  else if constexpr ( n == 3 ) return std::type_identity<T3>{};
  else if constexpr ( n == 4 ) return std::type_identity<T4>{};
  else if constexpr ( n == 5 ) return std::type_identity<T5>{};
  else if constexpr ( n == 6 ) return std::type_identity<T6>{};
  else if constexpr ( n == 7 ) return std::type_identity<T7>{};
  else if constexpr ( n == 8 ) return std::type_identity<T8>{};
  else if constexpr ( n == 9 ) return std::type_identity<T9>{};
  else                         return get_type_impl<n - 10, Ts...>();
}

template <std::size_t n, typename ...Ts>
  requires (n < sizeof ...(Ts))
using get_type_t = typename decltype(get_type_impl<n, Ts...>())::type;

}  // namespace tools
