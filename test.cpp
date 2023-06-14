#include <vector>
#include <string>
#include <iostream>
#include "ascip.hpp"

struct factory {
	template<typename type>
	constexpr auto mk_vec() { return std::vector<type>(); }
	constexpr auto mk_str() { return std::string(); }
	constexpr auto mk_str(auto&&... args) { return std::string(args...); }
	constexpr auto mk_sv(const char* str){ return std::string_view{ str }; }
};

int main(int,char**) {
	std::cout << "start" << std::endl;
	ascip::test<factory>();
	std::cout << "finish" << std::endl;

	ascip::details::tuple<int,double> tt{ 1, .5 };
	std::cout << "tt: " << get<0>(tt) << " " << get<1>(tt) << std::endl;
	return 0;
}
