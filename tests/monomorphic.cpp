//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <memory>
#include <variant>
#include "ascip.hpp"
#include "factory.hpp"

struct expr_rt;
using term_rt = std::variant<int,std::string>;
struct dbl_expr{
	std::unique_ptr<expr_rt> left;
	std::unique_ptr<expr_rt> right;
};
struct mul_expr : dbl_expr {} ;
struct pls_expr : dbl_expr {} ;
struct expr_rt : std::variant< pls_expr, mul_expr, int > {};

constexpr auto test_rvariant_val() {
	constexpr auto maker = [](auto& r){ return std::unique_ptr<expr_rt>( new expr_rt{std::move(r)} ); };
	constexpr auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	return rv(std::forward<decltype(maker)>(maker)
		, cast<dbl_expr>(p::rv_lreq++ >> _char<'+'> >> p::rv_rreq(rmaker))
		, cast<dbl_expr>(p::rv_lreq++ >> _char<'*'> >> p::rv_rreq(rmaker))
		, p::int_
		, rv_result(_char<'('> >> p::rv_req<0> >> _char<')'>)
		) ;
}

static_assert( [] {
	expr_rt r;
	return parse(test_rvariant_val(), +p::space, p::make_source("1 + 2 * (3+5)"), r);
}() == 13 );

struct value { char n{}; char v{}; std::vector<std::unique_ptr<value>> c; };
struct container {
    std::vector<std::unique_ptr<value>> c;
};

constexpr auto value_maker = [](auto& v){ v.emplace_back() = std::make_unique<value>(); return v.back().get(); };

/* NOTE: this is a test for a long compilation bug in variant parser.
 *       the test halts down the IDE, so it commented, but it should work and should to be compiled ~5 seconds
static_assert( [] {
	auto a = (p::char_<'a'>++ >> p::char_<'1'>++ >> -p::req<1>)(value_maker);
	auto c = (p::char_<'c'>++ >> p::char_<'3'>++ >> -p::req<1>)(value_maker);
	auto i = a;
	auto parser = p::nop >> (a | c | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i
		| i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i
		| i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i
		| i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i
		| i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i | i
		);
	container r;
	return parse(parser, +p::space, p::make_source("c3a1"), r.c);
}() == 4, "there was a bug in variant and this parser was a long long time to compile" );
*/

int main(int,char**) {
  return 0;
}