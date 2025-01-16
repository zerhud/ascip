#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/values.hpp"

namespace ascip_details::prs {

template<parser left, parser right> struct different_parser : base_parser<different_parser<left, right>> {
	left lp;
	right rp;
	constexpr different_parser( left l, right r ) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_parse_without_result fake_result;
		if(rp.parse(ctx, src, fake_result) >= 0) return -1;
		return lp.parse(ctx, src, result);
	}
};
template<parser left, parser right> different_parser(left, right) -> different_parser<left, right>;

template<parser left, parser right> constexpr auto operator-(left&& l, right&& r) {
	return different_parser( std::forward<decltype(l)>(l), std::forward<decltype(r)>(r)); }

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_different() {
#ifndef __clang__
	static_assert( ({char r='z';(*(a::any - t<'a'>::char_)).parse(a::make_test_ctx(), a::make_source("#$%a"), r);}) == 3, "different parser: stops on it's excluding parser" );
	static_assert( ({char r='z';(*(a::any - t<'a'>::char_)).parse(a::make_test_ctx(), a::make_source("#$%a"), r);r;}) == '%', "different parser: excluded left result as is" );
#endif
	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename left_type, typename right_type>
constexpr static auto transform(prs::different_parser<left_type, right_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto lp = transform<mutator>(std::move(src.lp), nctx);
	auto rp = transform<mutator>(std::move(src.rp), nctx);
	return transform_apply<mutator>(prs::different_parser( std::move(lp), std::move(rp) ), nctx);
}

}