#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<auto pos_in_variant>
constexpr static auto parse_next_variant(auto&& ctx, auto src, auto& result) {
	auto* next_variant = search_in_ctx<variant_stack_tag>(ctx);
	return next_variant->template parse_from<pos_in_variant+1>(ctx, src, result);
}

template<auto pos_in_variant, auto element_in_seq>
struct lreq_parser : base_parser<lreq_parser<pos_in_variant,element_in_seq>> {

	struct lreq_parser_org_src {};
	struct lreq_parser_req_need_count {};

	constexpr auto parse(auto&& ctx,auto,auto&) const requires (
		   ascip_details::is_in_concept_check(decltype(auto(ctx)){}) ){ return 0; }
	template<auto element>
	constexpr auto parse_current_parser(auto shift, auto&& ctx, auto src, auto& result) const {
		auto* next_variant = search_in_ctx<variant_stack_tag>(ctx);
		src += shift;
		if(!src) return -1;
		return next_variant->template parse_parse_from_only<pos_in_variant, element>(ctx, src, result);
	}
	constexpr auto count_reqursion_expressions(auto start_pos, auto&& ctx, auto src, auto& result) const {
			auto cur = 0;
			auto expr_count = 0;
			decltype(cur) acc = start_pos;
			while(0 <= cur) {
				++expr_count;
				acc += cur;
				cur = parse_current_parser<element_in_seq+1>(acc, ctx, src, result);
			}
			// cur == 0: at least one iteration, with just next parser
			return expr_count-1;
	}
	//TODO: implement for copiable result type - it will be faster, maybe
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (exists_in_ctx<lreq_parser_req_need_count>(decltype(auto(ctx)){})) {
			auto need_iter_count = --search_in_ctx<lreq_parser_req_need_count>(ctx);
			auto expr_start = *search_in_ctx<lreq_parser_org_src>(ctx);
			if(need_iter_count == 0) return parse_next_variant<pos_in_variant>(ctx, expr_start, result);
			return parse_current_parser<0>(0, ctx, expr_start, result);
		}
		else {
			auto shift_next_parser = parse_next_variant<pos_in_variant>(ctx, src, result);
			if(shift_next_parser < 0) return shift_next_parser;

			auto req_call_count = count_reqursion_expressions(shift_next_parser, ctx, src, result)-1;
			if(req_call_count<=0) return parse_next_variant<pos_in_variant>(ctx, src, result);

			auto nctx = make_ctx<lreq_parser_org_src>(&src,
			  make_ctx<lreq_parser_req_need_count>(req_call_count, ctx)
			);
			return parse_current_parser<0>(0, nctx, src, result);
		}
	}
};
template<auto pos_in_variant> constexpr static auto lreq = lreq_parser<pos_in_variant,0>{};

template<auto pos_in_variant> struct lrreq_parser : base_parser<lrreq_parser<pos_in_variant>> {
	constexpr auto parse(auto&& ctx,auto,auto&) const requires (
		   ascip_details::is_in_concept_check(decltype(auto(ctx)){}) ){ return 0; }
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		return parse_next_variant<pos_in_variant>(ctx, src, result);
	}
};
template<auto pos_in_variant> constexpr static auto lrreq = lrreq_parser<pos_in_variant>{};

constexpr static auto test_lreq_mk_result() {
	using term_rt = factory_t::template variant<int,decltype(mk_str())>;
	struct expr_rt;
	struct dbl_expr {
		factory_t::template unique_ptr<expr_rt> left;
		int opcode=5000;
		factory_t::template unique_ptr<term_rt> right;
	};
	struct expr_rt : factory_t::variant<dbl_expr, term_rt> {};

	return expr_rt{};
}
constexpr static auto test_lreq_mk_result_wm() {
	using term_rt = factory_t::template variant<int,decltype(mk_str())>;
	struct expr_rt;
	struct mul_expr {
		factory_t::template unique_ptr<expr_rt> left;
		int opcode=5000;
		factory_t::template unique_ptr<expr_rt> right;
	};
	struct plus_expr {
		factory_t::template unique_ptr<expr_rt> left;
		int opcode=5000;
		factory_t::template unique_ptr<expr_rt> right;
	};
	struct expr_rt : factory_t::variant<plus_expr, mul_expr, term_rt> {};

	return expr_rt{};
}
constexpr static bool test_lreq(auto&& src, auto rw, auto rs, auto checker) {
	auto result_maker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	auto ident = alpha >> *(alpha|d10|char_<'_'>);
	auto term = int_ | ident;
	auto expr =
		  ((lreq<0>(result_maker))++ >> as(_char<'+'>, 0)++ >> term(result_maker))
		| term
		;

	auto result = test_lreq_mk_result();
	auto result_ss = test_lreq_mk_result();
	auto r = expr.parse(make_test_ctx(), make_source(src), result);
	r /= (r==rw);
	r = parse(expr, +space, make_source(src), result_ss);
	r /= (r==rs);

	checker(result_ss);

	return true;
}
constexpr static bool test_lreq_wm(auto&& src, auto rw, auto rs, auto checker) {
	auto result_maker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	auto ident = alpha >> *(alpha|d10|char_<'_'>);
	auto term = int_ | ident;
	auto expr =
		  ((lreq<0>(result_maker))++ >> as(_char<'+'>, 0)++ >> lrreq<0>(result_maker))
		| ((lreq<1>(result_maker))++ >> as(_char<'*'>, 1)++ >> lrreq<1>(result_maker))
		| term
		;

	auto result = test_lreq_mk_result_wm();
	auto result_ss = test_lreq_mk_result_wm();
	auto r = expr.parse(make_test_ctx(), make_source(src), result);
	r /= (r==rw);
	r = parse(expr, +space, make_source(src), result_ss);
	r /= (r==rs);

	checker(result_ss);

	return true;
}

constexpr static void test_11p2(const auto& r) {
	get<0>(get<1>(*get<0>(r).left)) / (get<0>(get<1>(*get<0>(r).left))==11);
	get<0>(r).opcode / (get<0>(r).opcode==0);
	get<0>(*get<0>(r).right) / (get<0>(*get<0>(r).right)==2);
}
constexpr static void test_11p2p3(const auto& r) {
	test_11p2(*get<0>(r).left);
	get<0>(r).opcode / (get<0>(r).opcode==0);
	get<0>(*get<0>(r).right) / (get<0>(*get<0>(r).right)==3);
}
constexpr static bool test_lreq() {

	static_assert( test_lreq("", -1, -1, [](const auto&){}) );
	static_assert( test_lreq("1", 1, 1, [](const auto& r){get<0>(get<1>(r)) / (get<0>(get<1>(r))==1);}) );
	static_assert( test_lreq("11+2", 4, 4, [](const auto& r){test_11p2(r);}) );
	static_assert( test_lreq("11 + 2", 2, 6, [](const auto& r){test_11p2(r);}) );
	static_assert( test_lreq("11 + 2 + 3", 2, 10, [](const auto& r){test_11p2p3(r);}) );
	static_assert( test_lreq("11 + 2 + 3 + ok", 2, 15, [](const auto& r){
		test_11p2p3(*get<0>(r).left);
		get<0>(r).opcode / (get<0>(r).opcode==0);
		test_cmp_vec(get<1>(*get<0>(r).right), 'o', 'k');
	}) );

	static_assert( test_lreq_wm("", -1, -1, [](const auto&){}) );
	static_assert( test_lreq_wm("1", 1,  1, [](const auto&){}) );

	return true;
}
