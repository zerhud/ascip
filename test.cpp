#include <vector>
#include <string>
#include <memory>
#include <variant>
#include <sstream>
#include <iostream>
#include "ascip.hpp"

using namespace std::literals;

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

using parser = ascip<factory>;
using parser_with_own_tuple = ascip<factory>;
using redefined_parser = ascip<factory_for_redefine_test>;
using parser_without_tests = ascip<>;

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

template<typename gh, template<auto>class sup = gh::template term>
constexpr void test_error_handling() {
	std::cout << "test error handling" << std::endl;
	char r;
	parse(gh::any >> must<"a test">(sup<'a'>::char_), +gh::space, gh::make_source("\nb"), r,
		[](char& r, auto src, int line, auto msg){
		gh::write_out_error_msg(std::cout, "test_file", msg, "ups", src, line);
	});
	std::cout << "---" << std::endl;
	parse(gh::any >> gh::any >> +sup<'b'>::char_ >> must<"a test">(sup<'a'>::char_), +gh::space, gh::make_source("a\nbbbcuu"), r,
		[](char& r, auto src, int line, auto msg){
		gh::write_out_error_msg(std::cout, "test_file", msg, "ups", src, line);
		return -1;
	});
	std::cout << "finish test" << std::endl;
}

template< typename gh, template<auto>class term=gh::template term >
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
#ifndef __clang__
	static_assert( ({char r;make_test_gram<parser>().parse(parser::make_test_ctx(), parser::make_source("a"), r);r;}) == 'b' );
	static_assert( ({double r=100;parser::fp.parse(parser::make_test_ctx(), parser::make_source("0"sv), r); }) == -1);
	static_assert( ({
		std::vector<int> r;
		parse(+parser::int_, +parser::space, parser::make_source("1 2 3 "sv), r);
		r.at(2);
	}) == 3 );
#endif
	std::cout << "finish" << std::endl;
	std::cout << "rt start" << std::endl;
	std::string rt_r;
	parse(make_test_gram<parser>(), +parser::space, parser::make_source("c1  a2 a  3 a 3a"), rt_r);
	std::cout << "\t(5 == " << rt_r.size() << ") " << rt_r << std::endl;
	std::cout << "finish" << std::endl;


	ascip_details::tuple<int,double> tt{ 1, .5 };
	std::cout << "tuple test (1 0.5) == (" << get<0>(tt) << " " << get<1>(tt) << ')' << std::endl;

	return 0;
}

