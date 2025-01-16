#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "base.hpp"
#include "../details/values.hpp"
#include "../details/test_utils.hpp"

namespace ascip_details::prs {

template<ascip_details::parser parser> struct omit_parser : base_parser<omit_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr omit_parser() =default ;
	constexpr omit_parser(omit_parser&&) =default ;
	constexpr omit_parser(const omit_parser&) =default ;
	constexpr omit_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		type_parse_without_result r;
		return p.parse(ctx, src, r);
	}
};

constexpr auto omit(auto&& p) {
	using type = std::decay_t<decltype(p)>;
	return omit_parser<type>{ std::forward<decltype(p)>(p) };
}

template<typename p>
constexpr static bool test_omit() {
	static_assert( test_parser_char(omit(p::template char_<'a'>), "a", 1) == 'z' );
	static_assert( test_parser_char(omit(p::template char_<'a'>), "b", -1) == 'z' );
	static_assert( test_parser_char(p::template _char<'a'>, "a", 1) == 'z' );
	static_assert( test_parser_char(p::template _char<'a'>, "b", -1) == 'z' );
	return true;
}

} // namespace ascip_details::prs
