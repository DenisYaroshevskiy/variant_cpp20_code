#pragma once

#include <cstdint>

namespace tools {

template <auto V>
struct constant {
  static constexpr auto value = V;
};

namespace v1 {

template<std::size_t size, typename Op>
void switch_(std::size_t i, Op op) {
  using case_ = (void)(*)(Op);
  [&]<std::size_t ...idxs>(std::index_sequence<idxs...>) {
    static constexpr case_ cases[] {
      [](Op op) { op(constant<idxs>); }...
    };
    cases[i](op);
  }(std::make_index_sequence<size>{});
}

} // namespace v1

}  // namespace _tools
