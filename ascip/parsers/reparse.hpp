#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"

namespace ascip_details::prs {

template<parser parser> struct reparse_parser : base_parser<reparse_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto r = p.parse(static_cast<decltype(ctx)&&>(ctx), src, result);
		return r * (r<0);
	}
};

template<parser type> constexpr auto reparse(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return reparse_parser<ptype>{ {}, std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_reparse() {
	static_assert( []{
		char r;
		auto p = (reparse(a::any) >> a::any).parse(a::make_test_ctx(), a::make_source("ab"), r);
		return (p==1) + 2*(r=='a');
	}() == 3);
	static_assert( []{
		char r='z';
		auto p = (reparse(t<'b'>::char_) >> a::any).parse(a::make_test_ctx(), a::make_source("ab"), r);
		return (p==-1) + 2*(r=='z');
	}() == 3);
	return true;
}

} // namespace ascip_details::prs
