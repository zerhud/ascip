#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/hana.hpp"
#include "../details/values.hpp"
#include "../details/forwarder.hpp"

namespace ascip_details::prs {

struct unary_list_shift_tag {};

template<auto ind> struct unary_list_shift_parser : base_parser<unary_list_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
		eq(result, *search_in_ctx<ind, unary_list_shift_tag>(ctx));
		return 0;
	}
};

template<parser parser> struct unary_list_parser : base_parser<unary_list_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr unary_list_parser(unary_list_parser&&) =default ;
	constexpr unary_list_parser(const unary_list_parser&) =default ;
	constexpr unary_list_parser() =default ;
	constexpr unary_list_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		parse_result shift_storage = 0;
		auto nctx = make_ctx<list_shift_tag, unary_list_shift_tag>(&shift_storage, ctx);
		parse_result ret = -1;
		parse_result cur_r = 0;
		while (src && 0<=cur_r) {
			cur_r = call_parse(nctx, src, result);
			shift_storage = cur_r;
			ret += cur_r * (0<=cur_r);
			src += cur_r * (0<=cur_r);
		}
		if(src) pop(result);
		return (ret+1)*(0<=ret) + ret*(ret<0);
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return p.parse(ctx, src, result.o);
		else return p.parse(ctx, src, empback(result));
	}
};

template<parser p> constexpr auto operator+(p&& _p) {
	return unary_list_parser<std::decay_t<p>>{ _p };
}

template<parser p> constexpr auto operator*(p&& _p) {
	return -( +(std::forward<decltype(_p)>(_p)) );
}

} // namespace ascip_details::prs
