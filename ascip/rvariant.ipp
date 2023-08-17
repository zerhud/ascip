#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


struct rvariant_stack_tag {};
struct rvariant_copied_result_tag {};
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
} rlreq{};
constexpr static struct rvariant_rreq_parser : base_parser<rvariant_rreq_parser> {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto* var = search_in_ctx<rvariant_stack_tag>(ctx);
		return var->parse(ctx, src, result);
	}
} rrreq{};
template<ascip_details::parser parser>
struct rvariant_term_parser : base_parser<rvariant_term_parser<parser>> { parser p; };

template<typename maker_type, ascip_details::parser... parsers>
struct rvariant_parser : base_parser<rvariant_parser<parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;
	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	template<auto ind> constexpr static bool is_term() {
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		return ascip_details::is_specialization_of<cur_parser_t, rvariant_term_parser>;
	}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const requires ( ascip_details::is_in_concept_check(decltype(ctx){}) ) {
		return 0;
	}
	template<auto ind> constexpr static auto& cur_result(auto& result) {
		if constexpr (requires{ create<1>(result); } ) return create<ind>(result);
		else if constexpr (requires{ result.template emplace<1>(); } ) return result.template emplace<ind>();
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
	template<auto ind>
	constexpr auto parse_nonterm(auto&& ctx, auto src, auto& result) const {
		if constexpr (is_term<ind>()) {
			if constexpr (ind == 0) return 0;
			else return parse_nonterm<ind-1>(ctx, src, result);
		}
		else {
			ascip_details::type_any_eq_allow result_for_check;
			auto pr = get<ind>(seq).parse(ctx, src, result_for_check);
			auto prev_pr = pr;
			while(0 < pr) {
				auto cur = copy_result(result);
				auto prev_pr = pr;
				if constexpr (!std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>)
					search_in_ctx<rvariant_copied_result_tag>(ctx) = &cur;
				src += get<ind>(seq).parse(ctx, src, cur_result<ind>(result));
				pr = get<ind>(seq).parse(ctx, src, result_for_check);
			}
			if constexpr (ind==0) return prev_pr;
			else {
				if(0 <= pr) return pr;
				else return parse_nonterm<ind-1>(ctx, src, result);
			}	
		}
	}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto term_r = parse_term<sizeof...(parsers)-1>(ctx, src, result);
		if(term_r < 0) return term_r;
		auto nonterm_r = term_r;
		if constexpr (exists_in_ctx<rvariant_stack_tag>(decltype(auto(ctx)){}))
			nonterm_r = parse_nonterm<sizeof...(parsers)-1>(ctx, src += term_r, result);
		else  {
			using copied_result_type = decltype(copy_result(result));
			auto nctx = make_ctx<rvariant_copied_result_tag>(
					(copied_result_type*)nullptr,
					make_ctx<rvariant_stack_tag>(this, ctx));
			nonterm_r = parse_nonterm<sizeof...(parsers)-1>(nctx, src += term_r, result);
		}
		return term_r + (nonterm_r*(nonterm_r>0));
	}
};

constexpr static auto test_rvariant_simple(auto r, auto&& src, auto&&... parsers) {
	auto var = lrexpr([](auto& r){return &r;}, parsers...);
	var.parse(make_test_ctx(), make_source(src), r);
	static_assert( var.template is_term<0>() );
	static_assert( var.template is_term<1>() );
	return r;
}
template<typename dbl_expr>
constexpr static auto test_rvariant_val(auto r, auto&& maker, auto pr, auto&& src) {
	constexpr auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	auto cr = lrexpr(std::forward<decltype(maker)>(maker)
		, cast<dbl_expr>(rlreq++ >> _char<'+'> >> rrreq(rmaker))
		, cast<dbl_expr>(rlreq++ >> _char<'*'> >> rrreq(rmaker))
		, lrterm(int_)
		, lrterm(fp)
		, lrterm(quoted_string)
		).parse(make_test_ctx(), make_source(src), r);
	cr /= (cr == pr);
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
	struct pls_expr : dbl_expr {} ;
	struct expr_rt : factory_t::variant<pls_expr, mul_expr, int, double, decltype(mk_str())> {};
	constexpr auto pls_ind = 0;
	constexpr auto mul_ind = 1;
	constexpr auto int_ind = 2;
	constexpr auto fp_ind = 3;

	constexpr auto maker = [](auto& r){ return typename factory_t::template unique_ptr<expr_rt>( new expr_rt{std::move(r)} ); };
	static_assert( get<int_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "123")) == 123 );
	static_assert( get<fp_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "0.5")) == 0.5 );
	static_assert( get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1*5")).left.get() != nullptr );
	static_assert( get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1*5")).right.get() != nullptr );

	return true;
}

constexpr static bool test_rvariant() {
	static_assert( test_rvariant_simple(int{}, "123", lrterm(int_), lrterm(fp)) == 123 );
	static_assert( test_rvariant_simple(double{}, "1.2", lrterm(int_), lrterm(fp)) == 1.2 );
	return test_rvariant_dexpr();
	return true;
}
