#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<ascip_details::parser parser> struct unary_list_parser : base_parser<unary_list_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr unary_list_parser(unary_list_parser&&) =default ;
	constexpr unary_list_parser(const unary_list_parser&) =default ;
	constexpr unary_list_parser() =default ;
	constexpr unary_list_parser(parser p) : p(std::move(p)) {}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, ascip_details::empback(result));
		src += ret * (0<=ret);
		auto cur_r = ret;
		while(src && 0<cur_r) {
			cur_r = p.parse(ctx, src, ascip_details::empback(result));
			ret += cur_r * (0<=cur_r);
			src += cur_r * (0<=cur_r);
		}
		if(src) ascip_details::pop(result);
		return ret;
	}
};

template<ascip_details::parser left, ascip_details::parser right>
struct binary_list_parser : base_parser<binary_list_parser<left, right>> {
	left lp;
	right rp;
	constexpr binary_list_parser(left l, right r) : lp(l), rp(r) {}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow fake_result;
		auto ret = lp.parse(ctx, src, ascip_details::empback(result));
		if(ret<0) ascip_details::pop(result);
		auto cur = ret;
		while(cur > 0) {
			src += cur;
			auto r_res = rp.parse(ctx, src, fake_result);
			if( r_res <= 0 ) break;
			src += r_res;
			cur = lp.parse(ctx, src, ascip_details::empback(result));
			if( cur <= 0 ) {
				ascip_details::pop(result);
				break;
			}
			ret += cur + r_res;
		}
		return ret;
	}
};
#ifdef __clang__
template<typename p> unary_list_parser(p) -> unary_list_parser<p>;
template<typename l, typename r> binary_list_parser(l,r) -> binary_list_parser<l,r>;
#endif

constexpr static bool test_unary_list() {
	test_parser_parse(mk_vec<char>(), *char_<'a'>, "", 0);
	static_assert(test_parser_parse(mk_vec<char>(), *char_<'a'>, "", 0).size() == 0);
	static_assert(test_parser_parse(mk_vec<char>(), *char_<'a'>, "aa", 2).size() == 2);
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), *char_<'a'>, "aa", 2), 'a', 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), +char_<'a'>, "aa", 2), 'a', 'a' ));

#ifndef __clang__
	static_assert( ({char r='z';char_<'a'>.parse(make_test_ctx(),make_source('b'),r);r;}) == 'z' );
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), +(!char_<'a'>), "bb", 2), 0x00, 0x00 ),
			"!char_<'a'> parses but don't sore it's value, we have list with zeros (instead of infinit loop)");

	static_assert(test_cmp_vec( test_parser_parse(mk_str(),+(char_<'a'>|char_<'b'>), "aab", 3), 'a', 'a', 'b' ));
#endif

	return true;
}

constexpr static bool test_binary_list() {
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % ',', "a,b,c", 5), 'a', 'b', 'c' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % d10, "a1b2c", 5), 'a', 'b', 'c' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % d10, "a", 1), 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % d10, "a1", 1), 'a' ));
	return true;
}

constexpr static bool test_lists() {
	return test_unary_list() && test_binary_list();
}
