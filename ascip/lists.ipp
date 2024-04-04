#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<typename type> struct forwarder{ type& o; constexpr forwarder(type& o) : o(o) {} };
template<typename type> forwarder(type&) -> forwarder<type>;
constexpr static auto fwd(auto& o) { return forwarder( o ); }

template<ascip_details::parser parser> struct unary_list_parser : base_parser<unary_list_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr unary_list_parser(unary_list_parser&&) =default ;
	constexpr unary_list_parser(const unary_list_parser&) =default ;
	constexpr unary_list_parser() =default ;
	constexpr unary_list_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = call_parse(ctx, src, result);
		src += ret * (0<=ret);
		auto cur_r = ret;
		while(src && 0<cur_r) {
			cur_r = call_parse(ctx, src, result);
			ret += cur_r * (0<=cur_r);
			src += cur_r * (0<=cur_r);
		}
		if(src) ascip_details::pop(result);
		return ret;
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = ascip_details::is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return p.parse(ctx, src, result.o);
		else return p.parse(ctx, src, ascip_details::empback(result));
	}
};

template<ascip_details::parser left, ascip_details::parser right>
struct binary_list_parser : base_parser<binary_list_parser<left, right>> {
	left lp;
	right rp;
	constexpr binary_list_parser(left l, right r) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow fake_result;
		auto ret = call_parse(ctx, src, result);
		if(ret<0) ascip_details::pop(result);
		auto cur = ret;
		while(cur > 0) {
			src += cur;
			auto r_res = rp.parse(ctx, src, fake_result);
			if( r_res <= 0 ) break;
			src += r_res;
			cur = call_parse(ctx, src, result);
			if( cur <= 0 ) {
				ascip_details::pop(result);
				break;
			}
			ret += cur + r_res;
		}
		return ret;
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = ascip_details::is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return lp.parse(ctx, src, result.o);
		else return lp.parse(ctx, src, ascip_details::empback(result));
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
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), (+(char_<'a'> >> char_<'b'>))([](auto& r){return fwd(r);}), "abab", 4), 'a','b','a','b' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), (+(char_<'a'> >> char_<'b'>))([](auto& r){return fwd(r);}), "", -1) ));

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
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % d10, "A1", -1) ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), ((lower >> lower) % ',')([](auto&r){return fwd(r);}), "ab,cd", 5), 'a','b','c','d' ));
	return true;
}

constexpr static bool test_lists() {
	return test_unary_list() && test_binary_list();
}
