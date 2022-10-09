#pragma once

#include "instance_of.h"
#include "fwd.h"
#include <utility>

namespace tools
{

  template <typename T, typename... Ts>
  struct union_ {
    union {
      T head;
      union_<Ts...> tail;
    };
  };

  template <typename T>
  struct union_<T> { T head; };

  template <typename>
  struct union_size;

  template <typename... Ts> struct union_size<union_<Ts...>>
    : std::integral_constant<std::size_t, sizeof...(Ts)>
  {};

  template <typename T>
  constexpr std::size_t union_size_v = union_size<T>::value;

  template <std::size_t i, typename Self>
  auto&& get(Self&& self)
    requires instance_of<std::remove_cvref_t<Self>, union_> &&
            (i < union_size_v<std::remove_cvref_t<Self>>)
  {
    if constexpr (i == 0) return TOOLS_FWD(self).head;
    else                  return get<i - 1>(TOOLS_FWD(self).tail);
  }

  template <std::size_t i, typename ...Ts>
  constexpr void construct_at(union_<Ts...>& self, auto&&...args)
    requires requires { get<i>(self); } {
    std::construct_at(&get<i>(self), TOOLS_FWD(args)...);
  }

  template <std::size_t i, typename ...Ts>
  constexpr void destroy_at(union_<Ts...>& self)
    requires requires { get<i>(self); } {
    std::destroy_at(&get<i>(self));
  }


} // namespace tools
