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

namespace v2 {
// https://godbolt.org/z/s5xqd8YGW

template <std::size_t size, typename Op>
struct switcher {
  Op op;

  using res_t = decltype(op(c_idx_v<0>));

  constexpr switcher(c_idx<size>, Op op) : op(op) {}

  template <std::size_t start>
  static constexpr res_t impl(Op _op, std::size_t i) {
    #define TOOLS_CASE(x) if constexpr (size >= start + (x)) if (i == start + (x)) return _op(c_idx_v<start + (x)>);

    #define TOOLS_CASE10(x) \
      TOOLS_CASE(10 * (x) + 0) \
      TOOLS_CASE(10 * (x) + 1) \
      TOOLS_CASE(10 * (x) + 2) \
      TOOLS_CASE(10 * (x) + 3) \
      TOOLS_CASE(10 * (x) + 4) \
      TOOLS_CASE(10 * (x) + 5) \
      TOOLS_CASE(10 * (x) + 6) \
      TOOLS_CASE(10 * (x) + 7) \
      TOOLS_CASE(10 * (x) + 8) \
      TOOLS_CASE(10 * (x) + 9)

    #define TOOLS_CASE100(x) \
      TOOLS_CASE10(10 * (x) + 0) \
      TOOLS_CASE10(10 * (x) + 1) \
      TOOLS_CASE10(10 * (x) + 2) \
      TOOLS_CASE10(10 * (x) + 3) \
      TOOLS_CASE10(10 * (x) + 4) \
      TOOLS_CASE10(10 * (x) + 5) \
      TOOLS_CASE10(10 * (x) + 6) \
      TOOLS_CASE10(10 * (x) + 7) \
      TOOLS_CASE10(10 * (x) + 8) \
      TOOLS_CASE10(10 * (x) + 9)

    TOOLS_CASE100(0)
    TOOLS_CASE100(1)
    TOOLS_CASE100(2)
    TOOLS_CASE100(3)
    TOOLS_CASE100(4)
    TOOLS_CASE100(5)
    TOOLS_CASE100(6)
    TOOLS_CASE100(7)
    TOOLS_CASE100(8)
    TOOLS_CASE100(9)
    if constexpr (size >= start + 1000) return impl<start + 1000>(_op, i);
    else                                __builtin_unreachable();

    #undef TOOLS_CASE100
    #undef TOOLS_CASE10
    #undef TOOLS_CASE
  }

  constexpr res_t operator()(std::size_t i) const
  {
    return impl<0>(op, i);
  }
};

} // namespace v2

namespace v3 {
// ref: https://github.com/groundswellaudio/swl-variant
// https://godbolt.org/z/MGMdz7YvK

template <std::size_t size, typename Op>
struct switcher {
  Op op;

  using res_t = decltype(op(c_idx_v<0>));

  constexpr switcher(c_idx<size>, Op op) : op(op) {}

  template <std::size_t start>
  static constexpr res_t impl(Op _op, std::size_t i) {
    #define TOOLS_SWITCH_CASE(x) case start + x: if constexpr (size > start + x) return _op(c_idx_v<start + x>);
    #define TOOLS_SWITCH_CASE10(x) \
      TOOLS_SWITCH_CASE(10 * (x) + 0) \
      TOOLS_SWITCH_CASE(10 * (x) + 1) \
      TOOLS_SWITCH_CASE(10 * (x) + 2) \
      TOOLS_SWITCH_CASE(10 * (x) + 3) \
      TOOLS_SWITCH_CASE(10 * (x) + 4) \
      TOOLS_SWITCH_CASE(10 * (x) + 5) \
      TOOLS_SWITCH_CASE(10 * (x) + 6) \
      TOOLS_SWITCH_CASE(10 * (x) + 7) \
      TOOLS_SWITCH_CASE(10 * (x) + 8) \
      TOOLS_SWITCH_CASE(10 * (x) + 9)
    #define TOOLS_SWITCH_CASE100(x) \
      TOOLS_SWITCH_CASE10(10 * (x) + 0) \
      TOOLS_SWITCH_CASE10(10 * (x) + 1) \
      TOOLS_SWITCH_CASE10(10 * (x) + 2) \
      TOOLS_SWITCH_CASE10(10 * (x) + 3) \
      TOOLS_SWITCH_CASE10(10 * (x) + 4) \
      TOOLS_SWITCH_CASE10(10 * (x) + 5) \
      TOOLS_SWITCH_CASE10(10 * (x) + 6) \
      TOOLS_SWITCH_CASE10(10 * (x) + 7) \
      TOOLS_SWITCH_CASE10(10 * (x) + 8) \
      TOOLS_SWITCH_CASE10(10 * (x) + 9)

    switch (i) {
      TOOLS_SWITCH_CASE100(0)
      TOOLS_SWITCH_CASE100(1)
      TOOLS_SWITCH_CASE100(2)
      TOOLS_SWITCH_CASE100(3)
      TOOLS_SWITCH_CASE100(4)
      default: if constexpr (size >= start + 500) return impl<start + 500>(_op, i);
               else                               __builtin_unreachable();
    }

    #undef TOOLS_SWITCH_CASE100
    #undef TOOLS_SWITCH_CASE10
    #undef TOOLS_SWITCH_CASE
  }

  constexpr res_t operator()(std::size_t i) const
  {
    return impl<0>(op, i);
  }
};

} // namespace v3

}  // namespace tools
