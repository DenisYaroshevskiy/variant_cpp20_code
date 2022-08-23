#pragma once

namespace tools {

template <typename T, typename ...Ts>
struct union_storage
{
  union {
    T head;
    union_storage<Ts...> tail;
  };
};

template <typename T>
struct union_storage<T>
{
  T head;
};

template <std::size_t i, typename ...Ts>
auto&& get(auto&& self)

}  // namespace tools
