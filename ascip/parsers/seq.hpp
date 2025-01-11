#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "value.hpp"
#include "seq/common.hpp"
#include "seq/use_seq_result.hpp"
#include "seq/position_info.hpp"
#include "seq/reqursion.hpp"
#include "seq/struct_fields.hpp"
#include "../details.hpp"

namespace ascip_details::prs {

template<string_literal message, parser type> struct seq_error_parser ;
template<typename... parsers> struct opt_seq_parser : base_parser<opt_seq_parser<parsers...>>, seq_tag {
	tuple<parsers...> seq;

	constexpr opt_seq_parser() =default ;
	constexpr opt_seq_parser(tuple<parsers...> t) : seq(std::move(t)) {}
	constexpr opt_seq_parser(auto&&... args) requires (sizeof...(parsers) == sizeof...(args)) : seq( static_cast<decltype(args)&&>(args)... ) {}
	constexpr opt_seq_parser(const opt_seq_parser&) =default ;

	template<template<typename...>class tmpl>
	constexpr static auto is_spec_checker = [](const auto* p) {
		return is_specialization_of<std::decay_t<decltype(*p)>, tmpl>;
	};
	template<typename type>
	constexpr static bool _exists_in(auto&& ch) {
		return exists_in((type*)nullptr, ch, [](const auto* p){
			return requires{ p->seq; } && !requires{ static_cast<const opt_seq_parser*>(p); };
		});
	}
	template<typename type> constexpr static bool is_field_separator = _exists_in<type>([](const auto* p){return requires{ static_cast<const seq_inc_rfield*>(p); };});
	template<typename type> constexpr static bool is_inc_field_val = _exists_in<type>(is_spec_checker<seq_inc_rfield_val>);
	template<typename type> constexpr static bool is_num_field_val = _exists_in<type>(is_spec_checker<seq_num_rfield_val>);
	template<typename type> constexpr static bool is_inc_field_after = _exists_in<type>(is_spec_checker<seq_inc_rfield_after>);
	template<typename type> constexpr static bool is_inc_field_before = _exists_in<type>(is_spec_checker<seq_inc_rfield_before>);
	template<typename type> constexpr static bool is_dec_field_after = _exists_in<type>(is_spec_checker<seq_dec_rfield_after>);
	template<typename type> constexpr static bool is_dec_field_before = _exists_in<type>(is_spec_checker<seq_dec_rfield_before>);
	template<typename... types> constexpr static bool is_struct_requires =
		((is_field_separator<types> + ...) + (is_inc_field_val<types> + ...) +
		 (is_inc_field_after<types> + ...) + (is_inc_field_before<types> + ...) +
		 (is_dec_field_after<types> + ...) + (is_dec_field_before<types> + ...) +
		 (is_num_field_val<types> + ...)
		) > 0;
	constexpr static bool is_struct_requires_pd = is_struct_requires<parsers...>;

	template<typename type, template<typename...>class tmpl> constexpr static int grab_num_val() {
		int val = 0;
		exists_in((type*)nullptr, [&val](const auto* p){
			constexpr bool is_num = ascip_details::is_specialization_of<std::decay_t<decltype(*p)>, tmpl>;
			if constexpr (is_num) val = std::decay_t<decltype(*p)>::value;
			return is_num;
		}, [](const auto* p) {
			return requires{ p->seq; } && !requires{ static_cast<const opt_seq_parser*>(p); };
		});
		return val;
	}
	template<typename type> constexpr static int num_field_val() { return grab_num_val<type, seq_num_rfield_val>(); }
	template<typename type> constexpr static auto inc_field_val() { return grab_num_val<type, seq_inc_rfield_val>(); }

	template<auto find> constexpr auto call_parse(ascip_details::parser auto& p, auto&& ctx, auto src, auto& result) const {
		if constexpr (!is_struct_requires_pd) return p.parse(ctx, src, result);
		else if constexpr (is_field_separator<decltype(auto(p))>) return p.parse(ctx, src, result);
		else if constexpr ( std::is_same_v<std::decay_t<decltype(result)>, ascip_details::type_any_eq_allow> )
			return p.parse(ctx, src, result);
		else {
			return p.parse(ctx, src, ascip_reflection::get<find>(result));
		}
	}
	template<auto find, auto pind, typename cur_t, typename... tail> constexpr parse_result parse_seq(auto&& ctx, auto src, auto& result) const {
		//TODO: use -1 as last struct field, -2 as the field before last one and so on...
		constexpr auto cur_field = num_field_val<cur_t>() +
			( (find + is_inc_field_before<cur_t> + (-1*is_dec_field_before<cur_t>) + inc_field_val<cur_t>()) * !is_num_field_val<cur_t> );
		constexpr auto nxt_field = cur_field + is_inc_field_after<cur_t> + (-1*is_dec_field_after<cur_t>) + is_field_separator<cur_t>;

		auto& cur = get<pind>(seq);
		auto ret = call_parse<cur_field>(cur, ctx, src, result);
		src += ret * (0 <= ret);
		//NOTE: check src and return  ret if no more data exists?
		*search_in_ctx<seq_shift_stack_tag>(ctx) += ret * (0 <= ret);
		if constexpr (pind+1 == sizeof...(parsers)) return ret;
		else {
			if( ret < 0 ) return ret;
			auto req = parse_seq<nxt_field, pind+1, tail...>(ctx, src, result);
			return req*(req<0) + (ret+req)*(0<=req);
		}
	}

	template<auto find, auto pind>
	constexpr parse_result parse_and_store_shift(auto&& ctx, auto src, auto& result) const {
		//static_assert - exists concrete in ctx
		auto* old_shift = search_in_ctx<seq_shift_stack_tag>(ctx);
		auto cur_shift = 0;
		search_in_ctx<seq_shift_stack_tag>(ctx) = &cur_shift;
		auto ret = parse_seq<find, pind, parsers...>(ctx, src, result);
		search_in_ctx<seq_shift_stack_tag>(ctx) = old_shift;
		return ret;
	}
	constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
		return parse_seq<0, 0, parsers...>(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return -1;
		auto shift_store = 0;
		auto cur_ctx = make_ctx<seq_shift_stack_tag>(&shift_store,
		  make_ctx<seq_result_stack_tag>(&result,
		    make_ctx<seq_stack_tag>(this, ctx)
		  )
		);
		return parse_and_store_shift<0,0>(make_ctx<seq_crop_ctx_tag>(1, cur_ctx), src, result);
	}
};
template<typename... p> opt_seq_parser(p...) -> opt_seq_parser<std::decay_t<p>...>;

} // namespace ascip_details::prs

namespace ascip_details {

template<parser... left, typename right> constexpr auto operator>>(prs::opt_seq_parser<left...>&& l, right&& r) {
    return [&]<auto... inds>(std::index_sequence<inds...>){
        return prs::opt_seq_parser<left..., std::decay_t<right>>{ std::move(get<inds>(l.seq))..., std::move(r) };
    }(std::make_index_sequence<sizeof...(left)>{});
}
template<parser left, typename right> constexpr auto operator>>(left&& l, right&& r)
	requires (!is_specialization_of<std::decay_t<left>, prs::opt_seq_parser>) {
	return prs::opt_seq_parser<std::decay_t<left>, std::decay_t<right>>{ std::forward<left>(l), std::forward<right>(r) };
}
template<parser left> constexpr auto operator>>(left&& l, char r) {
	return std::forward<decltype(l)>(l) >> prs::value_parser{ r };
}
template<parser p> constexpr auto operator++(p&& l) {
	return prs::seq_inc_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator++(p&& l,int) {
	return prs::seq_inc_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l) {
	return prs::seq_dec_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l,int) {
	return prs::seq_dec_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }

template<auto cnt, parser type> constexpr auto finc(type&& p) {
	using p_type = std::decay_t<type>;
	using inc_type = prs::_seq_rfield_val<cnt>;
	return prs::seq_inc_rfield_val<p_type, inc_type>{ std::forward<decltype(p)>(p) }; }
template<auto cnt, parser type> constexpr auto fnum(type&& p) {
	using p_type = std::decay_t<type>;
	using num_type = prs::_seq_rfield_val<cnt>;
	return prs::seq_num_rfield_val<p_type, num_type>{ std::forward<decltype(p)>(p) }; }

template<typename mutator, template<typename...>class seq_parser, typename... list>
constexpr static auto transform(seq_parser<list...>&& src, auto& ctx) requires requires{ src.seq; } {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>(transform_apply_to_each<mutator, seq_parser, 0>(std::move(src.seq), nctx), nctx);
}

}