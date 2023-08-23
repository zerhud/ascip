#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


struct rvariant_stack_tag {};
struct rvariant_crop_ctx_tag {};
struct rvariant_copied_result_tag {};
template<auto ind> struct rvariant_stop_val { constexpr static auto val = ind; };
constexpr static struct rvariant_lreq_parser : base_parser<rvariant_lreq_parser> {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
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
	constexpr auto parse(auto&& ctx, auto, auto&) const requires (is_in_concept_check(decltype(auto(ctx)){})) { return 0; }
	constexpr auto parse(auto&& ctx, auto src, auto& result) const requires (!is_in_concept_check(decltype(auto(ctx)){})) {
		if(!src) return 0;
		auto* var = search_in_ctx<rvariant_stack_tag>(ctx);
		auto* croped_ctx = search_in_ctx<rvariant_crop_ctx_tag>(ctx);
		auto nctx = make_ctx<rvariant_crop_ctx_tag>(croped_ctx, *croped_ctx);
		return var->template parse_without_prep<stop_ind+1>(nctx, src, result);
	}
};
template<auto stop_ind> constexpr static rvariant_rreq_parser<stop_ind> _rv_rreq{};
constexpr static
	struct rvariant_rreq_pl_parser : base_parser<rvariant_rreq_pl_parser> { constexpr auto parse(auto&& ctx, auto src, auto& result) const { return 0; } }
	rv_rreq{};
constexpr static struct rvariant_req_parser : base_parser<rvariant_req_parser> {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
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
struct rvariant_parser : base_parser<rvariant_parser<parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;
	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	template<auto ind> constexpr static bool is_term() {
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		auto checker = [](const auto* p){return std::is_same_v<std::decay_t<decltype(*p)>, std::decay_t<decltype(rv_lreq)>>;};
		return !exists_in((cur_parser_t*)nullptr, checker);
	}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const requires ( ascip_details::is_in_concept_check(decltype(ctx){}) ) {
		return 0;
	}
	template<auto ind, auto cnt, auto cur, typename cur_parser, typename... tail>
	constexpr static auto _cur_ind() {
		constexpr const bool skip = is_top_result_parser<cur_parser>();
		if constexpr (ind == cnt) {
			if constexpr (skip) return -1;
			else return cur;
		}
		else return _cur_ind<ind,cnt+1,cur+(!skip),tail...>();
	}
	template<auto ind> consteval static auto cur_ind() { return _cur_ind<ind,0,0,parsers...>(); }
	template<auto ind> constexpr static auto& cur_result(auto& result) {
		if constexpr (cur_ind<ind>() == -1) return result;
		else if constexpr (requires{ create<1>(result); } ) return create<cur_ind<ind>()>(result);
		else if constexpr (requires{ result.template emplace<1>(); } ) return result.template emplace<cur_ind<ind>()>();
		else return result;
	}
	constexpr auto copy_result(auto& result) const {
		if constexpr (std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>) return result;
		else return maker(result);
	}
	template<auto ind> constexpr auto parse_term(auto&& ctx, auto src, auto& result) const {
		if constexpr (!is_term<ind>()) return -1;
		else {
			auto cur = get<ind>(seq).parse(ctx, src, cur_result<ind>(result));
			if(0 <= cur) return cur;
			if constexpr (ind==0) return cur;
			else return parse_term<ind-1>(ctx, src, result);
		}
	}
	template<auto ind, auto stop_pos>
	constexpr auto parse_nonterm(auto&& ctx, auto src, auto& result, auto shift) const {
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
				auto cur = copy_result(result);
				if constexpr (!std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>)
					search_in_ctx<rvariant_copied_result_tag>(ctx) = &cur;
				src += get<ind>(seq).parse(ctx, src, cur_result<ind>(result));
				pr = get<ind>(seq).parse(ctx, src, result_for_check);
			}
			auto total_shift = shift + (prev_pr*(prev_pr>0));
			if constexpr (ind==0) return total_shift;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, total_shift);
		}
	}
	template<auto stop_pos>
	constexpr auto parse_without_prep(auto&& ctx, auto src, auto& result) const {
		auto term_r = parse_term<sizeof...(parsers)-1>(ctx, src, result);
		if(term_r < 0) return term_r;
		auto nonterm_r = parse_nonterm<sizeof...(parsers)-1, stop_pos>(ctx, src += term_r, result, 0);
		return term_r + (nonterm_r*(nonterm_r>0));
	}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (exists_in_ctx<rvariant_stack_tag>(decltype(auto(ctx)){}))
			return parse_without_prep<0>(ctx, src, result);
		else {
			using copied_result_type = decltype(copy_result(result));
			auto nctx =
				make_ctx<rvariant_copied_result_tag>((copied_result_type*)nullptr,
				make_ctx<rvariant_stack_tag>(this, ctx));
			return parse_without_prep<0>(make_ctx<rvariant_crop_ctx_tag>(&nctx, nctx), src, result);
		}
	}
};

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
	static_assert( var_with_skip.b.template is_term<0>() );
	static_assert( var_with_skip.b.template is_term<1>() );
	return r;
}
template<typename dbl_expr>
constexpr static auto test_rvariant_val(auto r, auto&& maker, auto pr, auto&& src) {
	constexpr auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	auto var = rv(std::forward<decltype(maker)>(maker)
		, cast<dbl_expr>(rv_lreq++ >> _char<'+'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> _char<'-'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> _char<'*'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> _char<'/'> >> rv_rreq(rmaker))
		, int_
		, fp
		, quoted_string
		, rv_result(_char<'('> >> rv_req >> _char<')'>)
		) ;
	auto cr = var.parse(make_test_ctx(), make_source(src), r);
	cr /= (cr == pr);

	auto var_with_skip = inject_skipping(var, +space);
	static_assert( !var_with_skip.b.template is_term<0>() );
	static_assert( !var_with_skip.b.template is_term<1>() );
	static_assert( !var_with_skip.b.template is_term<2>() );
	static_assert( !var_with_skip.b.template is_term<3>() );
	static_assert( var_with_skip.b.template is_term<4>() );
	static_assert( var_with_skip.b.template is_term<5>() );
	static_assert( var_with_skip.b.template is_term<6>() );
	static_assert( var_with_skip.b.template is_term<7>() );

	return r;
}

constexpr static bool test_rvariant_dexpr() {
	struct expr_rt;
	using term_rt = factory_t::template variant<int,decltype(mk_str())>;
	struct dbl_expr{
		factory_t::template unique_ptr<expr_rt> left;
		factory_t::template unique_ptr<expr_rt> right;
	};
	struct mul_expr : dbl_expr {} ;
	struct min_expr : dbl_expr {} ;
	struct pls_expr : dbl_expr {} ;
	struct div_expr : dbl_expr {} ;
	struct expr_rt : factory_t::variant<pls_expr, min_expr, mul_expr, div_expr, int, double, decltype(mk_str())> {};
	constexpr auto pls_ind = 0;
	constexpr auto mul_ind = 2;
	constexpr auto int_ind = 4;
	constexpr auto fp_ind = int_ind+1;

	constexpr auto maker = [](auto& r){ return typename factory_t::template unique_ptr<expr_rt>( new expr_rt{std::move(r)} ); };
	static_assert( get<int_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "123")) == 123 );
	static_assert( get<fp_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "0.5")) == 0.5 );
	static_assert( get<int_ind>(*get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1*5")).left) == 1 );
	static_assert( get<int_ind>(*get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1*5")).right) == 5 );
	static_assert( get<int_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).right) == 7 );
	static_assert( get<int_ind>(*get<mul_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).left).left) == 1 );
	static_assert( get<int_ind>(*get<mul_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).left).right) == 5 );
	static_assert( get<int_ind>(*get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)")).left) == 1 );
	static_assert( get<int_ind>(*get<pls_ind>(*get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)")).right).left) == 5 );
	static_assert( get<int_ind>(*get<pls_ind>(*get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)")).right).right) == 7 );

	return true;
}

constexpr static bool test_rvariant() {
	static_assert( test_rvariant_simple(int{}, "123", int_, fp) == 123 );
	static_assert( test_rvariant_simple(double{}, "1.2", int_, fp) == 1.2 );
	return test_rvariant_dexpr();
	return true;
}
