#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "hana.hpp"
#include "values.hpp"

namespace ascip_details {
struct skip_parser_tag{};
struct err_handler_tag{};
struct new_line_count_tag {}; //TODO: fix new line counter or remove it
//NOTE: store in context, copy in prev context if parser not failed
//      or store in parse method variable and put a pointer to the context, make resetter to restore value on fail

template<typename t, typename value_t> struct context_frame {
  constexpr static auto tag = type_holder<t>{};
  value_t value;
};
constexpr auto make_default_context() {
  return tuple{context_frame<new_line_count_tag, int>{1}};
}
constexpr static auto make_default_context(auto err_handler){ return make_ctx<err_handler_tag>(err_handler, make_default_context()); }

constexpr auto make_test_ctx() { return make_default_context(); }
constexpr static auto make_test_ctx(auto err_handler){ return make_default_context(std::move(err_handler)); }

template<template<typename...>class tuple, typename... types> constexpr decltype(auto) repack(tuple<types...>&& tup, auto&& fnc) {
  return [&]<auto... inds>(std::index_sequence<inds...>){
    return fnc(std::move(get<inds>(tup))...);
  }(std::make_index_sequence<sizeof...(types)>{});
}
template<typename tag, typename value, typename... frames> constexpr auto make_ctx(value&& val, tuple<frames...> prev_ctx) {
  using adding_t = context_frame<tag, std::decay_t<value>>;
  return repack(std::move(prev_ctx), [&](auto&&... prev) {
    return tuple<adding_t, frames...>{adding_t{std::forward<decltype(val)>(val)}, std::move(prev)...};
  });
}
template<typename tag, typename value> constexpr auto make_ctx(value&& val) {
  return make_ctx<tag>(std::forward<decltype(val)>(val), make_default_context());
}

constexpr struct ctx_not_found_type {} ctx_not_found ;
template<typename tag, auto cur, auto cur_ind, auto ind, auto sz> constexpr auto& _search_in_ctx(auto& ctx) {
  if constexpr (sz<=cur) return ctx_not_found;
  else {
    if constexpr (decltype(auto(get<cur>(ctx)))::tag != type_c<tag>) return _search_in_ctx<tag, cur+1, cur_ind, ind, sz>(ctx);
    else {
      if constexpr (cur_ind==ind) return get<cur>(ctx).value;
      else return _search_in_ctx<tag, cur+1, cur_ind+1, ind, sz>(ctx);
    }
  }
}
template<typename tag, auto ind=0, typename... frames> constexpr auto& search_in_ctx(tuple<frames...>& ctx) {
  return _search_in_ctx<tag, 0, 0, ind, sizeof...(frames)>(ctx);
}
template<typename tag, auto ind=0, typename... frames> constexpr const auto& search_in_ctx(const tuple<frames...>& ctx) {
  return _search_in_ctx<tag, 0, 0, ind, sizeof...(frames)>(const_cast<tuple<frames...>&>(ctx));
}
/*
template<typename tag, auto ind=0> constexpr auto& search_in_ctx(auto&& ctx) { // for tests
  return search_in_ctx<tag,ind>(ctx);
}
*/

template<typename tag, typename... frames> constexpr bool exists_in_ctx(const tuple<frames...>&) {
  return ((frames::tag == type_c<tag>) + ... );
}


template<typename tag, auto cur, auto cur_ind, auto ind, auto sz> constexpr auto _crop_ctx(auto ctx, auto&&... tail) {
  static_assert( cur <= sz, "required frame not found");
  if constexpr (decltype(ctx)::tag != type_c<tag>) return _crop_ctx<tag, cur+1, cur_ind, ind, sz>(std::move(tail)...);
  else {
    if constexpr (cur_ind==ind) return tuple<decltype(ctx), std::decay_t<decltype(tail)>...>{std::move(ctx), std::move(tail)...};
    else return _crop_ctx<tag, cur+1, cur_ind+1, ind, sz>(std::move(tail)...);
  }
}
template<auto ind, typename tag, typename... frames> constexpr auto crop_ctx(tuple<frames...> ctx) {
  return [&]<auto... inds>(std::index_sequence<inds...>) {
    return _crop_ctx<tag, 0, 0, ind, sizeof...(frames)>(get<inds>(ctx)...);
  }(std::make_index_sequence<sizeof...(frames)>{});
}

constexpr void count_new_line(bool result_ok, auto& ctx, auto sym, auto& r) {
	constexpr bool need_count_new_lines =
		   exists_in_ctx<new_line_count_tag>(decltype(auto(ctx)){})
		&& type_dc<decltype(r)> != type_c<type_any_eq_allow>
	;
	if constexpr (need_count_new_lines)
		search_in_ctx<new_line_count_tag>(ctx) += (sym == '\n') * result_ok;
}

constexpr auto new_line_count(auto& ctx) {
  auto& result = search_in_ctx<new_line_count_tag>(ctx);
  if constexpr (requires{result == 0;}) return result;
  else return 0;
}

} // namespace ascip_details
