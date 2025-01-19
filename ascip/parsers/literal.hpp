#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../details/context.hpp"
#include "../details/string_literal.hpp"

namespace ascip_details::prs {
template <ascip_details::string_literal val>
struct literal_parser : base_parser<literal_parser<val>> {
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    // TODO: what if the literal contains \n - it will be not counted if literal matches
    // TODO: faster? add [] operator in src for direct access (operator[](auto i){return val[ind+i];})
    auto i = -1, r = 0;
    {
      auto tsrc = src;
      tsrc += val.size() - 1;
      if (!tsrc)
        return -1;
    }
    while (++i < val.size()) r += (src() == val[i]);
    parse_result ret = ((r + 1) * (r == val.size())) - 1;
    if constexpr (val.contains('\n')) {
      val.for_each([&](auto s) { count_new_line(ret, ctx, s, result); });
    }
    return ret;
  }
};

template <ascip_details::string_literal v>
constexpr static auto lit = literal_parser<v>{};
constexpr bool test_literal_parser() {
  char r;
  static_assert(literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd"), r) == 3);
  static_assert(literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd_tail"), r) == 3);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("abcd"), r) == 4);
  static_assert(literal_parser<"abcdef">{}.parse(make_test_ctx(), make_source("abcdef"), r) == 6);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("bbcd"), r) == -1);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("ab"), r) == -1);
  static_assert( [&] {
      auto ctx = make_test_ctx();
      literal_parser<"ab\ncd\nef">{}.parse(ctx, make_source("ab\ncd\nef"), r);
      return new_line_count(ctx);
    }() == 3, "if the literal contains new lines it must to be counted");
  return true;
}

} // namespace ascip_details::prs
