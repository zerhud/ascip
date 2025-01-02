//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <vector>

#include "ascip.hpp"

using p = ascip<>;

constexpr auto test_cmp_vec(const auto& vec, auto... vals) {
	auto correct = std::vector<decltype(auto(vec[0]))>();
	(correct.emplace_back(vals), ...);
	(void)(vec.size() / (vec.size() == sizeof...(vals)));
	//TODO: show also i shomehow in compiletime
	for(auto i=0;i<vec.size();++i) (void)(vec[i] / (vec[i] == correct[i]));
	return true;
}

constexpr bool test_parser_parse_r(auto&& r, auto p, auto&& src, auto pr, auto... vals) {
	auto parse_method_result = p.parse(p::make_test_ctx(), p::make_source(src), r);
	parse_method_result /= (parse_method_result==pr);
	test_cmp_vec(r, vals...);
	return true;
}

constexpr bool test_parser_parse_r_str(auto p, auto&& src, auto pr, auto... vals) {
	return test_parser_parse_r(std::string{}, p, static_cast<decltype(src)&&>(src), pr, static_cast<decltype(vals)&&>(vals)...);
}


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

static_assert( []{
	std::string r;
	auto ctx = p::make_test_ctx();
	p::quoted_string.parse(ctx, p::make_source("'1\n2\n'"), r);
	return search_in_ctx<ascip_details::new_line_count_tag>(ctx);
}() == 3, "line count works in quoted_string. the first line is line number 1." );

int main(int,char**) {
	return 0;
}
