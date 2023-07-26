#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include "impl/details.hpp"

/* desicions:
 * each parser is a class drived from base_parser class with passing it self as template parameter for base_parser
 * wrapping parser contains wrapped parser with [[no_unique_address]] attribute (most parsers are empty)
 * seq parser stores parsed part even if parser fails: the object must to be deleted and we cannot clean everithing
 * use seq operators in adl instead on base parser - we cannot use it on all parsers (+(++p) for example)
 * seq with left seq - hard to implement in adl and no reason to do it, so it's a part of seq structure
 */

namespace {


template<typename factory_t, template<typename...>class tuple>
struct ascip {

using holder = ascip<factory_t, tuple>;

template<typename parser> struct base_parser : ascip_details::adl_tag {
	using holder = ascip<factory_t, tuple>;
	constexpr static int start_context_arg = 1;
	constexpr static const char* source_symbol = "ab";

#include "impl/operators.ipp"
};

constexpr static auto make_test_ctx(auto&&... args) { return tuple(static_cast<decltype(args)>(args)...); }
constexpr static auto make_test_ctx() { return make_test_ctx(ascip_details::without_req_flag{}, (int*)nullptr); }
template<auto... i> friend constexpr auto make_test_ctx(const base_parser<auto>&) { return make_test_ctx(i...); }
// ^^ implemented for ascip_details::parser concept 

#include "impl/test_utils.ipp"
#include "impl/make_source.ipp"
#include "impl/base_parsers.ipp"
#include "impl/semact.ipp"
#include "impl/wrappers.ipp"
#include "impl/variant.ipp"
#include "impl/lists.ipp"
#include "impl/seq.ipp"
#include "impl/injection.ipp"
#include "impl/integrated.ipp"

constexpr static void test() {
	static_assert( ascip_details::parser<decltype(char_<'b'>)> );
	test_sources();
	test_parser_char();
	test_parser_value();
	static_assert( space.test() );
	static_assert( any.test() );
	static_assert( int_.test() );
	static_assert( test_variant() );
	static_assert( test_range_parser() );
	static_assert( test_negate() );
	static_assert( test_optional() );
	static_assert( test_omit() );
	static_assert( test_as() );
	static_assert( test_lists() );
	static_assert( test_checkers() );
	static_assert( test_different() );
	static_assert( test_semact() );
	static_assert( test_seq() );
	static_assert( test_injection() );
	static_assert( integrated_tests() );
}

template<auto sym> struct terms {
	constexpr static auto& char_ = holder::char_<sym>;
	constexpr static auto& _char = holder::_char<sym>;
	constexpr static auto& space = holder::space;
	constexpr static auto& any = holder::any;
	constexpr static auto& int_ = holder::int_;
	constexpr static auto& lower = holder::lower;
	constexpr static auto& upper = holder::upper;
	constexpr static auto& digit = holder::digit;
	constexpr static auto& d10 = holder::d10;
	constexpr static auto& ascii = holder::ascii;
	constexpr static auto& cur_pos = holder::cur_pos;
	constexpr static auto& cur_shift = holder::cur_shift;
	constexpr static auto& req = holder::req<sym>;
	constexpr static auto& nl = holder::nl;
	constexpr static auto& quoted_string = holder::quoted_string;
	constexpr static auto& dquoted_string = holder::dquoted_string;
	constexpr static auto& squoted_string = holder::squoted_string;
};

}; // struct ascip (context)

} // namespace <anonymus>

template<typename factory_t, template<typename...>class tuple>
struct ascip_literals {
template<typename char_t, char_t... chars>
friend constexpr auto operator""_lex() {
	return lexeme( omit( (... >> ascip<factory_t, tuple>::template char_<chars>) ) );
}
}; // namespace ascip_literals
