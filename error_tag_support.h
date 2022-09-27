#pragma once

#include <array>
#include <algorithm>

#include "get_type.h"

namespace tools {

template <typename T>
concept error_tag_type = requires { typename T::is_error; };

template <typename ...Ts>
concept any_errors = ( error_tag_type<Ts> || ... );

template <typename ...Ts>
constexpr std::size_t first_error_idx() {
  std::array tests { error_tag_type<Ts>... };
  return static_cast<std::size_t>(std::ranges::find(tests, true) - tests.begin());
}

template <typename ...Ts>
using first_error_t = get_type_t<first_error_idx<Ts...>(), Ts...>;

}  // namespace tools
