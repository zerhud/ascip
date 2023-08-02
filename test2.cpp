#include <tuple>
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <sstream>
#include <iostream>
#include "ascip2.hpp"

struct factory {
	template<typename... types> using variant = std::variant<types...>;
	template<typename... types> using unique_ptr = std::unique_ptr<types...>;

	template<typename type>
	constexpr auto mk_vec() { return std::vector<type>(); }
	constexpr auto mk_str() { return std::string(); }
	constexpr auto mk_str(auto&&... args) { return std::string(args...); }
	constexpr auto mk_sv(const char* str){ return std::string_view{ str }; }
};
struct factory_for_redefine_test {
	template<typename... types> using variant = std::variant<types...>;
	template<typename... types> using unique_ptr = std::unique_ptr<types...>;

	template<typename type>
	constexpr auto mk_vec() { return std::vector<type>(); }
	constexpr auto mk_str() { return std::string(); }
	constexpr auto mk_str(auto&&... args) { return std::string(args...); }
	constexpr auto mk_sv(const char* str){ return std::string_view{ str }; }
};

using parser = ascip<std::tuple, factory>;
using parser_with_own_tuple = ascip<ascip_details::tuple, factory>;
using redefined_parser = ascip<std::tuple, factory_for_redefine_test>;
using parser_without_tests = ascip<std::tuple>;

template<typename type> concept printable = requires(std::ostream& o, const type& obj){ o << obj; };
using term_rt = std::variant<int,std::string>;
struct expr_rt;
struct dbl_expr {
	std::unique_ptr<expr_rt> left;
	int opcode=5000;
	std::unique_ptr<expr_rt> right;
	void print(std::ostream& o) const ;
};
struct operator_mul: dbl_expr {};
struct operator_plus : dbl_expr {};
struct expr_rt : std::variant<operator_plus, operator_mul, term_rt> {};
template<template<typename...>class variant, printable... types>
std::ostream& operator<<(std::ostream& o, const variant<types...>& obj) {
	std::visit([&o](auto& v){o << v;}, obj);
	return o;
}
std::ostream& operator<<(std::ostream& o, const expr_rt& obj) {
	std::visit([&o](auto& v){
		if constexpr (requires{v.print(o);}) v.print(o);
		else o << v;
	}, obj);
	return o;
}
std::ostream& operator<<(std::ostream& o, const dbl_expr& obj) {
	if(!obj.left) o << "(null ";
	else o << '(' << *obj.left << ' ';
	o << obj.opcode ;
	if(!obj.right) o << " null";
	else o << ' ' << *obj.right;
	return o << ')';
}
void dbl_expr::print(std::ostream& o) const { o << *this; }
template<typename gram_holder>
struct expr_grammar {
	using gh = gram_holder;
	template<auto s> static auto char_ = gh::template char_<s>;
	template<auto s> static auto _char = gh::template _char<s>;
	constexpr static auto alpha = gh::alpha;
	constexpr static auto d10 = gh::d10;
	constexpr static auto int_ = gh::int_;
	constexpr static auto make() {
		auto result_maker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
		auto ident = gh::alpha >> *(alpha|d10|char_<'_'>);
		auto term = int_ | ident;
		auto expr =
			  cast<dbl_expr>((gh::lreq(result_maker))++ >> as(_char<'+'>, 0)++ >> gh::lrreq(result_maker))
			| cast<dbl_expr>((gh::lreq(result_maker))++ >> as(_char<'*'>, 1)++ >> gh::lrreq(result_maker))
			| term;
		return expr;
	}
};

constexpr void test_expr(auto&& src, auto&& correct) {
	expr_rt result;
	std::cout << "parse: " << src << std::endl;
	auto r = parse(
		expr_grammar<parser_without_tests>::make(),
		+parser_without_tests::space,
		parser_without_tests::make_source(static_cast<decltype(src)&&>(src)),
		result);
	std::stringstream out;
	out << r << ' ' << result;
	std::cout << "\t" << (out.str() == correct) << '\t' << out.str() << "\n\tresult:\t" << r << ' ' << result << std::endl;
}
constexpr void test_expr() {
	std::cout << "test left reqursion" << std::endl;
	std::unique_ptr<expr_rt> left(new expr_rt{term_rt{1}});
	std::cout << "test print: " << dbl_expr{std::move(left), 10, nullptr} << std::endl;
	test_expr("", "0");
	test_expr("1", "1 1");
	test_expr("1 + 2", "5 (1 0 2)");
	test_expr("11 + 2", "6 (11 0 2)");
	test_expr("11 + 2 + 3", "10 ((11 0 2) 0 3)");
	test_expr("11 + 2 + 4 + 8", "14 (((11 0 2) 0 3) 0 8)");
	test_expr("11 + 2 + 4 + 8 + 9", "18 ((((11 0 2) 0 4) 0 8) 0 9)");
	test_expr("kuku + 2 + 4 + ok + 9 + 2143", "28 (((((kuku 0 2) 0 4) 0 ok) 0 9) 0 2143)");
	test_expr("11 * 2 + 3", "10 ((11 1 2) 0 3)");
	test_expr("11 + 2 * 4 + 8", "14 ((11 0 (2 1 3)) 0 8)");
	test_expr("kuku + 2 + 4 * ok + 9 + 2143", "28 (((((kuku 0 2) 0 (4 1 ok) 0 9) 0 2143)");
	std::cout << "finish test" << std::endl;
}

template< typename gh, template<auto>class term=gh::template terms >
constexpr auto make_test_gram() {
	return (as(term<'a'>::char_, 'b') | term<'c'>::char_) % gh::int_;
}


int main(int,char**) {
	auto print_variant  = [](auto& v) {
		if constexpr (requires{ std::cout<<v; }) std::cout << v;
		else std::cout << typeid(v).name() ;
	};
	std::cout << "start" << std::endl;
	parser::test();
	redefined_parser::test();
	//TODO: implement own tuple :)
	//parser_with_own_tuple::test();
	static_assert( ({char r;make_test_gram<parser>().parse(parser::make_test_ctx(), parser::make_source("a"), r);r;}) == 'b' );
	std::cout << "finish" << std::endl;
	std::cout << "rt start" << std::endl;
	std::string rt_r;
	parse(make_test_gram<parser>(), +parser::space, parser::make_source("c1  a2 a  3 a 3a"), rt_r);
	std::cout << "\t(5 == " << rt_r.size() << ") " << rt_r << std::endl;
	std::cout << "finish" << std::endl;


	test_expr();

	ascip_details::tuple<int,double> tt{ 1, .5 };
	std::cout << "tuple test (1 0.5) == (" << get<0>(tt) << " " << get<1>(tt) << ')' << std::endl;

	return 0;
}

