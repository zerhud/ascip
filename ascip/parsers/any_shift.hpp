#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/context.hpp"

namespace ascip_details::prs {

template<auto ind> struct any_shift_parser : base_parser<any_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
  constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
    eq(result, *search_in_ctx<ind, any_shift_tag>(ctx));
    return 0;
  }
};

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_any_shift() {
  struct result{ char s{}; int shift{}; };
  static_assert( [] { result r;
    auto pr = parse(t<'a'>::char_ >> ++t<0>::any_shift, +p::space, p::make_source("  a"), r);
    return (pr==3) + 2*(r.shift==3);
  }() == 3 );
  static_assert( [] { result r;
    auto pr = parse(p::seq_enable_recursion >> t<'a'>::char_ >> ++t<0>::any_shift, +p::space, p::make_source("  a"), r);
    return (pr==3) + 2*(r.shift==3);
  }() == 3 );
  return true;
}

}
