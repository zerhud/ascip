#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


constexpr static auto dquoted_string = lexeme(_char<'"'> >> *(as<'"'>(char_<'\\'> >> char_<'"'>)| (ascip::any - char_<'"'>)) >> _char<'"'>);
constexpr static auto squoted_string = lexeme(_char<'\''> >> *(as<'\''>(char_<'\\'> >> char_<'\''>)| (ascip::any - char_<'\''>)) >> _char<'\''>);
constexpr static auto quoted_string = lexeme(squoted_string | dquoted_string);

constexpr static bool integrated_tests() {
	static_assert( test_parser_parse_r_str(quoted_string, "'ok'", 4, 'o', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "'o\\'k'", 6, 'o', '\'', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "' o\\'k'", 7, ' ', 'o', '\'', 'k') );

	static_assert( test_parser_parse_r_str(quoted_string, "\"ok\"", 4, 'o', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "\"o'k\"", 5, 'o', '\'', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "\" o'k\"", 6, ' ', 'o', '\'', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "\" o\\\"k\"", 7, ' ', 'o', '"', 'k') );

	static_assert( ({ auto r = mk_str(); parse(quoted_string, +space, make_source("'ok'"), r); }) == 4);
	static_assert( ({ auto r = mk_str(); parse(+alpha, +space, make_source("a b c "), r); }) == 5);
	static_assert( ({ auto r = mk_str(); parse(+alpha, +space, make_source(" a b c "), r); }) == 6);
	static_assert( ({ auto r = mk_str(); parse((as(char_<'a'>, 'b')|char_<'c'>) % int_, +space, make_source("c1  a2 a  3 a 3a"), r); }) == 16);
	static_assert( ({ auto r = mk_str(); parse((as<'b'>(char_<'a'>)|char_<'c'>) % int_, +space, make_source("c1  a2 a  3 a 3a"), r); }) == 16);

	static_assert( ({ auto r = mk_str(); auto ctx = make_test_ctx();
		quoted_string.parse(ctx, make_source("'1\n2\n'"), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx);}) == 3);
	return true;
}
