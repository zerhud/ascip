#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "base.hpp"
#include "../details/values.hpp"

namespace ascip_details::prs {

/// undocumented parser, mostly for inner use
template<typename t> struct value_parser : base_parser<value_parser<t>> {
	t val;
	constexpr value_parser(t v) : val(v) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto cur = src();
		const bool ok = cur == val;
		eq(ok, result, cur);
		count_new_line(1, ctx, cur, result);
		return -2 * !ok + 1;
	}

	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		char r{};
		parse(make_test_ctx(), make_source(val), r) == 1           || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val+1), r) == -1        || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val-1), r) == -1        || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val), r),r) == val     || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val-1), r),r) == val+1 || (throw __LINE__, 1);

		return true;
#pragma GCC diagnostic pop
	}
};

#ifdef __clang__
template<typename t> value_parser(t) -> value_parser<t>;
#endif

template<typename t> constexpr auto parse_value(t&& val) {
	return value_parser<std::decay_t<t>>{ std::forward<decltype(val)>(val) };
}

constexpr static bool test_parser_value() {
	static_assert( value_parser{ 'a' }.test() ); static_assert( value_parser{ 'Z' }.test() );
	static_assert( value_parser{ L'!' }.test() ); static_assert( value_parser{ '\n' }.test() );
	static_assert( []{char r{}; return value_parser{'a'}.parse(make_test_ctx(), make_source("abc"), r);}() == 1 );
	static_assert( []{char r{}; return value_parser{'b'}.parse(make_test_ctx(), make_source("abc"), r);}() == -1 );
	static_assert( [] {
		auto ctx1 = make_test_ctx();
		auto ctx2 = make_test_ctx();
		char r{};
		value_parser{'\n'}.parse(ctx1, make_source("a"), r);
		value_parser{'\n'}.parse(ctx2, make_source("\n"), r);
		return (new_line_count(ctx1) == 1) + 2*(new_line_count(ctx2) == 2);
	}() == 3 );
	return true;
}

}

namespace ascip_details {

template<typename mutator, typename value_type>
constexpr static auto transform_special(prs::value_parser<value_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>(std::move(src), nctx );
}

}
