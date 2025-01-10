#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/context.hpp"
#include "../details/make_source.hpp"
#include "../details/values.hpp"

namespace ascip_details::prs {

template<auto from, auto to> struct range_parser : base_parser<range_parser<from,to>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto sym = src();
		const bool ok = from <= sym && sym <= to;
		eq( ok, result, sym );
		if constexpr (from <= '\n' && '\n' <= to)
			count_new_line(ok, ctx, sym, result);
		return -2 * !ok + 1;
	}
};

constexpr static bool test_range_parser() {
	constexpr static auto ascii = range_parser<(char)0x01,(char)0x7F>{};
#ifndef __clang__
	constexpr static auto lower = range_parser<'a','z'>{};
	static_assert( ({char r{};lower.parse(make_test_ctx(), make_source("a"), r);r;}) == 'a' );
	static_assert( ({char r{};lower.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source("A"), r);}) == 1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source('~'+1), r);}) == 1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source('~'+2), r);}) == -1 );
#endif
	static_assert( [&] {
		char r{};
		auto ctx = make_test_ctx();
		ascii.parse(ctx, make_source("\n"), r);
		return new_line_count(ctx);
	}() == 2 );
	return true;
}

} // namespace ascip_details::prs
