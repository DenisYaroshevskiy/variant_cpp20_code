#include "union_storage.h"

#include <vector>

namespace
{
namespace instance_of_test {
static_assert(tools::instance_of<std::vector<int>, std::vector>);
static_assert(!tools::instance_of<int, std::vector>);
static_assert(!tools::instance_of<std::vector<int>&, std::vector>);
} // namespace instance_of_test

namespace union_storage_test {

using us = tools::union_storage<int, char, double>;

static_assert(std::same_as<int const& , decltype(get<0>(std::declval<us const& >()))>);
static_assert(std::same_as<int&,        decltype(get<0>(std::declval<us&       >()))>);
static_assert(std::same_as<int&&,       decltype(get<0>(std::declval<us&&      >()))>);
static_assert(std::same_as<int const&&, decltype(get<0>(std::declval<us const&&>()))>);

static_assert(std::same_as<char const& , decltype(get<1>(std::declval<us const& >()))>);
static_assert(std::same_as<char&,        decltype(get<1>(std::declval<us&       >()))>);
static_assert(std::same_as<char&&,       decltype(get<1>(std::declval<us&&      >()))>);
static_assert(std::same_as<char const&&, decltype(get<1>(std::declval<us const&&>()))>);

static_assert(std::same_as<double const& , decltype(get<2>(std::declval<us const& >()))>);
static_assert(std::same_as<double&,        decltype(get<2>(std::declval<us&       >()))>);
static_assert(std::same_as<double&&,       decltype(get<2>(std::declval<us&&      >()))>);
static_assert(std::same_as<double const&&, decltype(get<2>(std::declval<us const&&>()))>);


}  // namespace union_storage_test

} // namespace


int main() {}
