#pragma once

#include "error_tag_support.h"

namespace tools {

template <typename ...Ts>
struct no_common_type {
  using is_error = void;
};

template <typename ...Ts>
auto common_type_impl() {
  /**/ if constexpr (( error_tag_type<Ts> || ... ) ) return std::type_identity<first_error_t<Ts...>>{};
  else if constexpr ( requires { typename std::common_type_t<Ts...>; } ) return std::common_type<Ts...>{};
  else return std::type_identity<no_common_type<Ts...>>{};
}

template <typename ...Ts>
using common_type_t = typename decltype(common_type_impl<Ts...>())::type;

}  // namespace tools
