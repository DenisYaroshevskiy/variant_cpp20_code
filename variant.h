
// variant --------------------------------

namespace tools {
namespace _variant {

constexpr auto&& get_data(auto&& self) {
  return TOOLS_FWD(self).data;
}

}  // namespace _variant

template <typename ...Ts>
class variant {
  detail::variant_data<Ts...> data;
  friend constexpr auto&& _variant::get_data(auto&& self);
 public:
  template <one_of<Ts...> T>
  constexpr explicit variant(T x) {
    tools::construct_at<tools::find_type_idx_v<T, Ts...>>(data.data, x);
    data.idx = tools::find_type_idx_v<T, Ts...>;
  }

  constexpr variant(const variant& _x) noexcept(
      (std::is_nothrow_copy_constructible_v<Ts> && ...)
  ) {
    detail::visit([&]<typename T>(const T& x) mutable {
      tools::construct_at<tools::find_type_idx_v<T, Ts...>>(data.data, x);
    }, _x.data);
    data.idx = _x.data.idx;
  }

  constexpr ~variant() {
    detail::visit([]<typename T>(T& self) {
      std::destroy_at(&self);
    }, data);
  }
};

template <typename Vis, typename ...Vs>
  requires ( instance_of<std::remove_cvref_t<Vs>, tools::variant> && ... )
constexpr auto visit(Vis&& vis, Vs&& ... vs)
  noexcept ( noexcept(detail::visit(TOOLS_FWD(vis), _variant::get_data(TOOLS_FWD(vs)) ...)) ) ->
  decltype ( detail::visit(TOOLS_FWD(vis), _variant::get_data(TOOLS_FWD(vs)) ...) )
{
  return detail::visit(TOOLS_FWD(vis), _variant::get_data(TOOLS_FWD(vs)) ...);
}

}  // namespace tools
