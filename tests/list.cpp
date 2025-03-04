//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include "ascip.hpp"
#include "factory.hpp"

struct shift_result { char s{}; int shift{}; };

constexpr void test_unary_list() {
	test_parser_parse(std::vector<char>(), *char_<'a'>, "", 0);
	static_assert(test_parser_parse(std::vector<char>(), *char_<'a'>, "", 0).size() == 0);
	static_assert(test_parser_parse(std::vector<char>(), *char_<'a'>, "aa", 2).size() == 2);
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), *char_<'a'>, "aa", 2), 'a', 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), +char_<'a'>, "aa", 2), 'a', 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), (+(char_<'a'> >> char_<'b'>))([](auto& r){return p::fwd(r);}), "abab", 4), 'a','b','a','b' ), "use the std::vector as result for seq parser, not for list" );
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), (+(char_<'a'> >> char_<'b'>))([](auto& r){return p::fwd(r);}), "", -1) ));

#ifndef __clang__
	static_assert( ({char r='z';char_<'a'>.parse(make_test_ctx(),make_source('b'),r);r;}) == 'z' );
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), +(!char_<'a'>), "bb", 2), 0x00, 0x00 ),
			"!char_<'a'> parses but don't sore it's value, we have list with zeros (instead of infinit loop)");

	static_assert(test_cmp_vec( test_parser_parse(std::string(),+(char_<'a'>|char_<'b'>), "aab", 3), 'a', 'a', 'b' ));
#endif

	static_assert( [] {
		std::vector<shift_result> r;
		auto pr = parse(+(p::alpha++ >> p::ulist_shift<0>), +p::space, p::make_source("a    b c d e"), r);
		return (pr==12) + 2*(r.size()==5)
		+ 4*(r[1].shift==1) + 8*(r[2].shift==5)
		+ 16*(r[3].shift==2) + 32*(r[0].shift==0)
		;
	}() == 63 );
}

constexpr void test_binary_list() {
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), lower % ',', "a,b,c", 5), 'a', 'b', 'c' ));
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), lower % d10, "a1b2c", 5), 'a', 'b', 'c' ));
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), lower % d10, "a", 1), 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), lower % d10, "a1", 1), 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), lower % d10, "A1", -1) ));
	static_assert(test_cmp_vec( test_parser_parse(std::vector<char>(), ((lower >> lower) % ',')([](auto&r){return p::fwd(r);}), "ab,cd", 5), 'a','b','c','d' ));
	static_assert([] {
		std::vector<shift_result> r;
		auto pr = parse((p::alpha++ >> p::blist_shift<0>) % ',', +p::space, p::make_source("a ,  b, c ,d ,e"), r);
		return (pr==15) + 2*(r.size()==5)
		+ 4*(r[1].shift==3) + 8*(r[2].shift==4)
		+ 16*(r[3].shift==4) + 32*(r[0].shift==0)
		;
	}() == 63 );
}

int main(int,char**) {
  return 0;
}
