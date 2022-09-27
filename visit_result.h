#pragma once

#include "common_type.h"
#include "one_result.h"

namespace tools::detail
{

template <typename Vis, typename... Vs, std::size_t... i>
auto visit_result_impl(std::index_sequence<i...>) {
  return std::type_identity<
      tools::common_type_t<one_case_res_t<i, Vis, Vs...> ...>>{};
}

template <typename Vis, typename... Vs>
using visit_result_t = typename decltype(visit_result_impl<Vis, Vs...>(
    std::make_index_sequence<(std::remove_cvref_t<Vs>::size * ... )>{})
  )::type;

} // namespace tools::detail