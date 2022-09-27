#pragma once

#include "c_array_math.h"
#include "instance_of.h"
#include "int_util.h"
#include "switch_.h"
#include "union_.h"
#include "visit_result.h"


namespace tools::detail {

template <typename ... Ts>
struct variant_data {
  using index_type = uint_at_least<sizeof...(Ts)>;
  static constexpr std::size_t size = sizeof...(Ts);

  union_<Ts...> data;
  index_type idx;
};

template <typename Vis, typename ...Vs>
auto visit(Vis&& vis, Vs&& ... vs)
  noexcept ( noexcept_visit_v<Vis, Vs...> ) -> visit_result_t<Vis, Vs...>
  requires (!error_tag_type<visit_result_t<Vis, Vs...>>) {
  using math = c_array_math<std::remove_cvref_t<Vs>::size...>;
  using res = visit_result_t<Vis, Vs...>;

  return v3::switcher(c_idx_v<(std::remove_cvref_t<Vs>::size * ...)>,
    [&]<std::size_t case_>(c_idx<case_>) {
      using seq = typename math::template multi_idx_sequence<case_>;

      return [&]<std::size_t ... is>(std::index_sequence<is...>) -> res {
        return TOOLS_FWD(vis)(get<is>(TOOLS_FWD(vs).data) ...);
      }(seq{});
    }
  );
}


}  // namespace tools::detail
