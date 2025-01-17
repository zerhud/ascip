//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <memory>
#include <variant>

#include "factory.hpp"

constexpr auto& int_ = p::int_;
constexpr auto& rv_lreq = p::rv_lreq;
constexpr auto& rv_rreq = p::rv_rreq;

template<typename dbl_expr>
constexpr auto test_rvariant_val(auto r, auto&& maker, auto pr, auto&& src) {
	constexpr auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	auto var = rv(std::forward<decltype(maker)>(maker)
		, cast<dbl_expr>(rv_lreq++ >> _char<'+'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> _char<'-'> >> rv_rreq(rmaker))
		, int_
		, cast<dbl_expr>(rv_lreq++ >> _char<'*'> >> rv_rreq(rmaker)) | cast<dbl_expr>(rv_lreq++ >> _char<'/'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> lit<"**"> >> rv_rreq(rmaker))
		, p::fp
		, p::quoted_string
		, rv_result(_char<'('> >> p::template rv_req<0> >> _char<')'>)
		) ;
	auto cr = var.parse(make_test_ctx(), make_source(src), r);
	cr /= (cr == pr);

	auto var_with_skip = p::inject_skipping(var, +p::space);
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

constexpr bool test_rvariant_dexpr() {
	struct expr_rt;
	using term_rt = std::variant<int,std::string>;
	struct dbl_expr{
		std::unique_ptr<expr_rt> left;
		std::unique_ptr<expr_rt> right;
	};
	struct mul_expr : dbl_expr {} ;
	struct min_expr : dbl_expr {} ;
	struct pls_expr : dbl_expr {} ;
	struct div_expr : dbl_expr {} ;
	struct pow_expr : dbl_expr {} ;
	struct expr_rt : std::variant<
		 pls_expr, min_expr,
		 int,
		 std::variant<mul_expr, div_expr>,
		 pow_expr,
		 double, std::string
	> {};
	constexpr auto pls_ind = 0;
	constexpr auto mul_ind = 3;
	constexpr auto pow_ind = 4;
	constexpr auto int_ind = 2;
	constexpr auto fp_ind = 5;

	constexpr auto maker = [](auto& r){ return std::unique_ptr<expr_rt>( new expr_rt{std::move(r)} ); };
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

struct neasted_expr;
using neasted_fwd = std::unique_ptr<neasted_expr>;
struct neasted_binary {
	neasted_fwd left;
	neasted_fwd right;
};

struct neasted_a : neasted_binary {};
struct neasted_b : neasted_binary {};
struct neasted_expr : std::variant< neasted_a, neasted_b, int > {};

struct outter_expr;
using outter_fwd = std::unique_ptr<outter_expr>;
struct outter_binary {
	outter_fwd left;
	outter_fwd right;
};

struct outter_a : outter_binary {};
struct outter_b : outter_binary {};

struct outter_expr : std::variant< outter_a, outter_b, neasted_expr, char > {};

using parser = ascip;
constexpr auto mk_neasted() {
	auto mk = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	return rv( [](auto& r){ return std::unique_ptr<neasted_expr>( new neasted_expr{std::move(r)} ); }
	, cast<neasted_binary>( parser::rv_lreq >> ++def(parser::_char<'+'>) >> parser::rv_rreq(mk) )
	, cast<neasted_binary>( parser::rv_lreq >> ++def(parser::_char<'-'>) >> parser::rv_rreq(mk) )
	, parser::int_
	, rv_result( def(parser::_char<'('>) >> parser::rv_req<0> >> parser::_char<')'> )
	);
}
constexpr auto mk_outter() {
	auto mk = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	return rv( [](auto& r){ return std::unique_ptr<outter_expr>( new outter_expr{std::move(r)} ); }
	, cast<outter_binary>( parser::rv_lreq >> ++parser::_char<'+'> >> parser::rv_rreq(mk) )
	, cast<outter_binary>( parser::rv_lreq >> ++parser::_char<'-'> >> parser::rv_rreq(mk) )
	, def(parser::_char<'{'>) >> mk_neasted() >> parser::_char<'}'>
	, parser::char_<'a'>
	, rv_result( def(parser::_char<'('>) >> parser::rv_req<0> >> parser::_char<')'> )
	);
}

static_assert( mk_outter().is_term<2>() );
#ifndef __clang__
static_assert( []{ outter_expr r; return parse(mk_outter(), +parser::space, parser::make_source("a+a"), r); }() == 3 );
static_assert( []{ neasted_expr r; return parse(mk_neasted(), +parser::space, parser::make_source("1+2+3"), r); }() == 5 );
static_assert( []{ outter_expr r; return parse(mk_outter(), +parser::space, parser::make_source("a+{1+2}"), r); }() == 7 );
#endif

int main(int,char**) {
	return 0;
}
