#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "base.hpp"
#include "../details/context.hpp"

namespace ascip_details::prs {

template<typename parser, typename tag, typename value_type> struct add_to_ctx_parser : base_parser<add_to_ctx_parser<parser, tag, value_type>> {
  value_type value;
  [[no_unique_address]] parser p;

  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    auto new_ctx = make_ctx<tag>(value, ctx);
    return p.parse(new_ctx, src, result);
  }
};

template<typename parser, typename tag, typename maker_type> struct create_in_ctx_parser : base_parser<create_in_ctx_parser<parser, tag, maker_type>> {
  maker_type maker;
  [[no_unique_address]] parser p;

  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const requires requires{ is_parse_non_result(result).ok; } {
    return p.parse(ctx, src, result);
  }
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    auto new_ctx = make_ctx<tag>(maker(result), ctx);
    return p.parse(new_ctx, src, result);
  }
};

template<typename parser, typename act_type, typename... tags> struct ctx_use_parser : base_parser<ctx_use_parser<parser, act_type, tags...>> {
  act_type act;
  [[no_unique_address]] parser p;

  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    auto ret = p.parse(ctx, src, result);
    constexpr bool actual_result = !requires{ is_parse_non_result(result).ok; };
    if constexpr(actual_result) if(-1 < ret) act(result, search_in_ctx<0, tags>(ctx)...);
    return ret;
  }
};

template<typename parser, typename act_type, typename... tags> struct ctx_use_as_result_parser : base_parser<ctx_use_as_result_parser<parser, act_type, tags...>> {
  act_type act;
  [[no_unique_address]] parser p;

  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const requires requires{is_checking(result).ok;} {
    return p.parse(ctx, src, result);
  }
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    auto ret = p.parse(ctx, src, search_in_ctx<0, tags>(ctx)...);
    constexpr bool actual_result = !requires{ is_parse_non_result(result).ok;};
    if constexpr(actual_result) if(-1 < ret) act(result, search_in_ctx<0, tags>(ctx)...);
    return ret;
  }
};

} // namespace ascip_details::prs

namespace ascip_details {

template<typename tag, typename value_type, parser type> constexpr auto add_to_ctx(value_type&& value, type&& p) {
  using ptype = std::decay_t<decltype(p)>;
  return prs::add_to_ctx_parser<ptype, tag, std::decay_t<value_type>>{ {}, std::forward<decltype(value)>(value), std::forward<decltype(p)>(p) };
}
template<typename tag, typename maker_type, parser type> constexpr auto create_in_ctx(maker_type&& value, type&& p) {
  using ptype = std::decay_t<decltype(p)>;
  return prs::create_in_ctx_parser<ptype, tag, std::decay_t<maker_type>>{ {}, std::forward<decltype(value)>(value), std::forward<decltype(p)>(p) };
}
template<typename... tags, parser type> constexpr auto from_ctx(auto&& act, type&& p) {
  using ptype = std::decay_t<decltype(p)>;
  using act_type = std::decay_t<decltype(act)>;
  return prs::ctx_use_parser<ptype, act_type, tags...>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}
template<typename... tags, parser type> constexpr auto result_from_ctx(auto&& act, type&& p) {
  using ptype = std::decay_t<decltype(p)>;
  using act_type = std::decay_t<decltype(act)>;
  return prs::ctx_use_as_result_parser<ptype, act_type, tags...>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename mutator, typename parser, typename tag, typename value_type>
constexpr static auto transform_special(prs::add_to_ctx_parser<parser, tag, value_type>&& src, auto& ctx) {
  auto nctx = mutator::create_ctx(src, ctx);
  auto np = transform<mutator>( std::move(src.p), nctx );
  return transform_apply<mutator>( prs::add_to_ctx_parser<decltype(np), tag, value_type>{ {}, std::move(src.value), std::move(np) }, nctx );
}

template<typename mutator, typename parser, typename tag, typename maker_type>
constexpr static auto transform_special(prs::create_in_ctx_parser<parser, tag, maker_type>&& src, auto& ctx) {
  auto nctx = mutator::create_ctx(src, ctx);
  auto np = transform<mutator>( std::move(src.p), nctx );
  return transform_apply<mutator>( prs::create_in_ctx_parser<decltype(np), tag, maker_type>{ {}, std::move(src.maker), std::move(np) }, nctx );
}

}
