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

struct binary_list_shift_tag {};

template<auto ind> struct binary_list_shift_parser : base_parser<binary_list_shift_parser<ind>>, any_shift_parser_tag {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
		eq(result, *search_in_ctx<ind, binary_list_shift_tag>(ctx));
		return 0;
	}
};

template<parser left, parser right>
struct binary_list_parser : base_parser<binary_list_parser<left, right>> {
	left lp;
	right rp;
	constexpr binary_list_parser(left l, right r) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		parse_result shift_storage=0;
		constexpr bool need_shift = exists_in((left*)nullptr, is_any_shift_parser);
		auto nctx = make_ctx_if<need_shift, binary_list_shift_tag, list_shift_tag, any_shift_tag>(&shift_storage, ctx);
		type_parse_without_result fake_result;
		parse_result skip=0, cur = 0, ret = -1;
		while (skip >= 0) {
			src += skip;
			cur = call_parse(nctx, src, result);
			if( cur <= 0 ) {
				pop(result);
				break;
			}
			ret += skip + cur;
			src += cur;
			skip = rp.parse(nctx, src, fake_result);
			shift_storage = skip*(skip>=0) + cur*(cur>=0);
		}
		return (ret+1)*(0<=ret) + ret*(ret<0);
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