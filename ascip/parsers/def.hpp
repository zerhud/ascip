#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/values.hpp"

namespace ascip_details::prs {

template<parser type> struct def_parser : base_parser<def_parser<type>> { type p; };

template<parser type> constexpr auto def(type&& p) {
  return def_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) };
}

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_def_parser() {
  static_assert( [] {
    char r{};
    auto pr = parse(t<'a'>::char_ >> def(t<'b'>::char_) >> t<'c'>::char_, p::make_source("abc"), r);
    return (pr==3) + 2*(r=='c');
  }() == 3 );
  static_assert( [] {
    type_parse_without_result r{};
    return parse(t<'a'>::char_ >> def(t<'b'>::char_) >> t<'c'>::char_, p::make_source("abc"), r);
  }() == 3 );
  static_assert( [] {
    type_check_parser r{};
    return parse(t<'a'>::char_ >> def(t<'b'>::char_) >> t<'c'>::char_, p::make_source("abc"), r);
  }() == 2 );
  static_assert( [] {
    type_parse_without_result r{};
    return parse(t<'a'>::char_ >> t<'b'>::char_ > t<'c'>::char_, +p::space, p::make_source("abc"), r, [](int,auto){return 0;});
  }() == 3 );
  static_assert( [] {
    type_check_parser r{};
    return parse(t<'a'>::char_ >> t<'b'>::char_ > t<'c'>::char_, +p::space, p::make_source("abc"), r, [](int,auto){return 0;});
  }() == 2 );
  return true;
}

}