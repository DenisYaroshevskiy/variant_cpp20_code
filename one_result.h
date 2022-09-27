#pragma once

#include "c_array_math.h"

#include <concepts>

namespace tools::details {

template <typename ...Ts>
struct not_invocable {
  using is_error = void;
};

template <typename Vis, typename ...Ts>
struct one_case_result_type {
  using type = not_invocable<Vis, Ts...>;
  static constexpr bool is_noexcept = false;
};

template <typename Vis, typename ...Ts>
  requires std::invocable<Vis, Ts...>
struct one_case_result_type<Vis, Ts...>   {
  using type = std::invoke_result_t<Vis, Ts...>;
  static constexpr bool is_noexcept =
    noexcept(std::declval<Vis>()(std::declval<Ts>()...));
};

template <std::size_t case_, typename Vis, typename ...Vs>
auto one_case_result_impl(c_idx<case_>, Vis&& vis, Vs&& ... vs) {
  using math = c_array_math<std::remove_cvref_t<Vs>::size...>;
  using seq = typename math::template multi_idx_sequence<case_>;

  return [&]<std::size_t ... is>(std::index_sequence<is...>) {
    return one_case_result_type<Vis, decltype(get<is>(TOOLS_FWD(vs).data)) ...>{};
  }(seq{});
};

template <std::size_t case_, typename Vis, typename ...Vs>
using one_case_res_t = typename decltype(one_case_result_impl(c_idx<case_>{}, std::declval<Vis>(), std::declval<Vs>()...))::type;

template <std::size_t case_, typename Vis, typename ...Vs>
constexpr bool one_case_noexcept_v = decltype(one_case_result_impl(c_idx<case_>{}, std::declval<Vis>(), std::declval<Vs>()...))::is_noexcept;

}  // namespace tools::details
