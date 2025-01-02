#include "ascip.hpp"

#include <tuple>
#include <memory>
#include <variant>
#include <iostream>

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

using parser = ascip<ascip_details::inner_tuple>;
constexpr auto mk_neasted() {
	auto mk = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	return rv( [](auto& r){ return std::unique_ptr<neasted_expr>( new neasted_expr{std::move(r)} ); }
	, cast<neasted_binary>( parser::rv_lreq >> ++parser::_char<'+'> >> parser::rv_rreq(mk) )
	, cast<neasted_binary>( parser::rv_lreq >> ++parser::_char<'-'> >> parser::rv_rreq(mk) )
	, parser::int_
	, rv_result( parser::_char<'('> >> parser::rv_req<0> >> parser::_char<')'> )
	);
}
constexpr auto mk_outter() {
	auto mk = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	return rv( [](auto& r){ return std::unique_ptr<outter_expr>( new outter_expr{std::move(r)} ); }
	, cast<outter_binary>( parser::rv_lreq >> ++parser::_char<'+'> >> parser::rv_rreq(mk) )
	, cast<outter_binary>( parser::rv_lreq >> ++parser::_char<'-'> >> parser::rv_rreq(mk) )
	, parser::_char<'{'> >> mk_neasted() >> parser::_char<'}'>
	, parser::char_<'a'>
	, rv_result( parser::_char<'('> >> parser::rv_req<0> >> parser::_char<')'> )
	);
}

static_assert( mk_outter().is_term<2>() );
#ifndef __clang__
static_assert( []{ outter_expr r; return parse(mk_outter(), +parser::space, parser::make_source("a+a"), r); }() == 3 );
static_assert( []{ neasted_expr r; return parse(mk_neasted(), +parser::space, parser::make_source("1+2"), r); }() == 3 );
static_assert( []{ outter_expr r; return parse(mk_outter(), +parser::space, parser::make_source("a+{1+2}"), r); }() == 7 );
#endif

int main(int,char**) {
	return 0;
}
