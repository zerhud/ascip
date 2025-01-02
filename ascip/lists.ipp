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
		if(!src) return -1;
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

