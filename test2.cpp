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

using parser = ascip<factory, std::tuple>;
//using parser_with_own_tuple = ascip<factory, ascip_details::tuple>;

int main(int,char**) {
	std::cout << "start" << std::endl;
	parser::test();
	//parser_with_own_tuple::test();
	std::cout << "finish" << std::endl;

	ascip_details::tuple<int,double> tt{ 1, .5 };
	std::cout << "tuple test (1 0.5) == (" << get<0>(tt) << " " << get<1>(tt) << ')' << std::endl;

	return 0;
}

