#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/definition_tags.hpp"

namespace ascip_details::prs {

template<typename value_maker, typename searcher, parser parser>
struct by_table_parser : base_parser<by_table_parser<value_maker, searcher, parser>>, by_table_parser_tag {
  parser p;
  value_maker make;
  searcher search;

  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    auto tmp_result = make();
    auto get_result = [&]->auto& {
      if constexpr (std::is_pointer_v<std::decay_t<decltype(tmp_result)>>) return *tmp_result;
      else return tmp_result;
    };
    auto ret = p.parse(ctx, src, get_result());
    if (0 <= ret) eq(result, search(get_result()));
    return ret;
  }
};

constexpr auto by_table(auto maker, auto searcher, auto parser) {
  return by_table_parser<decltype(maker), decltype(searcher), decltype(parser)>{{}, {}, std::move(parser), std::move(maker), std::move(searcher)};
}

}