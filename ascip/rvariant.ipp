#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


struct rvariant_rerun_tag {};
struct rvariant_stack_tag {};
struct rvariant_crop_ctx_tag {};
struct rvariant_copied_result_tag {};
template<auto ind> struct rvariant_stop_val { constexpr static auto val = ind; };
constexpr static struct rvariant_lreq_parser : base_parser<rvariant_lreq_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr const bool need_in_result =
			   !is_in_concept_check(decltype(auto(ctx)){})
			&& !std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>
			;
		if constexpr (need_in_result) {
			result = std::move(*search_in_ctx<rvariant_copied_result_tag>(ctx));
			search_in_ctx<rvariant_copied_result_tag>(ctx) = nullptr;
		}
		return 0;
	}
} rv_lreq{};
template<auto stop_ind>
struct rvariant_rreq_parser : base_parser<rvariant_rreq_parser<stop_ind>> {
	constexpr parse_result parse(auto&& ctx, auto, auto&) const requires (is_in_concept_check(decltype(auto(ctx)){})) { return 0; }
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const requires (!is_in_concept_check(decltype(auto(ctx)){})) {
		if(!src) return 0;
		auto* var = search_in_ctx<rvariant_stack_tag>(ctx);
		auto* croped_ctx = search_in_ctx<rvariant_crop_ctx_tag>(ctx);
		auto nctx = make_ctx<rvariant_crop_ctx_tag>(croped_ctx, *croped_ctx);
		return var->template parse_without_prep<stop_ind+1>(nctx, src, result);
	}
};
template<auto stop_ind> constexpr static rvariant_rreq_parser<stop_ind> _rv_rreq{};
constexpr static struct rvariant_rreq_pl_parser : base_parser<rvariant_rreq_pl_parser> { constexpr parse_result parse(auto&& ctx, auto src, auto& result) const { return 0; } } rv_rreq{};
constexpr static struct rvariant_req_parser : base_parser<rvariant_req_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (is_in_concept_check(decltype(auto(ctx)){})) return 0;
		else {
			auto* var = search_in_ctx<rvariant_stack_tag>(ctx);
			auto* croped_ctx = search_in_ctx<rvariant_crop_ctx_tag>(ctx);
			auto nctx = make_ctx<rvariant_crop_ctx_tag>(croped_ctx, *croped_ctx);
			return var->parse(nctx, src, result);
		}
	}
} rv_req{};
template<ascip_details::parser parser>
struct rvariant_top_result_parser : base_parser<rvariant_top_result_parser<parser>> { parser p; };
#ifdef __clang__
template<typename p> rvariant_top_result_parser(p) -> rvariant_top_result_parser<p>;
#endif

template<template<class>class wrapper, ascip_details::parser parser> constexpr static auto is_top_result_parser_helper(const wrapper<parser>& p) -> decltype(p.p);
template<typename parser>
constexpr static bool is_top_result_parser() {
	if constexpr (requires{ is_top_result_parser_helper(std::declval<parser>()); })
		return
			   is_top_result_parser<decltype(is_top_result_parser_helper(std::declval<parser>()))>()
			|| ascip_details::is_specialization_of<parser, rvariant_top_result_parser>;
	else return ascip_details::is_specialization_of<parser, rvariant_top_result_parser>;
}

template<typename maker_type, ascip_details::parser... parsers>
struct rvariant_parser : base_parser<rvariant_parser<maker_type, parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;
	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	template<auto ind> constexpr static bool is_term() {
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		auto checker = [](const auto* p){return std::is_same_v<std::decay_t<decltype(*p)>, std::decay_t<decltype(rv_lreq)>>;};
		auto stop = [](const auto* p){
			using p_type = std::decay_t<decltype(*p)>;
			const bool is_rv = requires{ p->maker; };
			return is_rv && !ascip_details::is_specialization_of<cur_parser_t, rvariant_parser>;
		};
		return !exists_in((cur_parser_t*)nullptr, checker, stop);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const requires ( ascip_details::is_in_concept_check(decltype(ctx){}) ) {
		return 0;
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
		if constexpr (std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>) return result;
		else return maker(result);
	}
	template<auto ind> constexpr parse_result parse_term(auto&& ctx, auto src, auto& result) const {
		if constexpr (ind == 0) {
			if constexpr (is_term<ind>()) return get<ind>(seq).parse(ctx, src, result);
			else return -1;
		}
		else if constexpr (!is_term<ind>()) return parse_term<ind-1>(ctx, src, result);
		else {
			auto cur = get<ind>(seq).parse(ctx, src, ascip_details::variant_result<cur_ind<ind>()>(result));
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
			ascip_details::type_any_eq_allow result_for_check;
			auto pr = get<ind>(seq).parse(ctx, src, result_for_check);
			decltype(pr) prev_pr = 0;
			while(0 < pr) {
				prev_pr += pr;
				auto cur = move_result(result);
				if constexpr (!std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>)
					search_in_ctx<rvariant_copied_result_tag>(ctx) = &cur;
				src += get<ind>(seq).parse(ctx, src, ascip_details::variant_result<cur_ind<ind>()>(result));
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
			make_ctx<rvariant_stack_tag>(this,
			make_ctx<rvariant_rerun_tag>(&ctx, ctx)));
		return parse_without_prep<0>(make_ctx<rvariant_crop_ctx_tag>(&nctx, nctx), src, result);
	}
	constexpr parse_result parse_rerun(auto&& ctx, auto src, auto& result) const {
		auto* rerun_ctx = search_in_ctx<rvariant_rerun_tag>(ctx);
		return parse_with_prep(*rerun_ctx, src, result);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (!exists_in_ctx<rvariant_stack_tag>(decltype(auto(ctx)){}))
			return parse_with_prep(ctx, src, result);
		else  {
			using rv_stack_type = std::decay_t<decltype(search_in_ctx<rvariant_stack_tag>(decltype(auto(ctx)){}))>;
			if constexpr ( std::is_same_v<rv_stack_type, std::decay_t<decltype(this)>> )
				return parse_without_prep<0>(ctx, src, result);
			else return parse_with_prep(ctx, src, result);
			//else return parse_with_prep(ctx, src, result);
			//const bool is_in_neasted_rv = static_cast<const void*>(search_in_ctx<rvariant_stack_tag>(ctx)) != static_cast<const void*>(this);
			//return is_in_neasted_rv ? parse_rerun(ctx, src, result) : parse_without_prep<0>(ctx, src, result);
		}
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

constexpr static auto test_rvariant_simple(auto r, auto&& src, auto&&... parsers) {
	auto var = rv([](auto& r){return &r;}, parsers...);
	var.parse(make_test_ctx(), make_source(src), r);
	static_assert( var.template is_term<0>() );
	static_assert( var.template is_term<1>() );
	auto var_with_skip = inject_skipping(var, +space);
	static_assert( var_with_skip.template is_term<0>() );
	static_assert( var_with_skip.template is_term<1>() );
	return r;
}
template<typename dbl_expr>
constexpr static auto test_rvariant_val(auto r, auto&& maker, auto pr, auto&& src) {
	constexpr auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	auto var = rv(std::forward<decltype(maker)>(maker)
		, cast<dbl_expr>(rv_lreq++ >> _char<'+'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> _char<'-'> >> rv_rreq(rmaker))
		, int_
		, cast<dbl_expr>(rv_lreq++ >> _char<'*'> >> rv_rreq(rmaker)) | cast<dbl_expr>(rv_lreq++ >> _char<'/'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> lit<"**"> >> rv_rreq(rmaker))
		, fp
		, quoted_string
		, rv_result(_char<'('> >> rv_req >> _char<')'>)
		) ;
	auto cr = var.parse(make_test_ctx(), make_source(src), r);
	cr /= (cr == pr);

	auto var_with_skip = inject_skipping(var, +space);
	static_assert( !var_with_skip.template is_term<0>() );
	static_assert( !var_with_skip.template is_term<1>() );
	static_assert(  var_with_skip.template is_term<2>() );
	static_assert( !var_with_skip.template is_term<3>() );
	static_assert( !var_with_skip.template is_term<4>() );
	static_assert(  var_with_skip.template is_term<5>() );
	static_assert(  var_with_skip.template is_term<6>() );
	static_assert(  var_with_skip.template is_term<7>() );

	return r;
}

constexpr static bool test_rvariant_dexpr() {
	struct expr_rt;
	using term_rt = typename factory_t::template variant<int,decltype(mk_str())>;
	struct dbl_expr{
		typename factory_t::template unique_ptr<expr_rt> left;
		typename factory_t::template unique_ptr<expr_rt> right;
	};
	struct mul_expr : dbl_expr {} ;
	struct min_expr : dbl_expr {} ;
	struct pls_expr : dbl_expr {} ;
	struct div_expr : dbl_expr {} ;
	struct pow_expr : dbl_expr {} ;
	struct expr_rt : factory_t::template variant<
		 pls_expr, min_expr,
		 int,
		 typename factory_t::template variant<mul_expr, div_expr>,
		 pow_expr,
		 double, decltype(mk_str())
	       > {};
	constexpr auto pls_ind = 0;
	constexpr auto mul_ind = 3;
	constexpr auto pow_ind = 4;
	constexpr auto int_ind = 2;
	constexpr auto fp_ind = 5;

	constexpr auto maker = [](auto& r){ return typename factory_t::template unique_ptr<expr_rt>( new expr_rt{std::move(r)} ); };
#ifndef __clang__
	static_assert( test_rvariant_val<dbl_expr>(expr_rt{}, maker, -1, "").index() == int_ind, "if parser fails the variant have the last terminal index" );
	static_assert( get<int_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "123")) == 123 );
	static_assert( get<fp_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "0.5")) == 0.5 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1*5"))).left) == 1 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1*5"))).right) == 5 );
	static_assert( get<int_ind>(*get<1>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1/5"))).right) == 5 );
	static_assert( get<int_ind>(*get<pow_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 6, "1*5**2"))).right).right) == 2 );
	static_assert( get<int_ind>(*get<pow_ind>(*get<1>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 6, "1/5**2"))).right).right) == 2 );
	static_assert( get<int_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).right) == 7 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).left)).left) == 1 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).left)).right) == 5 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)"))).left) == 1 );
	static_assert( get<int_ind>(*get<pls_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)"))).right).left) == 5 );
	static_assert( get<int_ind>(*get<pls_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)"))).right).right) == 7 );
#endif

	return true;
}

constexpr static bool test_rvariant() {
	static_assert( test_rvariant_simple(int{}, "123", int_, fp) == 123 );
	static_assert( test_rvariant_simple(double{}, "1.2", int_, fp) == 1.2 );
	return test_rvariant_dexpr();
	return true;
}
