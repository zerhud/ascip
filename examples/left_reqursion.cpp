#include <tuple>
#include <memory>
#include <string>
#include <variant>
#include <sstream>
#include <iostream>
#include "ascip.hpp"

using parser_without_tests = ascip<std::tuple>;
using terminal = std::variant<int,std::string>;

struct expr;
struct binary_expr {
	std::unique_ptr<expr> left;
	std::unique_ptr<expr> right;
};
struct ternary_expr {
	std::unique_ptr<expr> cond;
	std::unique_ptr<expr> if_true;
	std::unique_ptr<expr> if_false;
	std::ostream& print(std::ostream& o) const;
};
struct operator_plus : binary_expr {std::ostream& print(std::ostream& o) const;};
struct operator_minus : binary_expr {std::ostream& print(std::ostream& o) const;};
struct operator_multi : binary_expr {std::ostream& print(std::ostream& o) const;};
struct operator_divid : binary_expr {std::ostream& print(std::ostream& o) const;};
struct operator_extra : binary_expr {std::ostream& print(std::ostream& o) const;};
struct operator_power : binary_expr {std::ostream& print(std::ostream& o) const;};

//struct expr : std::variant<ternary_expr, operator_plus, operator_minus, operator_multi, operator_divid, operator_extra, operator_power, terminal>{};
struct expr : std::variant<operator_plus, operator_minus, operator_multi, operator_divid, operator_extra, terminal>{};

// let's print the resulting expression
template<typename type> concept printable = requires(std::ostream& o, const type& obj){ o << obj; };

template<template<typename...>class variant, printable... types>
std::ostream& operator<<(std::ostream& o, const variant<types...>& v){
	std::visit([&o](const auto& v){o << v;}, v);
	return o;
}
std::ostream& operator<<(std::ostream& o, const expr& e) {
	std::visit([&o](auto& v){
		if constexpr (requires{v.print(o);}) v.print(o);
		else o << v;
		}, e);
	return o;
}
std::ostream& operator<<(std::ostream& o, const std::unique_ptr<expr>& e) { 
	if(e) return o << *e;
	return o << "null";
}
std::ostream& operator<<(std::ostream& o, const operator_plus& p) {
	return o << '(' << p.left << " + " << p.right << ')';
}
std::ostream& operator<<(std::ostream& o, const operator_minus& p) {
	return o << '(' << p.left << " - " << p.right << ')';
}
std::ostream& operator<<(std::ostream& o, const operator_multi& p) {
	return o << '(' << p.left << " * " << p.right << ')';
}
std::ostream& operator<<(std::ostream& o, const operator_divid& p) {
	return o << '(' << p.left << " / " << p.right << ')';
}
std::ostream& operator<<(std::ostream& o, const operator_power& p) {
	return o << '(' << p.left << " ** " << p.right << ')';
}
std::ostream& operator<<(std::ostream& o, const operator_extra& p) {
	return o << '(' << p.left << " % " << p.right << ')';
}
// may be your can write better, but it just an example :)
std::ostream& operator_plus::print(std::ostream& o) const { return o << *this; }
std::ostream& operator_minus::print(std::ostream& o) const { return o << *this; }
std::ostream& operator_multi::print(std::ostream& o) const { return o << *this; }
std::ostream& operator_divid::print(std::ostream& o) const { return o << *this; }
std::ostream& operator_extra::print(std::ostream& o) const { return o << *this; }
std::ostream& operator_power::print(std::ostream& o) const { return o << *this; }
std::ostream& ternary_expr::print(std::ostream& o) const { return o << *cond << " ? " << *if_true << " : " << *if_false; }

template<typename gh, template<auto>class th=gh::template term>
constexpr auto make_grammar() {
	// way to create reqursive expression in result
	// or we can create type with creates with new in ctor and skip result_maker
	auto result_maker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	auto term = gh::int_ | (gh::alpha >> *(gh::alpha|gh::d10|th<'_'>::char_));
	// lreq is left reqursion parser, so it stay on left field
	// lrreq just parses next variant, we need to place it on the right
	// ++ for store to second field (to binary_expr::right)
	return rv( [](auto& r){ return std::unique_ptr<expr>( new expr{std::move(r)} ); }
	  , cast<binary_expr>(gh::rv_lreq >> th<'+'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'-'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'*'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'/'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'%'>::_char >> ++gh::rv_rreq(result_maker))
	  , term
	);
	/*
	return rv( [](auto& r){ return std::unique_ptr<expr>( new expr{std::move(r)} ); }
	  , cast<ternary_expr>(gh::rv_lreq >> th<'?'>::_char >> ++gh::rv_rreq(result_maker) >> th<':'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'+'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'-'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'*'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'/'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'%'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> gh::template lit<"**"> >> ++gh::rv_rreq(result_maker))
	  , rv_result(th<'('>::_char >> gh::rv_req >> th<')'>::_char)
	  , term
	);
	*/
	/*
	return 
	    cast<ternary_expr>(gh::lreq(result_maker) >> th<'?'>::_char >> ++gh::lrreq(result_maker) >> th<':'>::_char >> ++gh::lrreq(result_maker))
	  | cast<binary_expr>(gh::lreq(result_maker) >> th<'+'>::_char >> ++gh::lrreq(result_maker))
	  | cast<binary_expr>(gh::lreq(result_maker) >> th<'-'>::_char >> ++gh::lrreq(result_maker))
	  | cast<binary_expr>(gh::lreq(result_maker) >> th<'*'>::_char >> ++gh::lrreq(result_maker))
	  | cast<binary_expr>(gh::lreq(result_maker) >> th<'/'>::_char >> ++gh::lrreq(result_maker))
	  | cast<binary_expr>(gh::lreq(result_maker) >> th<'%'>::_char >> ++gh::lrreq(result_maker))
	  | cast<binary_expr>(gh::lreq(result_maker) >> gh::template lit<"**"> >> ++gh::lrreq(result_maker))
	  | use_variant_result(th<'('>::_char >> gh::lvreq >> th<')'>::_char)
	  | term
	  ;
	*/
	// lvreq parser can to be replaced with req parser with hack.
	// note also use_variant_result - in normal case (without reqursion) variant, passed as result
	// must to have same element count as the parsers, but the function skips current index and uses
	// the top level value as result instead.
}

constexpr void test_expr(auto&& src, auto&& correct) {
	expr result;
	std::cout << "parse: " << src << std::endl;
	auto r = parse(
		make_grammar<parser_without_tests>(),
		+parser_without_tests::space,
		parser_without_tests::make_source(static_cast<decltype(src)&&>(src)),
		result);
	std::stringstream out;
	out << r << ' ' << result;
	std::cout << "\t" << (out.str() == correct) << '\t' << out.str() << "\n\tresult:\t" << correct << std::endl;
}

int main(int,char**) {
	// let's see how we parse
	test_expr("1+2", "3 (1 + 2)");
	test_expr("1 + 2", "5 (1 + 2)");
	test_expr("1 + 2 - 3", "9 (1 + (2 - 3))");
	// operator * has less priority
	test_expr("1+2*3+4", "7 ((1 + (2 * 3)) + 4)");
	test_expr("1 + 2 * 3 + 4", "13 ((1 + (2 * 3)) + 4)");
	// and the same with minus
	test_expr("1 + 2 - 3 - 4", "13 (1 + ((2 - 3) - 4))");
	test_expr("1 + 2 - 3 + 4", "13 ((1 + (2 - 3)) + 4)");
	//test_expr("(1 + 2) * 3 + 4", "15 (((1 + 2) * 3) + 4)");
	//test_expr("(1 + 2) * (3 + 4)", "17 ((1 + 2) * (3 + 4))");
	//test_expr("1 ? (1 + 2) * (3 % 4) : 0", "25 1 ? ((1 + 2) * (3 % 4)) : 0");

	return 0;
}
