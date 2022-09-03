#pragma once

#include "instance_of.h"
#include "int_util.h"
#include "switch_.h"
#include "union_.h"

namespace tools::detail {

template <typename ... Ts>
struct variant_data {
  using index_type = uint_at_least<sizeof...(Ts)>;

  union_<Ts...> data;
  index_type idx;
};


}  // namespace tools::detail
