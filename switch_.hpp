#pragma once

#include <cstdint>
#include <utility>

namespace tools {

template <std::size_t V>
struct c_idx {
  static constexpr auto value = V;
};

template <auto V>
constexpr c_idx<V> c_idx_v;

namespace v1 {
// https://godbolt.org/z/fseddGo5b

template <std::size_t size, typename Op>
struct switcher {
  Op op;

  using res_t = decltype(op(c_idx_v<0>));

  constexpr switcher(c_idx<size>, Op op) : op(op) {}

  constexpr res_t operator()(std::size_t i) const
  {
    using case_ = res_t(*)(Op);

    return [&]<std::size_t... idxs>(std::index_sequence<idxs...>) {
      constexpr case_ cases[]{  [](Op op){ return op(c_idx_v<idxs>); }... };

      return cases[i](op);
    }
    (std::make_index_sequence<size>{});
  }
};

} // namespace v1

}  // namespace _tools
