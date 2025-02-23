#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "injection/parsers.hpp"
#include "injection/mutator.hpp"

namespace ascip_details {

template<parser parser_type, parser skipper>
constexpr auto inject_skipping(parser_type&& to, skipper&& what) {
	using mutator = prs::injection_mutator<std::decay_t<skipper>>;
	auto ret = transform<mutator>(std::move(to));
	if constexpr(ascip_details::is_specialization_of<std::decay_t<decltype(ret)>, prs::injected_parser>)
		return ret.b;
	else return ret;
}

template<parser p1, parser p2> constexpr auto make_injected(const p1& l, const p2& r) {
	return prs::injected_parser<p1, p2>( l, r );
}

namespace prs {
template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_injection_parser() {
	constexpr auto cs = t<' '>::char_;
	constexpr auto ca = t<'a'>::char_;
#ifndef __clang__
	static_assert( ({ char r='z'; make_injected(cs, ca).parse(make_test_ctx(), make_source(" a"), r);
	}) == 2, "inejction parser can parse");
	static_assert( ({ char r='z'; make_injected(cs, ca).parse(make_test_ctx(), make_source("  b"), r);
	}) == -1, "inejction parser cannot parse invalid data");
	static_assert( ({ char r='z';
		make_injected(cs, ca).parse(make_test_ctx(), make_source(" a"), r);
	r;}) == 'a', "the first parameter of inejction parser skipped");
	static_assert( ({ char r='z';
		make_injected(cs, ca).parse(make_test_ctx(), make_source("ba"), r);
	}) == -1, "inejction parser fails if cannot parse first parameter");
	static_assert( [&]{ char r='z';
		auto pr=make_injected(cs, ca).parse(make_test_ctx(), make_source("a"), r);
		return (pr==1) + (2*(r=='a'));}() == 3, "inejction parser parse if only first parameter fails");
	static_assert( [&]{ char r='z';
		auto pr=make_injected(cs, ca).parse(make_test_ctx(), make_source("aa"), r);
		return (pr==1) + (2*(r=='a'));}() == 3, "inejction parser parse if only first parameter fails");
#endif
	return true;
}
template<typename a, auto p1, auto p2, template<auto>class tt=a::template tmpl>
constexpr static bool test_seq_injection() {
	//TODO: test something like skip(++lexeme(parser))([](auto& v){return &v;})
	constexpr auto cs = tt<' '>::char_;
	constexpr auto ca = tt<'a'>::char_;
	constexpr auto cb = tt<'b'>::char_;

	using p1_t = decltype(auto(p1));
	using p2_t = decltype(auto(p2));
	using inj_t = injected_parser<p2_t,p1_t>;

	(void)static_cast<const p1_t&>(inject_skipping(p1, p2));
	(void)static_cast<const opt_seq_parser<inj_t, inj_t>&>(inject_skipping(p1 >> p1, p2));
	(void)static_cast<const opt_seq_parser<inj_t, must_parser<"unknown", inj_t>>&>(inject_skipping(p1 > p1, p2));

	(void)static_cast<const opt_seq_parser<p1_t, p1_t, p1_t>&>(inject_skipping(lexeme(p1 >> p1 >> p1), p2));
	(void)static_cast<const opt_seq_parser<p1_t, p1_t, p1_t>&>(inject_skipping(lexeme(lexeme(p1 >> p1 >> p1)), p2));
	(void)static_cast<const opt_seq_parser<seq_inc_rfield_before<injected_parser<p2_t, opt_seq_parser<p1_t, p1_t>>>, inj_t>&>(inject_skipping(++lexeme(p1 >> p1) >> p1, p2));
	(void)static_cast<const opt_seq_parser<p1_t, opt_seq_parser<p1_t, p1_t>>&>(inject_skipping(lexeme(p1 >> lexeme(p1 >> p1)), p2));
	(void)static_cast<const opt_seq_parser<p1_t, injected_parser<p2_t, opt_seq_parser<p1_t, p1_t>>>&>(inject_skipping(lexeme(p1 >> skip(lexeme(p1 >> p1))), p2));
	(void)static_cast<const opt_seq_parser<p1_t, injected_parser<p2_t, opt_seq_parser<inj_t, inj_t>>>&>(inject_skipping(lexeme(p1 >> skip(lexeme(skip(p1 >> p1)))), p2));
	(void)static_cast<const opt_seq_parser<inj_t, injected_parser<p2_t, opt_seq_parser<p1_t, p1_t>>>&>(inject_skipping(lexeme(p1) >> lexeme(p1 >> p1), p2));
	(void)static_cast<const opt_seq_parser<p1_t, opt_seq_parser<inj_t, inj_t>>&>(
			inject_skipping(lexeme(p1 >> skip(p1 >> p1)), p2));

	constexpr auto lambda_ret_val = [](auto&v){return &v;};
	using lambda_ret_val_t = std::decay_t<decltype(lambda_ret_val)>;
	(void)static_cast<const semact_parser<injected_parser<p2_t, opt_seq_parser<p1_t, opt_seq_parser<inj_t, inj_t>>>, lambda_ret_val_t>&>(inject_skipping(lexeme(p1 >> skip(p1 >> lexeme(p1)))(lambda_ret_val), p2));

	(void)static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping( p1|p1, p2 ));
	(void)static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping( p1|p1|p1, p2 ));
	(void)static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		cast_parser<char,inj_t>
		>&>(inject_skipping( check<char>(p1)|cast<char>(p1), p2 ));

	(void)static_cast<const unary_list_parser<inj_t>&>(inject_skipping( +p1, p2 ));
	(void)static_cast<const unary_list_parser<opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( +(p1>>p1), p2 ));

	(void)static_cast<const opt_parser<inj_t>&>(inject_skipping( -p1, p2 ));
	(void)static_cast<const opt_parser<opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( -(p1>>p1), p2 ));

	(void)static_cast<const different_parser<inj_t, inj_t>&>(inject_skipping( p1 - p1, p2 ));
	(void)static_cast<const opt_seq_parser<
		inj_t,
		different_parser<
			opt_seq_parser<inj_t,inj_t>,
			inj_t
		>
		>&>(inject_skipping( p1 >> ((p1>>p1) - p1), p2 ));

#ifndef __clang__
	/* TODO: remove support of lambda in seq?
	static_assert( ({ char r='z'; int t=0;
		auto p = ca >> cb >> [&t](...){++t;return 0;};
		p.parse(make_test_ctx(), make_source("ab"), r);
	t;}) == 1, "injection works win lambda in seq");
	static_assert( ({ char r='z'; int t=0;
		auto p = ca >> cb([&t](...){++t;});
		p.parse(make_test_ctx(), make_source("ab"), r);
	t; }) == 1, "injection works with semact" );
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping(ca >> cb([&t](...){++t;}), +a::space);
		p.parse(make_test_ctx(), make_source("ab"), r);
	t; }) == 1, "injection works with semact" );
	*/
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping(ca >> cast<char>(cb([&t](...){++t;})), +a::space);
		p.parse(make_test_ctx(), make_source("abc"), r);
	t; }) == 1, "injection works with semact" );
#endif

	(void)static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping( p1|p1, p2 ));
	(void)static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping( p1|p1|p1, p2 ));
	(void)static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		result_checker_parser<char,inj_t>
		>&>(inject_skipping( check<char>(p1)|check<char>(p1), p2 ));

	(void)static_cast<const result_checker_parser<int, inj_t>&>(inject_skipping( check<int>(p1), p2 ));
	(void)static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( check<int>(p1 >> p1), p2 ));
	(void)static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,seq_num_rfield_val<inj_t, _seq_rfield_val<2>>>>&>(inject_skipping( check<int>(p1 >> fnum<2>(p1)), p2 ));
	{
		auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
		auto var = rv(rmaker, p1, p1);
		(void)static_cast<rvariant_parser<decltype(rmaker), p1_t, p1_t>&>(var);
		(void)static_cast<const rvariant_parser<decltype(rmaker), inj_t, inj_t>&>(inject_skipping(var, p2));
	}

#ifndef __clang__
	static_assert( ({ char r{}; const auto parser = +a::alpha; const auto skipper = +a::space;
		inject_skipping(parser, skipper).parse(make_test_ctx(), make_source(" a b c "), r); }) == -1 );
#endif
	(void)static_cast<const binary_list_parser<inj_t, inj_t>&>(inject_skipping( p1 % p1, p2 ));

	return true;
}

template<typename a>
constexpr static bool test_injection() {
	return test_injection_parser<a>() && test_seq_injection<a, a::template char_<'a'>, a::space>();
}

}
}
