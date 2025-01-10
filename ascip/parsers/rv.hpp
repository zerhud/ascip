#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/hana.hpp"

namespace ascip_details::prs {

struct rvariant_stack_tag {};
struct rvariant_crop_ctx_tag {};
struct rvariant_copied_result_tag {};

template<auto ind> struct rvariant_stop_val { constexpr static auto val = ind; };

struct rvariant_lreq_parser : base_parser<rvariant_lreq_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr bool need_in_result = !std::is_same_v<decltype(result), type_any_eq_allow&> ;
		if constexpr (need_in_result) {
			result = std::move(*search_in_ctx<rvariant_copied_result_tag>(ctx));
			search_in_ctx<rvariant_copied_result_tag>(ctx) = nullptr;
		}
		return 0;
	}
};

template<auto stop_ind>
struct rvariant_rreq_parser : base_parser<rvariant_rreq_parser<stop_ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return 0;
		auto* var = search_in_ctx<rvariant_stack_tag>(ctx);
		auto nctx = crop_ctx<0, rvariant_crop_ctx_tag>(ctx);
		return var->template parse_without_prep<stop_ind+1>(nctx, src, result);
	}
};

struct rvariant_rreq_pl_parser : base_parser<rvariant_rreq_pl_parser> {
	constexpr static parse_result parse(auto&&, auto, auto&) { return 0; }
};

template<auto ind> struct rvariant_req_parser : base_parser<rvariant_req_parser<ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto* var = search_in_ctx<rvariant_stack_tag, ind>(ctx);
		auto nctx = crop_ctx<ind, rvariant_crop_ctx_tag>(ctx);
		return var->parse(nctx, src, result);
	}
} ;

template<parser parser>
struct rvariant_top_result_parser : base_parser<rvariant_top_result_parser<parser>> { parser p; };
#ifdef __clang__
template<typename p> rvariant_top_result_parser(p) -> rvariant_top_result_parser<p>;
#endif

template<parser type> constexpr auto rv_result(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return rvariant_top_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) };
}

template<template<class>class wrapper, parser parser> constexpr static auto is_top_result_parser_helper(const wrapper<parser>& p) -> decltype(p.p);
template<typename parser>
constexpr static bool is_top_result_parser() {
	if constexpr (requires{ is_top_result_parser_helper(std::declval<parser>()); })
		return
			   is_top_result_parser<decltype(is_top_result_parser_helper(std::declval<parser>()))>()
			|| is_specialization_of<parser, rvariant_top_result_parser>;
	else return is_specialization_of<parser, rvariant_top_result_parser>;
}

template<typename maker_type, parser... parsers>
struct rvariant_parser : base_parser<rvariant_parser<maker_type, parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;

	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	template<auto ind> constexpr static bool is_term() {
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		auto checker = [](const auto* p){return std::is_same_v<std::decay_t<decltype(*p)>, rvariant_lreq_parser>;};
		auto stop = [](const auto* p){
			const bool is_rv = requires{ p->maker; };
			return is_rv && !is_specialization_of<cur_parser_t, rvariant_parser>;
		};
		return !exists_in((cur_parser_t*)nullptr, checker, stop);
	}
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
	template<auto ind, auto stop_pos>
	constexpr parse_result parse_nonterm(auto&& ctx, auto src, auto& result, auto shift) const {
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
	template<auto stop_pos>
	constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
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
#ifdef __clang__
template<typename... t> rvariant_parser(t...) ->  rvariant_parser<t...>;
#endif

template<auto ind>
struct rvariant_mutator {
	struct context{};
	constexpr static auto create_ctx() { return context{}; }
	constexpr static auto create_ctx(auto&&,auto&&) { return context{}; }
	template<typename type>
	constexpr static auto apply(rvariant_rreq_pl_parser&&,auto&&) {
		return rvariant_rreq_parser<ind>{};
	}
};

template<parser type, parser... types> constexpr auto rv(auto&& maker, type&& first, types&&... list) {
	return [&maker]<auto... inds>(std::index_sequence<inds...>, auto&&... parsers) {
		return rvariant_parser(
				std::move(maker),
				transform<rvariant_mutator<inds>>(std::move(parsers))...
				);
	}(
		  std::make_index_sequence<sizeof...(list)+1>{}
		, std::forward<decltype(first)>(first)
		, std::forward<decltype(list)>(list)...
	);
}


template<typename p, template<auto>class t=p::template tmpl>
constexpr auto test_rvariant_simple(auto r, auto&& src, auto&&... parsers) {
	auto var = rv([](auto& r){return &r;}, parsers...);
	var.parse(make_test_ctx(), make_source(src), r);
	static_assert( var.template is_term<0>() );
	static_assert( var.template is_term<1>() );
	auto var_with_skip = inject_skipping(var, +p::space);
	static_assert( var_with_skip.template is_term<0>() );
	static_assert( var_with_skip.template is_term<1>() );
	return r;
}

template<typename p>
constexpr bool test_rvariant() {
	static_assert( test_rvariant_simple<p>(int{}, "123", p::int_, p::fp) == 123 );
	static_assert( test_rvariant_simple<p>(double{}, "1.2", p::int_, p::fp) == 1.2 );
	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename type, typename... parsers>
constexpr static auto transform(prs::rvariant_parser<type, parsers...>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>( transform_apply_to_each<mutator,prs::rvariant_parser,0>( std::move(src.seq),nctx,std::move(src.maker)), nctx );
}

}