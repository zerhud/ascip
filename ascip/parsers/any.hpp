#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/make_source.hpp"
#include "../details/context.hpp"

namespace ascip_details::prs {

struct any_parser : base_parser<any_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = 0;
		decltype(src()) cur;
		do {
			cur = src();
			ascip_details::eq( result, cur );
			count_new_line(true, ctx, cur, result);
			++ret;
		}
		while(src && (cur & 0x80)) ;
		return ret;
	}
	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		char r=0x00; type_parse_without_result rr;
		parse(make_test_ctx(), make_source(' '), r) == 1       || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('Z'), r),r) == 'Z' || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('~'), r),r) == '~' || (throw __LINE__, 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("я"), rr)  == 2);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"я"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"яz"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("❤"), rr) == 3);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("θ"), rr) == 2);

		auto ctx = make_test_ctx();
		search_in_ctx<new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		search_in_ctx<new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		search_in_ctx<new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);

		return true;
#pragma GCC diagnostic pop
	}
};

} // namespace ascip_details::prs
