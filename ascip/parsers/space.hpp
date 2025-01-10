#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/context.hpp"
#include "../details/make_source.hpp"

namespace ascip_details::prs {

struct space_parser : base_parser<space_parser> {
	constexpr parse_result parse(auto&& ctx,auto src, auto& r) const {
		auto sym = src();
		const bool is_space = 0x07 < sym && sym < '!'; // 0x08 is a backspace
		count_new_line(is_space, ctx, sym, r);
		return -1 + 2*is_space;
	}
	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		constexpr char r=0x00;
		parse(make_test_ctx(), make_source(' '), r) == 1    || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\n'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\t'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('!'), r) == -1   || (throw __LINE__, 1);
		r == 0x00                                           || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		new_line_count(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		new_line_count(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		new_line_count(ctx) == 2 || (throw __LINE__, 1);

		return true;
#pragma GCC diagnostic pop
	}
} ;

} // namespace ascip_details::prs
