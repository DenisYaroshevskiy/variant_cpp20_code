#pragma once

namespace tools::detail {

template <typename ...Ts>
struct no_common_type {
  no_common_type(auto&&...) {}
  using is_error = void;
};

template <typename ...Ts>
struct common_type {
  using type = no_common_type<Ts...>;
};

template <typename ...Ts>
requires requires { typename std::common_type_t<Ts...>; }
struct common_type<Ts...> {
  using type = std::common_type_t<Ts...>;
};

template <typename ...Ts>
using common_type_t = typename common_type<Ts...>::type;


}  // namespace tools::detail
