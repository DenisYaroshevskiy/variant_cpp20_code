#pragma once

#include <array>
#include <cstdint>
#include <numeric>

namespace tools {

template <std::size_t ... dims>
struct c_array_math {
  using mutli_idx = std::array<std::size_t, sizeof...(dims)>;

  static constexpr mutli_idx pows = [] {
    std::array dims_a = {dims...};
    std::array res = dims_a;
    for (std::size_t running = 1, i = dims_a.size(); i > 0; --i) {
      res[i - 1] = running;
      running *= dims_a[i - 1];
    }
    return res;
  }();

  static constexpr mutli_idx to_multi_idx(std::size_t idx) {
      auto res = pows;
      for (std::size_t i = 0; i != pows.size(); ++i) {
        res[i] = idx / pows[i];
        idx %= pows[i];
      }
      return res;
  }

  static constexpr std::size_t to_linear_idx(mutli_idx idx) {
    return std::inner_product(pows.begin(), pows.end(), idx.begin(), 0u);
  }

  template <std::size_t idx>
  static constexpr auto to_mutli_idx_sequence() {
    return []<std::size_t ...is>(std::index_sequence<is...>) {
      constexpr auto arr = to_multi_idx(idx);
      return std::index_sequence<arr[is]...>{};
    }(std::make_index_sequence<sizeof...(dims)>{});
  }

  template <std::size_t i>
  using multi_idx_sequence = decltype(to_mutli_idx_sequence<i>());
};

}  // namespace tools
