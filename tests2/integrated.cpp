//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "factory.hpp"

static_assert( test_parser_parse_r_str(p::quoted_string, "'ok'", 4, 'o', 'k') );
static_assert( test_parser_parse_r_str(p::quoted_string, "'o\\'k'", 6, 'o', '\'', 'k') );
static_assert( test_parser_parse_r_str(p::quoted_string, "' o\\'k'", 7, ' ', 'o', '\'', 'k') );

static_assert( test_parser_parse_r_str(p::quoted_string, "\"ok\"", 4, 'o', 'k') );
static_assert( test_parser_parse_r_str(p::quoted_string, "\"o'k\"", 5, 'o', '\'', 'k') );
static_assert( test_parser_parse_r_str(p::quoted_string, "\" o'k\"", 6, ' ', 'o', '\'', 'k') );
static_assert( test_parser_parse_r_str(p::quoted_string, "\" o\\\"k\"", 7, ' ', 'o', '"', 'k') );

constexpr auto parse_to_str(auto parser, auto src) {
	std::string r;
	return parse(parser, +p::space, p::make_source(src), r);
}
static_assert( parse_to_str(p::quoted_string, "'ok'") == 4 );
static_assert( parse_to_str(+p::alpha, "a b c") == 5 );
static_assert( parse_to_str(+p::alpha, " a b c") == 6 );
static_assert( parse_to_str((as(p::char_<'a'>, 'b')|p::char_<'c'>) % p::int_, "c1  a2 a  3 a 3a") == 16 );
static_assert( parse_to_str((as<'b'>(p::char_<'a'>)|p::char_<'c'>) % p::int_, "c1  a2 a  3 a 3a") == 16 );

/* TODO: fix new line counter or remove it
static_assert( []{
	std::string r;
	auto ctx = p::make_test_ctx();
	p::quoted_string.parse(ctx, p::make_source("'1\n2\n'"), r);
	return new_line_count(ctx);
}() == 3, "line count works in quoted_string. the first line is line number 1." );
*/

int main(int,char**) {
	return 0;
}
