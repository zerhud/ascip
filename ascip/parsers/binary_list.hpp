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

template<parser left, parser right>
struct binary_list_parser : base_parser<binary_list_parser<left, right>> {
	left lp;
	right rp;
	constexpr binary_list_parser(left l, right r) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_parse_without_result fake_result;
		auto ret = call_parse(ctx, src, result);
		if(ret<0) pop(result);
		auto cur = ret;
		while(cur > 0) {
			src += cur;
			auto r_res = rp.parse(ctx, src, fake_result);
			if( r_res <= 0 ) break;
			src += r_res;
			cur = call_parse(ctx, src, result);
			if( cur <= 0 ) {
				pop(result);
				break;
			}
			ret += cur + r_res;
		}
		return ret;
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return lp.parse(ctx, src, result.o);
		else return lp.parse(ctx, src, empback(result));
	}
};

template<parser left, parser right> constexpr auto operator%(left&& l, const right& r) {
	return binary_list_parser( std::forward<decltype(l)>(l), std::forward<decltype(r)>(r) );
}

constexpr auto operator%(parser auto&& l, char r) {
	return binary_list_parser( std::forward<decltype(l)>(l), value_parser{r} );
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename left_type, typename right_type>
constexpr static auto transform(prs::binary_list_parser<left_type, right_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto lp = transform<mutator>(std::move(src.lp), nctx);
	auto rp = transform<mutator>(std::move(src.rp), nctx);
	return transform_apply<mutator>(prs::binary_list_parser( std::move(lp), std::move(rp) ), nctx);
}

}