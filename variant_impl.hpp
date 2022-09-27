#pragma once

#include "c_array_math.h"
#include "instance_of.h"
#include "int_util.h"
#include "switch_.h"
#include "union_.h"
#include "one_result.h"


namespace tools::details {

template <typename ... Ts>
struct variant_data {
  using index_type = uint_at_least<sizeof...(Ts)>;
  static constexpr std::size_t size = sizeof...(Ts);

  union_<Ts...> data;
  index_type idx;
};

}  // namespace tools::details
