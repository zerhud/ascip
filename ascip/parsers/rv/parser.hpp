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

template<auto ind> struct rvariant_shift_parser : base_parser<rvariant_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
		eq(result, *search_in_ctx<ind, rvariant_shift_tag>(ctx));
		return 0;
	}
};

template<typename maker_type, parser... parsers>
struct rvariant_parser : base_parser<rvariant_parser<maker_type, parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;

	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	constexpr static auto size() { return sizeof...(parsers); }
	template<auto ind> constexpr static bool is_term() { return rv_utils::contains_recursion<__type_pack_element<ind, parsers...>>(); }
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
		if constexpr (requires{is_parse_non_result(result).ok;}) return result;
		else return maker(result);
	}
	template<auto ind> constexpr parse_result parse_term(auto&& ctx, auto src, auto& result) const {
		if constexpr (ind < 0 || sizeof...(parsers) < ind) return -1;
		else if constexpr (!is_term<ind>()) return parse_term<ind-1>(ctx, src, result);
		else {
			auto cur = get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(result));
			if(cur < 0) return parse_term<ind-1>(ctx, src, result);
			*search_in_ctx<rvariant_shift_tag>(ctx) = cur;
			return cur;
		}
	}
	template<auto ind, auto stop_pos> constexpr parse_result parse_nonterm(auto&& ctx, auto src, auto& result, auto shift) const {
		if(!src) return shift;
		if constexpr (ind < stop_pos) return shift;
		else if constexpr (is_term<ind>()) {
			if constexpr (ind == 0) return shift;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, shift);
		}
		else {
			type_check_parser result_for_check;
			parse_result prev_pr = 0;
			while( get<ind>(seq).parse(ctx, src, result_for_check) >= 0 ) {
				auto cur = move_result(result);
				if constexpr (!requires{is_parse_non_result(result).ok;})
					search_in_ctx<rvariant_cpy_result_tag>(ctx) = &cur;
				auto pr = get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(result));
				src += pr / (pr>=0);
				prev_pr += pr;
				*search_in_ctx<rvariant_shift_tag>(ctx) = pr;
			}
			auto total_shift = shift + prev_pr*(prev_pr>0);
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
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		using copied_result_type = decltype(move_result(result));
		using mono_type = rv_utils::monomorphic<decltype(src), std::decay_t<decltype(result)>>;
		const mono_type* mono_ptr;
		parse_result shift_storage=0;
		auto rctx = make_ctx<rvariant_shift_tag>(&shift_storage,
			make_ctx<rvariant_stack_tag>(&mono_ptr,
			make_ctx<rvariant_cpy_result_tag>((copied_result_type*)nullptr, ctx) ) );
		auto mono = rv_utils::mk_mono(this, rctx, src, result);
		mono_ptr = &mono;
		return mono.parse_mono(0, src, result);
	}
};

template<typename... t> rvariant_parser(t...) ->  rvariant_parser<t...>;

}
