#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<ascip_details::parser pt> struct lexeme_parser : base_parser<lexeme_parser<pt>> { [[no_unique_address]] pt p; 
	constexpr auto parse(auto&& ctx, auto src, auto& result) const { return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result); }
};
template<ascip_details::parser pt> struct skip_parser : base_parser<lexeme_parser<pt>> { [[no_unique_address]] pt p; 
	constexpr auto parse(auto&& ctx, auto src, auto& result) const { return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result); }
};
template<ascip_details::parser skip, ascip_details::parser base> struct injected_parser : base_parser<injected_parser<skip,base>> {
	[[no_unique_address]] skip s;
	[[no_unique_address]] base b;
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow skip_result;
		auto sr = s.parse(ctx, src, skip_result);
		sr *= (0<=sr);
		src += sr;
		auto mr = b.parse(ctx, src, result);
		return (sr * (0<=mr)) + mr; // 0<=mr ? mr+sr : mr;
	}

	constexpr auto parse_with_user_result(auto&& ctx, auto src, auto& result) const
	requires requires(const base& p){ p.parse_with_user_result(ctx, src, result); } {
		return b.parse_with_user_result(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}
};

template<typename skip_type>
struct injection_mutator {
	struct context{ };
	struct in_lexeme_context{ };
	constexpr static auto create_ctx(){ return context{}; }
	constexpr static auto create_ctx(auto&& src, auto&& ctx) {
		if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, lexeme_parser>)
			return in_lexeme_context{ };
		else if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, skip_parser>)
			return context{};
		else return ctx;
	}

	template<typename type>
	constexpr static auto apply(auto&& p, auto& ctx) {
		constexpr const bool is_inside_lexeme = std::is_same_v<in_lexeme_context,std::decay_t<decltype(ctx)>>;
		constexpr const bool is_parser_lexeme = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, lexeme_parser>;
		constexpr const bool is_parser_skip = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, skip_parser>;

		constexpr const bool is_parser_variant = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, variant_parser>;
		constexpr const bool is_parser_blist = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, binary_list_parser>;
		constexpr const bool is_parser_diff = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, different_parser>;
		constexpr const bool is_opt_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, opt_seq_parser>;

		constexpr const bool is_parser_for_skip =
			   is_opt_seq_parser
			|| is_parser_variant
			|| is_parser_blist
			|| is_parser_diff
			;

		if constexpr (is_parser_lexeme)
			return injected_parser<skip_type, std::decay_t<decltype(p.p)>>{{}, skip_type{}, std::move(p.p)};
		else if constexpr (is_inside_lexeme || is_parser_for_skip) return p;
		else if constexpr (is_parser_skip) return p.p;
		else if constexpr ( requires{ p.p; }) return p;
		else return injected_parser<skip_type, std::decay_t<decltype(p)>>{{}, skip_type{}, std::move(p)};
	}
};

template<auto unused, ascip_details::parser parser, ascip_details::parser skipper>
constexpr static auto inject_skipping(parser&& to, skipper&& what) {
	using mutator = injection_mutator<std::decay_t<skipper>>;
	return transform<mutator>(std::move(to));
}
template<ascip_details::parser p1, ascip_details::parser p2> constexpr static auto make_injected(const p1& l, const p2& r) {
	return typename p1::holder::template injected_parser<p1, p2>{ {}, l, r }; }
/*

template<ascip_details::parser p1, ascip_details::parser p2> constexpr static auto make_injected(const p1& l, const p2& r) {
	return typename decltype(auto(l))::holder::template injected_parser<p1, p2>{ {}, l, r }; }

template<bool apply, auto... inds, template<typename...>class result_t, ascip_details::parser... parsers>
constexpr static auto inject_skipping_seq(const result_t<parsers...>& to, const auto& what, auto&&... args) {
	if constexpr (sizeof...(inds) == sizeof...(parsers))
		return result_t<decltype(auto(args))...>( std::forward<decltype(args)>(args)... );
	else return inject_skipping_seq<apply, inds..., sizeof...(inds)>(
			to, what, std::forward<decltype(args)>(args)...,
			inject_skipping<apply>(get<sizeof...(inds)>( to.seq ), what)
	);
}

template<bool apply> constexpr static auto inject_skipping(const auto& to, const auto& what) {
	if constexpr (apply) return make_injected(what, to);
	else return to; };
template<bool apply, ascip_details::parser... parsers> constexpr static auto
inject_skipping(const opt_seq_parser<parsers...>& to, const auto& what) { return inject_skipping_seq<apply>(to, what); }
template<bool apply, auto val, ascip_details::parser type> constexpr static auto
inject_skipping(const _seq_inc_rfield_val<val, type>& p, const auto& s) {
	return  finc<val>(inject_skipping<apply>(static_cast<const type&>(p), s)); }
template<bool apply, ascip_details::parser type> constexpr static auto
inject_skipping(const lexeme_parser<type>& to, const auto& what) {
	return injected_parser{ {}, what, inject_skipping<false>(to.p, what) }; }
template<bool apply, ascip_details::parser type> constexpr static auto
inject_skipping(const skip_parser<type>& to, const auto& what) { return inject_skipping<true>(to.p, what); }
template<bool apply, ascip_details::parser... parsers> constexpr static auto
inject_skipping(const variant_parser<parsers...>& to, const auto& what) { return inject_skipping_seq<apply>(to, what); }
template<bool apply, typename tag, ascip_details::parser type> constexpr static auto
inject_skipping(const result_checker_parser<tag, type>& p, const auto& s) {
	return check<tag>( inject_skipping<apply>(p.p, s) ); }
template<bool apply, typename tag, ascip_details::parser type> constexpr static auto inject_skipping(const cast_parser<tag, type>& p, const auto& s) {
	return cast<tag>( inject_skipping<apply>(p.p, s) ); }
template<bool apply, template<typename>class wrapper_type, ascip_details::parser wrapped_type> constexpr static auto
inject_skipping(const wrapper_type<wrapped_type>& to, const auto& what) {
	if constexpr (requires{ wrapper_type{ {}, inject_skipping<apply>(to.p, what) }; })
		return wrapper_type{ {}, inject_skipping<apply>(to.p, what) };
	else
		return wrapper_type{ inject_skipping<apply>(static_cast<const wrapped_type&>(to), what) };
}
template<bool apply, template<typename,typename>class wrapper_type, ascip_details::parser wrapped_type1, ascip_details::parser wrapped_type2> constexpr static auto
inject_skipping(const wrapper_type<wrapped_type1, wrapped_type2>& to, const auto& what)
requires (
  !requires{ static_cast<const opt_seq_parser<wrapped_type1,wrapped_type2>&>(to); } &&
  !requires{ static_cast<const variant_parser<wrapped_type1,wrapped_type2>&>(to); }) {
	return wrapper_type{ inject_skipping<apply>(ascip_reflection::get<0>(to), what), inject_skipping<apply>(ascip_reflection::get<1>(to), what) }; }
	*/

constexpr static bool test_injection_parser() {
	static_assert( ({ char r='z'; make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source(" a"), r);
	}) == 2, "inejction parser can parse");
	static_assert( ({ char r='z'; make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source("  b"), r);
	}) == -1, "inejction parser cannot parse invalid data");
	static_assert( ({ char r='z';
		make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source(" a"), r);
	r;}) == 'a', "the first parameter of inejction parser skipped");
	static_assert( ({ char r='z';
		make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source("ba"), r);
	}) == -1, "inejction parser fails if cannot parse first parameter");
	static_assert( ({ char r='z';
		auto pr=make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source("a"), r);
	(pr==1) + (2*(r=='a'));}) == 3, "inejction parser parse if only first parameter fails");
	static_assert( ({ char r='z';
		auto pr=make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source("aa"), r);
	(pr==1) + (2*(r=='a'));}) == 3, "inejction parser parse if only first parameter fails");
	return true;
}
template<auto p1, auto p2>
constexpr static bool test_seq_injection() {
	using p1_t = decltype(auto(p1));
	using p2_t = decltype(auto(p2));
	using inj_t = injected_parser<p2_t,p1_t>;

	//static_cast<const decltype(p1)&>(inject_skipping<false>(p1, p2));
	static_cast<const inj_t&>(inject_skipping<true>(p1, p2));
	//static_cast<const opt_seq_parser<p1_t, p1_t>&>(inject_skipping<false>(p1 >> p1, p2));
	static_cast<const opt_seq_parser<inj_t, inj_t>&>(inject_skipping<true>(p1 >> p1, p2));

	static_cast<const injected_parser<p2_t,opt_seq_parser<p1_t, p1_t, p1_t>>&>(inject_skipping<true>(lexeme(p1 >> p1 >> p1), p2));
	static_cast<const injected_parser<p2_t,opt_seq_parser<p1_t, opt_seq_parser<inj_t, inj_t>>>&>(
			inject_skipping<true>(lexeme(p1 >> skip(p1 >> p1)), p2));

	static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping<true>( p1|p1, p2 ));
	static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping<true>( p1|p1|p1, p2 ));
	static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		cast_parser<char,inj_t>
		>&>(inject_skipping<true>( check<char>(p1)|cast<char>(p1), p2 ));

	static_cast<const unary_list_parser<inj_t>&>(inject_skipping<true>( +p1, p2 ));
	static_cast<const unary_list_parser<opt_seq_parser<inj_t,inj_t>>&>(inject_skipping<true>( +(p1>>p1), p2 ));

	static_cast<const opt_parser<inj_t>&>(inject_skipping<true>( -p1, p2 ));
	static_cast<const opt_parser<opt_seq_parser<inj_t,inj_t>>&>(inject_skipping<true>( -(p1>>p1), p2 ));

	static_cast<const different_parser<inj_t, inj_t>&>(inject_skipping<true>( p1 - p1, p2 ));
	static_cast<const opt_seq_parser<
		inj_t,
		different_parser<
		  opt_seq_parser<inj_t,inj_t>,
		  inj_t
		>
		>&>(inject_skipping<true>( p1 >> (p1>>p1) - p1, p2 ));

	static_assert( ({ char r='z'; int t=0;
		auto p = char_<'a'> >> char_<'b'> >> [&t](...){++t;return 0;};
		p.parse(make_test_ctx(), make_source("ab"), r);
	t;}) == 1, "injection works win lambda in seq");
	static_assert( ({ char r='z'; int t=0;
		auto p = char_<'a'> >> char_<'b'>([&t](...){++t;});
		p.parse(make_test_ctx(), make_source(mk_str("ab")), r);
	t; }) == 1, "injection works with semact" );
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping<true>(char_<'a'> >> char_<'b'>([&t](...){++t;}), +space);
		p.parse(make_test_ctx(), make_source(mk_str("ab")), r);
	t; }) == 1, "injection works with semact" );
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping<true>(char_<'a'> >> cast<char>(char_<'b'>([&t](...){++t;})), +space);
		p.parse(make_test_ctx(), make_source(mk_str("abc")), r);
	t; }) == 1, "injection works with semact" );

	static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping<true>( p1|p1, p2 ));
	static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping<true>( p1|p1|p1, p2 ));
	static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		result_checker_parser<char,inj_t>
		>&>(inject_skipping<true>( check<char>(p1)|check<char>(p1), p2 ));

	static_cast<const result_checker_parser<int, inj_t>&>(inject_skipping<true>( check<int>(p1), p2 ));
	static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,inj_t>>&>(inject_skipping<true>( check<int>(p1 >> p1), p2 ));

	static_assert( ({ char r; const auto parser = +alpha; const auto skipper = +space;
		inject_skipping<true>(parser, skipper).parse(make_test_ctx(), make_source(" a b c "), r); }) == -1 );
	static_cast<const binary_list_parser<inj_t, inj_t>&>(inject_skipping<true>( p1 % p1, p2 ));

	return true;
}
constexpr static bool test_injection() {
	return test_injection_parser() && test_seq_injection<char_<'a'>,space>();
}
