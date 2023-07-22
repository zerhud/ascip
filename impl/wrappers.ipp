#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<ascip_details::parser parser> struct negate_parser : parser {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = parser::parse(ctx, static_cast<decltype(auto(src))&&>(src), result);
		return -1 * (0 <= ret);
	}
	constexpr auto operator!() {
		return static_cast<const parser&>(*this);
	}
};

constexpr static bool test_negate() {
	static_assert( ({char r;(!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);}) == -1 );
	static_assert( ({char r;(!!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);}) == 1 );
	static_assert( ({char r;(!!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);r;}) == 'a' );
	static_cast<const decltype(auto(char_<'a'>))&>(!!!char_<'a'>);
	return true;
}

template<ascip_details::parser parser> struct opt_parser : base_parser<opt_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), result);
		return -1 * (0 <= ret);
	}
};

constexpr static bool test_optional() {
	return false;
}
