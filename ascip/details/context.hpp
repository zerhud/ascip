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
struct new_line_count_tag {};
struct inverted_result_tag {};
struct use_result_tag {};

template<typename value_t, typename... t> struct context_frame {
  constexpr static auto tags = type_list<t...>{};
  value_t value;
};
template<typename l_value_t, typename... l_t, typename r_value_t, typename... r_t>
constexpr auto operator+(context_frame<l_value_t, l_t...> left, context_frame<r_value_t, r_t...> right) {
  return tuple<context_frame<l_value_t, l_t...>, context_frame<r_value_t, r_t...>>{ std::move(left), std::move(right) };
}
template<typename... l_frames, typename r_value_t, typename... r_t>
constexpr auto operator+(tuple<l_frames...> left, context_frame<r_value_t, r_t...> right) {
  return [&]<auto... inds>(std::index_sequence<inds...>) {
    return tuple<l_frames..., context_frame<r_value_t, r_t...>>{ get<inds>(left)..., right};
  }(std::make_index_sequence<sizeof...(l_frames)>{});
}
constexpr auto make_default_context() {
  return tuple{context_frame<int, new_line_count_tag>{1}};
}
constexpr auto make_default_context(auto* nls) {
  using st = std::decay_t<decltype(*nls)>;
  return tuple{context_frame<st*, new_line_count_tag>{nls}};
}
constexpr static auto make_default_context(auto* nls, auto err_handler) {
  return make_ctx<err_handler_tag>(err_handler, make_default_context(nls));
}

constexpr auto make_test_ctx() { return make_default_context(); }
constexpr static auto make_test_ctx(auto* nls, auto err_handler){ return make_default_context(nls, std::move(err_handler)); }

template<template<typename...>class tuple, typename... types> constexpr decltype(auto) repack(tuple<types...> tup, auto&& fnc) {
  return [&]<auto... inds>(std::index_sequence<inds...>){
    return fnc(std::move(get<inds>(tup))...);
  }(std::make_index_sequence<sizeof...(types)>{});
}
template<typename... tag, typename value, typename... frames> constexpr auto make_ctx(value&& val, tuple<frames...> prev_ctx) {
  using adding_t = context_frame<std::decay_t<value>, tag...>;
  return repack(std::move(prev_ctx), [&](auto&&... prev) {
    return tuple<adding_t, frames...>{adding_t{std::forward<decltype(val)>(val)}, std::move(prev)...};
  });
}
template<typename... tag, typename value> constexpr auto make_ctx(value&& val) {
  return make_ctx<tag...>(std::forward<decltype(val)>(val), make_default_context());
}
template<bool cond, typename... tag> constexpr auto make_ctx_if(auto&& val, auto prev_ctx) {
  if constexpr (cond) return make_ctx<tag...>(std::forward<decltype(val)>(val), std::move(prev_ctx));
  else return std::move(prev_ctx);
}

constexpr struct ctx_not_found_type {} ctx_not_found ;
template<auto cur, auto cur_ind, auto ind, auto sz, typename... tag> constexpr auto& _search_in_ctx(type_list<tag...> tags, auto& ctx) {
  if constexpr (sz<=cur) return ctx_not_found;
  else {
    if constexpr (!contains_any(decltype(auto(get<cur>(ctx)))::tags, type_list<tag...>{})) return _search_in_ctx<cur+1, cur_ind, ind, sz>(tags, ctx);
    else {
      if constexpr (cur_ind==ind) return get<cur>(ctx).value;
      else return _search_in_ctx<cur+1, cur_ind+1, ind, sz>(tags, ctx);
    }
  }
}
template<auto ind, typename... tag, typename... frames> constexpr auto& search_in_ctx(tuple<frames...>& ctx) {
  return _search_in_ctx<0, 0, ind, sizeof...(frames)>(type_list<tag...>{}, ctx);
}
template<auto ind, typename... tag, typename... frames> constexpr const auto& search_in_ctx(const tuple<frames...>& ctx) {
  return _search_in_ctx<0, 0, ind, sizeof...(frames)>(type_list<tag...>{}, const_cast<tuple<frames...>&>(ctx));
}
template<typename... tag, typename... frames> constexpr auto& search_in_ctx(tuple<frames...>& ctx) { return search_in_ctx<0, tag...>(ctx); }
template<typename... tag, typename... frames> constexpr const auto& search_in_ctx(const tuple<frames...>& ctx) { return search_in_ctx<0, tag...>(ctx); }

template<typename... _tags, typename... frames> constexpr bool exists_in_ctx(const tuple<frames...>&) {
  constexpr type_list<_tags...> tags{};
  return (contains_any(frames::tags, tags) + ... );
}
template<bool cond, typename... _tags, typename value, typename... frames> constexpr auto replace_in_ctx(value&& val, tuple<frames...> prev) {
  constexpr type_list<_tags...> tags{};
  if constexpr (!cond) return std::move(prev);
  else if constexpr((contains_any(frames::tags, tags) + ...)) return repack(std::move(prev), [&](auto... pack) {
    return (... + [&](auto cur){
      if constexpr(contains_any(decltype(cur)::tags, tags)) return context_frame<std::decay_t<value>, _tags...>{val};
      else return cur;
    }(std::move(pack)));
  });
  else return make_ctx<_tags...>(std::forward<decltype(val)>(val), std::move(prev));
}


constexpr auto& new_line_counter(auto& ctx) {
  if constexpr (requires{*search_in_ctx<new_line_count_tag>(ctx);}) return *search_in_ctx<new_line_count_tag>(ctx);
  else return search_in_ctx<new_line_count_tag>(ctx);
}
struct fucky_clang { constexpr static bool ok=true; };
consteval auto need_count_new_lines(auto& ctx) {
  constexpr bool exists_val = requires{ search_in_ctx<new_line_count_tag>(ctx) = 1; };
  constexpr bool exists_ptr = requires{ *search_in_ctx<new_line_count_tag>(ctx) = 1; };
  if constexpr (exists_ptr || exists_val) return fucky_clang{};
  else return false;
}
consteval auto need_count_new_lines(auto& ctx, auto& r) {
  if constexpr (!requires{ is_checking(r).ok; } && requires{need_count_new_lines(ctx).ok;}) return fucky_clang{};
  else return false;
}
constexpr void count_new_line(auto p_result, auto& ctx, auto sym, auto& r) {
	if constexpr (requires{need_count_new_lines(ctx, r).ok;}) {
	  constexpr bool is_result_inverted = exists_in_ctx<inverted_result_tag>(decltype(auto(ctx)){});
	  if constexpr (is_result_inverted) new_line_counter(ctx) += (sym == '\n') * (p_result < 0);
	  else new_line_counter(ctx) += (sym == '\n') * (p_result > 0);
	}
}

constexpr auto new_line_count(auto& ctx) {
  if constexpr (!requires{need_count_new_lines(ctx).ok;}) return 0;
  else return new_line_counter(ctx);
}

template<typename type> constexpr auto update_shift(auto& ctx, auto ret) {
  if constexpr(requires{ *search_in_ctx<type>(ctx) = 1; }) *search_in_ctx<type>(ctx) = ret * (0<=ret);
}

struct new_line_counter_resetter_fake {
  constexpr static void update(auto&&...) {}
};

template<typename storage>
struct new_line_counter_resetter {
  storage* val;
  storage prev;
  constexpr explicit new_line_counter_resetter(auto& ctx) : val(&new_line_counter(ctx)), prev(*val) {}
  constexpr ~new_line_counter_resetter() { *val = prev; }
  constexpr void update(auto pr) { prev = prev*(pr<=0) + (*val)*(0<pr); }
};

constexpr auto make_new_line_count_resetter(auto& ctx, auto& r) {
  using storage_type = std::decay_t<decltype(new_line_counter(ctx))>;
  if constexpr (!requires{need_count_new_lines(ctx, r).ok;}) return new_line_counter_resetter_fake{};
  else return new_line_counter_resetter<storage_type>{ctx};
}

} // namespace ascip_details
