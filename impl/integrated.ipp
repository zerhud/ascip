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
	return true;
}
