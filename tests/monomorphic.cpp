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
		, cast<dbl_expr>(p::rv_lrec++ >> _char<'+'> >> p::rv_rrec(rmaker))
		, cast<dbl_expr>(p::rv_lrec++ >> _char<'*'> >> p::rv_rrec(rmaker))
		, p::int_
		, rv_result(_char<'('> >> p::rv_rec<0> >> _char<')'>)
		) ;
}

static_assert( [] {
	expr_rt r;
	return parse(test_rvariant_val(), +p::space, p::make_source("1 + 2 * (3+5)"), r);
}() == 13 );


int main(int,char**) {
  return 0;
}