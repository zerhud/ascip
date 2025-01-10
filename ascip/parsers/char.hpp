#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "base.hpp"
#include "../details.hpp"

namespace ascip_details::prs {
template <auto sym> struct char_parser : base_parser<char_parser<sym>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		const bool ok = src() == sym;
		eq(ok, result, sym);
		count_new_line(ok, ctx, sym, result);
		return -1 + 2 * ok;
	}

	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		char r{};
		parse(make_test_ctx(), make_source(sym), r) == 1 || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym + 1), r) == -1 || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym - 1), r) == -1 || (throw __LINE__, 1);

		r = sym + 1;
		(parse(make_test_ctx(), make_source(sym), r), r) == sym || (throw __LINE__, 1);

		r = sym + 1;
		(parse(make_test_ctx(), make_source(sym - 1), r), r) == sym + 1 || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		if constexpr (sym == '\n') {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);
		}
		else {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		}
#pragma GCC diagnostic pop

		return true;
	}
};

}