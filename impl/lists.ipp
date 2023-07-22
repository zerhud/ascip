#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<ascip_details::parser parser> struct unary_list_parser : base_parser<unary_list_parser<parser>> {
	[[no_unique_address]] parser p;
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

template<typename type>
constexpr static auto mk_vec() { 
	return factory_t{}.template mk_vec<type>();
}

template<auto pr, const char* srct>
constexpr static auto test_parser_parse(auto&& r, auto p, auto&& src) {
	ascip_details::type_any_eq_allow res;
	auto rr = p.parse(make_test_ctx(), make_source(src), res);
	rr /= (rr==pr);
	return 1;
}

constexpr static bool test_unary_list() {
	static_assert( ({auto r=mk_vec<char>(); (*char_<'a'>).parse(make_test_ctx(),make_source(""), r); }) == 0 );
	test_parser_parse<0>(mk_vec<char>(), *char_<'a'>, "");
	return true;
}

constexpr static bool test_lists() {
	return test_unary_list();
}
