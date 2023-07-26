#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include "ascip2.hpp"

struct factory {
	template<typename type>
	constexpr auto mk_vec() { return std::vector<type>(); }
	constexpr auto mk_str() { return std::string(); }
	constexpr auto mk_str(auto&&... args) { return std::string(args...); }
	constexpr auto mk_sv(const char* str){ return std::string_view{ str }; }
};
struct factory_for_redefine_test {
	template<typename type>
	constexpr auto mk_vec() { return std::vector<type>(); }
	constexpr auto mk_str() { return std::string(); }
	constexpr auto mk_str(auto&&... args) { return std::string(args...); }
	constexpr auto mk_sv(const char* str){ return std::string_view{ str }; }
};

using parser = ascip<factory, std::tuple>;
using parser_with_own_tuple = ascip<factory, ascip_details::tuple>;
using redefined_parser = ascip<factory_for_redefine_test, std::tuple>;

template<typename gram_holder>
struct test_grammar {
	using gh = gram_holder;
	template<auto s> static auto c_ = gram_holder::template char_<s>;
	constexpr static auto make() {
		auto& int_ = gh::int_;
		return as(c_<'a'>, 'b') % int_;
	}
};

template< typename gram_holder, template<auto>class term=gram_holder::template terms >
constexpr auto make_test_gram() {
	return as(term<'a'>::char_, 'b') % gram_holder::int_;
}

int main(int,char**) {
	std::cout << "start" << std::endl;
	parser::test();
	redefined_parser::test();
	//TODO: implement own tuple :)
	//parser_with_own_tuple::test();
	static_assert( ({char r;make_test_gram<parser>().parse(parser::make_test_ctx(), parser::make_source("a"), r);r;}) == 'b' );
	static_assert( ({char r;test_grammar<parser>::make().parse(parser::make_test_ctx(), parser::make_source("a"), r);r;}) == 'b' );
	std::cout << "finish" << std::endl;
	std::cout << "rt start" << std::endl;
	std::string rt_r;
	parse(make_test_gram<parser>(), +parser::space, parser::make_source("a1  a2 a  3 a 3a"), rt_r);
	std::cout << "\t(5 == " << rt_r.size() << ") " << rt_r << std::endl;
	std::cout << "finish" << std::endl;

	ascip_details::tuple<int,double> tt{ 1, .5 };
	std::cout << "tuple test (1 0.5) == (" << get<0>(tt) << " " << get<1>(tt) << ')' << std::endl;

	return 0;
}

