#pragma once

#include "instance_of.h"
#include "fwd.h"
#include <utility>

namespace tools {

template <typename T, typename ...Ts>
struct union_storage {
  union {
    T head;
    union_storage<Ts...> tail;
  };
};

template <typename T>
struct union_storage<T> {
  T head;
};

template <typename >
struct union_storage_size;

template <typename ... Ts>
struct union_storage_size<union_storage<Ts...>> :
   std::integral_constant<std::size_t, sizeof...(Ts)> {
};

template <typename T>
constexpr std::size_t union_storage_size_v = union_storage_size<T>::value;

template <std::size_t i, typename US>
  requires instance_of<std::remove_cvref_t<US>, union_storage> &&
           (i < union_storage_size_v<std::remove_cvref_t<US>>)
auto&& get(US&& self) {
  if constexpr (i == 0) return TOOLS_FWD(self).head;
  else                  return get<i - 1>(TOOLS_FWD(self).tail);
}

}  // namespace tools
