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
 * seq with left seq operator - hard to implement in adl and no reason to do it, so it's a part of seq structure
 * variant_pos_tag - used for get variant position from context. there is no easy way to get it from type or by this.
 *   (lambda works as unique type only from free function, inside a template<...> struct {...}; it doesn't)
 */

namespace {


template<template<typename...>class tuple, typename factory_t=void>
struct ascip {

using holder = ascip<tuple, factory_t>;

template<typename parser> struct base_parser : ascip_details::adl_tag {
	using type_in_base = parser;
	using holder = ascip<tuple, factory_t>;

	constexpr static int start_context_arg = 1;
	constexpr static const char* source_symbol = "ab";

	template<auto ind>
	constexpr auto parse_from(auto&& ctx, auto src, auto& result) const {
		return static_cast<const parser&>(*this)
			.p.template parse_from<ind>(
					static_cast<decltype(ctx)&&>(ctx),
					static_cast<decltype(src)&&>(src),
					result);
	}

#include "impl/operators.ipp"
};

constexpr static auto make_test_ctx() { return ascip_details::make_ctx<ascip_details::new_line_count_tag>(1); }
constexpr static auto make_test_ctx(auto err_handler){ return make_ctx<ascip_details::err_handler_tag>(err_handler, make_test_ctx()); }
template<auto... i> friend constexpr auto make_test_ctx(const base_parser<auto>&) { return ascip_details::make_ctx<ascip_details::parser_concept_check_tag>(1); }
// ^^ implemented for ascip_details::parser concept 

#include "impl/test_utils.ipp"
#include "impl/make_source.ipp"
#include "impl/base_parsers.ipp"
#include "impl/semact.ipp"
#include "impl/wrappers.ipp"
#include "impl/variant.ipp"
#include "impl/lists.ipp"
#include "impl/seq.ipp"
#include "impl/lreq.ipp"
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
	static_assert( test_lreq() );
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
	constexpr static auto& alpha = holder::alpha;
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
	return lexeme( omit( (... >> ascip<tuple, factory_t>::template char_<chars>) ) );
}
}; // namespace ascip_literals
