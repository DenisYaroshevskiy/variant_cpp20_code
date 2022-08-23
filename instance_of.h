#include <type_traits>

#pragma once

namespace tools {
namespace _instance_of {

template <template <typename ...> class, typename>
struct impl : std::false_type {};

template <template <typename ...> class What, typename ... Ts>
struct impl<What, What<Ts...>> : std::true_type {};

} // namespace _instance_of

template <typename T, template <typename ...> class What>
concept instance_of = _instance_of::impl<What, T>::value;

}  // namespace tools
