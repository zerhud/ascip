#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDE_STD
#include <utility>
#endif
#include "ascip/details.hpp"

/* desicions:
 *
 * each parser is a class drived from base_parser class with passing it self as template parameter for base_parser
 * wrapping parser contains wrapped parser with [[no_unique_address]] attribute (most parsers are empty)
 * seq parser stores parsed part even if parser fails: the object must to be deleted and we cannot clean everithing
 * use seq operators in adl instead on base parser - we cannot use it on all parsers (+(++p) for example)
 * seq with left seq operator - it was hard to implement in adl at the time and there was no reason to do so, so it's a part of seq structure
 * transfom method - use it for trnasform parser with mutator structure. ussage: inject skipping parser, add stop number in right reqursion
 * variant_pos_tag - used for get variant position from context. there is no easy way to get it from type or by this.
 *   (lambda works as unique type only from free function, inside a template<...> struct {...}; it doesn't)
 * error handling:
 *   must<"name">(parser) catches semantic errors, calls lambda passed in parse method (via ctx)
 *   check result method - semact parser checks result with user method and returns user result (user also can throw error)
 *   check result method alternative - same method as lambda in sequence
 *   error handler parameters: line number, result, message, current src
 *   result checker parameters: line number, result, src on seq start
 *   implement function enabled from adl for roll back src on line begin
 * left reqursion implementation: implmemented with reverse variant parser. it needs in extra make_result method for temporary
 *   store parse result.
 */

namespace {


template<template<typename...>class tuple, typename factory_t=void>
struct ascip {

using holder = ascip<tuple, factory_t>;
using parse_result = decltype(-1);

template<typename parser> struct base_parser : ascip_details::adl_tag {
	using type_in_base = parser;
	using holder = ascip<tuple, factory_t>;

	constexpr static int start_context_arg = 1;
	constexpr static const char* source_symbol = "ab";

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert( requires(const parser& p){ p.p; }, "child parser should define own parse method or have p field" );
		return static_cast<const parser&>(*this)
			.p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}

#include "ascip/operators.ipp"
};

constexpr static auto make_test_ctx() { return ascip_details::make_ctx<ascip_details::new_line_count_tag>(1); }
constexpr static auto make_test_ctx(auto err_handler){ return make_ctx<ascip_details::err_handler_tag>(err_handler, make_test_ctx()); }
//template<auto... i> friend constexpr auto make_test_ctx(const base_parser<auto>&) { return ascip_details::make_ctx<ascip_details::parser_concept_check_tag>(1); }
template<auto... i, typename parser_param> friend constexpr auto make_test_ctx(const base_parser<parser_param>&) { return ascip_details::make_ctx<ascip_details::parser_concept_check_tag>(1); }
// ^^ implemented for ascip_details::parser concept 

#include "ascip/test_utils.ipp"
#include "ascip/make_source.ipp"
#include "ascip/exists.ipp"
#include "ascip/base_parsers.ipp"
#include "ascip/semact.ipp"
#include "ascip/wrappers.ipp"
#include "ascip/variant.ipp"
#include "ascip/rvariant.ipp"
#include "ascip/lists.ipp"
#include "ascip/seq.ipp"
#include "ascip/transform.ipp"
#include "ascip/injection.ipp"
#include "ascip/integrated.ipp"

constexpr static void test() {
#ifdef __clang__
	static_assert( ascip_details::parser<char_parser<'b'>> );
#else
	static_assert( ascip_details::parser<decltype(char_<'b'>)> );
#endif
	test_sources();
	test_parser_char();
	test_parser_value();
	static_assert( space.test() );
	static_assert( any.test() );
	static_assert( int_.test() );
	static_assert( fp.test() );
	static_assert( fp.test() );
	static_assert( test_literal_parser() );
	static_assert( test_variant() );
	static_assert( test_rvariant() );
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
	static_assert( test_transform() );
}

template<auto sym> struct tmpl {
	constexpr static auto& fp = holder::fp;
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
	constexpr static auto& rv_lreq = holder::rv_lreq;
	constexpr static auto& rv_rreq = holder::rv_rreq;
	constexpr static auto& nl = holder::nl;
	constexpr static auto& quoted_string = holder::quoted_string;
	constexpr static auto& dquoted_string = holder::dquoted_string;
	constexpr static auto& squoted_string = holder::squoted_string;
};
template<auto sym> struct term : tmpl<sym> {};
// c++ cannot use auto as string_literal
template<ascip_details::string_literal v> struct sterm {
	constexpr static auto& lit = holder::lit<v>;
};

}; // struct ascip (context)

} // namespace <anonymus>

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"
#endif

template<typename factory_t, template<typename...>class tuple>
struct ascip_literals {
template<typename char_t, char_t... chars>
friend constexpr auto operator""_lex() {
	return lexeme( omit( (... >> ascip<tuple, factory_t>::template char_<chars>) ) );
}
}; // namespace ascip_literals

#ifdef __clang__
#pragma GCC diagnostic pop
#endif
