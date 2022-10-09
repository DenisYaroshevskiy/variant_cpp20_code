#include "c_array_math.h"
#include "common_type.h"
#include "int_util.h"
#include "switch_.h"
#include "union_.h"
#include "error_tag_support.h"
#include "variant_impl.hpp"
#include "visit_result.h"

#include <exception>
#include <stdexcept>
#include <vector>

namespace
{
namespace instance_of_test {
static_assert(tools::instance_of<std::vector<int>, std::vector>);
static_assert(!tools::instance_of<int, std::vector>);
static_assert(!tools::instance_of<std::vector<int>&, std::vector>);
} // namespace instance_of_test

namespace union_test {

using us = tools::union_<int, char, double>;

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

}  // namespace union_test

namespace switch_test {

template <std::size_t a, std::size_t b>
concept test = a == b;

constexpr auto identity = []<std::size_t i>(tools::c_idx<i>) mutable { return i; };
constexpr auto v1_identity = tools::v1::switcher{tools::c_idx_v<10>, identity};

static_assert(2u == v1_identity(2u));
static_assert(1u == v1_identity(1u));

constexpr auto v2_identity = tools::v2::switcher{tools::c_idx_v<2000>, identity};
static_assert(test<4u, v2_identity(4u)>);
static_assert(test<870u, v2_identity(870u)>);
static_assert(999u == v2_identity(999u));
static_assert(1018u == v2_identity(1018u));
static_assert(1u    == v2_identity(1u));

constexpr auto v3_identity = tools::v3::switcher{tools::c_idx_v<2000>, identity};
static_assert(999u  == v3_identity(999u));
static_assert(1u    == v3_identity(1u));
static_assert(1500u == v3_identity(1500u));

void switch_runtime_test() {
  for (auto i = 0u; i != 2000u; ++i) {
   if(v2_identity(i) != i) throw std::runtime_error("v2");
   if(v3_identity(i) != i) throw std::runtime_error("v3");
  }
}

}  // namespace switch_test

namespace int_util_test {

static_assert(std::same_as<std::uint8_t,  tools::uint_at_least<100>>);
static_assert(std::same_as<std::uint8_t,  tools::uint_at_least<255>>);
static_assert(std::same_as<std::uint16_t, tools::uint_at_least<256>>);
static_assert(std::same_as<std::uint32_t, tools::uint_at_least<std::numeric_limits<std::uint16_t>::max() + 1u>>);
static_assert(std::same_as<std::uint64_t, tools::uint_at_least<5'000'000'000>>);

} // namespace int_util_test

namespace c_array_math_test {

constexpr bool test() {
  using math = tools::c_array_math<3, 4, 2>;
  int arr[3][4][2] = {};

  std::size_t linear = 0u;

  for (std::size_t i = 0; i != 3; ++i) {
    for (std::size_t j = 0; j != 4; ++j) {
      for (std::size_t k = 0; k != 2; ++k) {
        auto multi = std::array{i, j, k};
        if (math::to_multi_idx(linear) != multi) return false;
        if (math::to_linear_idx(multi) != linear) return false;
        ++linear;
      }
    }
  }

  return true;
}

static_assert(test());

}  // namespace c_array_math_test

namespace get_type_test {

using namespace tools;

template <std::size_t i>
struct ith {};

template <std::size_t i>
using call_get_t =
  get_type_t<i,
    ith<0>,  ith<1>,  ith<2>,  ith<3>,  ith<4>,
    ith<5>,  ith<6>,  ith<7>,  ith<8>,  ith<9>,
    ith<10>, ith<11>, ith<12>, ith<13>, ith<14>>;

template <std::size_t i>
concept can_call_get = requires {
  { std::declval<call_get_t<i>>() };
};

template <typename T, typename ...Ts>
concept can_find = requires {
  { find_type_idx<T, Ts...> };
};

static_assert(std::same_as<call_get_t<0>, ith<0>>);
static_assert(std::same_as<call_get_t<1>, ith<1>>);
static_assert(std::same_as<call_get_t<2>, ith<2>>);
static_assert(std::same_as<call_get_t<3>, ith<3>>);
static_assert(std::same_as<call_get_t<4>, ith<4>>);

static_assert(std::same_as<call_get_t<5>, ith<5>>);
static_assert(std::same_as<call_get_t<6>, ith<6>>);
static_assert(std::same_as<call_get_t<7>, ith<7>>);
static_assert(std::same_as<call_get_t<8>, ith<8>>);
static_assert(std::same_as<call_get_t<9>, ith<9>>);

static_assert(std::same_as<call_get_t<10>, ith<10>>);
static_assert(std::same_as<call_get_t<11>, ith<11>>);
static_assert(std::same_as<call_get_t<12>, ith<12>>);
static_assert(std::same_as<call_get_t<13>, ith<13>>);
static_assert(std::same_as<call_get_t<14>, ith<14>>);

static_assert(find_type_idx<ith<2>, ith<0>, ith<1>, ith<2>> == 2);
static_assert(find_type_idx<ith<1>, ith<0>, ith<1>, ith<2>> == 1);
static_assert(can_find<int, char, int, double>);
static_assert(!can_find<int, char, double>);


static_assert(can_call_get<11>);
static_assert(!can_call_get<15>);
static_assert(!can_call_get<16>);

static_assert(
  std::same_as<
    get_type_t<0, int, char, double>,
    int
  >
);


static_assert(
  std::same_as<
    get_type_t<1, int, char, double>,
    char
  >
);

static_assert(
  std::same_as<
    get_type_t<2, int, char, double>,
    double
  >
);


}  // namespace get_type_test

namespace first_error_test {

using namespace tools;
using namespace tools::detail;

struct err_t {
  using is_error = void;
};

static_assert(std::same_as<first_error_t<err_t>, err_t>);
static_assert(std::same_as<first_error_t<int, err_t>, err_t>);
static_assert(std::same_as<first_error_t<int, int, err_t>, err_t>);
static_assert(std::same_as<first_error_t<int, err_t, int, err_t>, err_t>);

static_assert(std::same_as<
  first_error_t<not_invocable<int, int>>, not_invocable<int, int>>);

} // namespace first_error_test

namespace common_type_test {

using namespace tools;
using namespace tools::detail;

static_assert(std::same_as<common_type_t<>, no_common_type<>>);
static_assert(std::same_as<common_type_t<int, double>, double>);
static_assert(std::same_as<common_type_t<double, int, int>, double>);
static_assert(std::same_as<common_type_t<std::string, int, int>,
              no_common_type<std::string, int, int>>);

static_assert(std::same_as<common_type_t<not_invocable<int, int>, int>,
                           not_invocable<int, int>>);

static_assert(std::same_as<common_type_t<not_invocable<int, int>, not_invocable<int, double>>,
                           not_invocable<int, int>>);

}  // namespace common_type_test

namespace one_case_result_test {

struct A {};
struct B {};
struct C {};
struct D {};

using V1 = tools::detail::variant_data<A, B>;
using V2 = tools::detail::variant_data<C, D>;


struct Visit {
  A&& operator()(A&&, C&&) const noexcept;
  D&& operator()(A&&, D&&) const;
  B&& operator()(B&&, C&&) const;
  D&& operator()(B&&, D&&) const noexcept;
};

using namespace tools::detail;

static_assert(
  std::same_as<one_case_res_t<0, Visit, V1&&, V2&&>, A&&>
);

static_assert(one_case_noexcept_v<0, Visit, V1&&, V2&&>);

static_assert(
  std::same_as<one_case_res_t<1, Visit, V1&&, V2&&>, D&&>
);

static_assert(!one_case_noexcept_v<1, Visit, V1&&, V2&&>);

static_assert(
  std::same_as<one_case_res_t<2, Visit, V1&&, V2&&>, B&&>
);

static_assert(
  std::same_as<one_case_res_t<3, Visit, V1&&, V2&&>, D&&>
);

static_assert(
  std::same_as<one_case_res_t<0, Visit, V1 const&, V2&&>,
  not_invocable<Visit, A const &, C&&>>
);

}  // namespace one_case_result_test

namespace visit_result_test {

using namespace tools::detail;

using V1 = tools::detail::variant_data<char, short>;
using V2 = tools::detail::variant_data<int, double>;
using V3 = tools::detail::variant_data<std::string, double>;

auto min_l = [](auto a, auto b) -> decltype( b < a ? b : a ) {
  return b < a ? b : a;
};

static_assert(std::same_as<decltype(
  tools::detail::visit(min_l, std::declval<V1>(), std::declval<V2>())),
  double>);

#if 0
void foo(V1& v1, V3& v3) {
  tools::detail::visit(min_l, v1, v3);
}
#endif

}  // namespace visit_result_test

} // namespace


int main() {
  switch_test::switch_runtime_test();
}
