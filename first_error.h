#pragma once

#include <array>
#include <algorithm>

namespace tools::detail {

struct out_of_bounds {
  using is_error = void;
};

template <std::size_t n,
          typename T0 = out_of_bounds,
          typename T1 = out_of_bounds,
          typename T2 = out_of_bounds,
          typename T3 = out_of_bounds,
          typename T4 = out_of_bounds,
          typename T5 = out_of_bounds,
          typename T6 = out_of_bounds,
          typename T7 = out_of_bounds,
          typename T8 = out_of_bounds,
          typename T9 = out_of_bounds,
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
using get_type_t = typename decltype(get_type_impl<n, Ts...>())::type;

template <typename T>
concept error_tag_type = requires { typename T::is_error; };

template <typename ...Ts>
constexpr std::size_t first_error_idx() {
  std::array tests { error_tag_type<Ts>... };
  return static_cast<std::size_t>(std::ranges::find(tests, true) - tests.begin());
}

template <typename ...Ts>
using first_error_t = get_type_t<first_error_idx<Ts...>(), Ts...>;

}  // namespace tools::detail
