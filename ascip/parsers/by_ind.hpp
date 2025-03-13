#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/reflection.hpp"

namespace ascip_details::prs {

template<auto ind, parser parser> struct by_ind_parser : base_parser<by_ind_parser<ind, parser>> {
  parser p;
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    if constexpr (requires{get<ind>(result);}) return p.parse(ctx, std::move(src), get<ind>(result));
    else return p.parse(ctx, std::move(src), ascip_reflection::get<ind>(result));
  }
};

}

namespace ascip_details {

template<auto ind, parser parser> constexpr auto by_ind(parser&& p) {
  return prs::by_ind_parser<ind, std::decay_t<parser>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename mutator, auto ind, typename type>
constexpr static auto transform_special(prs::by_ind_parser<ind, type>&& src, auto& ctx) {
  auto nctx = mutator::create_ctx(src, ctx);
  auto p = transform<mutator>(std::move(src.p), nctx);
  return transform_apply<mutator>(by_ind<ind>( std::move(p) ), nctx);
}

}

namespace ascip_details::prs {

template<typename p, template<auto>class th=p::template tmpl>
constexpr bool test_by_ind() {
  constexpr auto pp = by_ind<1>(th<'a'>::char_) >>  by_ind<0>(th<'b'>::char_);
  static_assert( [&] {
    struct result{ char f1{}, f2{}; } r;
    auto pr = parse(pp, +p::space, p::make_source("ab"), r);
    return (pr==2) + 2*(r.f1=='b') + 4*(r.f2=='a');
  }() == 7 );
  return true;
}

}
