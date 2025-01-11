#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "common.hpp"
#include "result.hpp"
#include "utility.hpp"
#include "monomorphic.hpp"

namespace ascip_details::prs {

template<typename maker_type, parser... parsers>
struct rvariant_parser : base_parser<rvariant_parser<maker_type, parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;

	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	template<auto ind> constexpr static bool is_term() { return rv_utils::contains_reqursion<__type_pack_element<ind, parsers...>>(); }
	template<auto ind> consteval static auto cur_ind() {
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		if constexpr (is_top_result_parser<cur_parser_t>()) return -1;
		else {
			auto cur = 0;
			auto cnt = 0;
			(void)( ((ind==cnt)||(++cnt,cur+=!is_top_result_parser<parsers>(),false)) || ... );
			return cur;
		}
	}
	constexpr auto move_result(auto& result) const {
		if constexpr (std::is_same_v<decltype(result), type_any_eq_allow&>) return result;
		else return maker(result);
	}
	template<auto ind> constexpr parse_result parse_term(auto&& ctx, auto src, auto& result) const {
		if constexpr (ind == 0) {
			if constexpr (is_term<ind>()) return get<ind>(seq).parse(ctx, src, result);
			else return -1;
		}
		else if constexpr (!is_term<ind>()) return parse_term<ind-1>(ctx, src, result);
		else {
			auto cur = get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(result));
			if(0 <= cur) return cur;
			if constexpr (ind==0) return cur;
			else return parse_term<ind-1>(ctx, src, result);
		}
	}
	template<auto ind, auto stop_pos> constexpr parse_result parse_nonterm(auto&& ctx, auto src, auto& result, auto shift) const {
		if(!src) return shift;
		if constexpr (ind < stop_pos) return shift;
		else if constexpr (is_term<ind>()) {
			if constexpr (ind == 0) return 0;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, shift);
		}
		else {
			type_any_eq_allow result_for_check;
			auto pr = get<ind>(seq).parse(ctx, src, result_for_check);
			decltype(pr) prev_pr = 0;
			while(0 < pr) {
				prev_pr += pr;
				auto cur = move_result(result);
				if constexpr (!std::is_same_v<decltype(result), type_any_eq_allow&>)
					search_in_ctx<rvariant_copied_result_tag>(ctx) = &cur;
				src += get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(result));
				pr = get<ind>(seq).parse(ctx, src, result_for_check);
			}
			auto total_shift = shift + (prev_pr*(prev_pr>0));
			if constexpr (ind==0) return total_shift;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, total_shift);
		}
	}
	template<auto stop_pos> constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
		auto term_r = parse_term<sizeof...(parsers)-1>(ctx, src, result);
		if(term_r < 0) return term_r;
		auto nonterm_r = parse_nonterm<sizeof...(parsers)-1, stop_pos>(ctx, src += term_r, result, 0);
		return term_r + (nonterm_r*(nonterm_r>0));
	}
	constexpr parse_result parse_with_prep(auto&& ctx, auto src, auto& result) const {
		using copied_result_type = decltype(move_result(result));
		auto nctx =
			make_ctx<rvariant_copied_result_tag>((copied_result_type*)nullptr,
			make_ctx<rvariant_stack_tag>(this, ctx));
		return parse_without_prep<0>(make_ctx<rvariant_crop_ctx_tag>(1, nctx), src, result);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
        using rv_stack_type = std::decay_t<decltype(search_in_ctx<rvariant_stack_tag>(decltype(auto(ctx)){}))>;
        if constexpr ( std::is_same_v<rv_stack_type, std::decay_t<decltype(this)>> )
            return parse_without_prep<0>(ctx, src, result);
        else return parse_with_prep(ctx, src, result);
	}
};

template<typename... t> rvariant_parser(t...) ->  rvariant_parser<t...>;

}