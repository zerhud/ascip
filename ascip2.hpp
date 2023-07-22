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
 * negate parser switching state and it drived from wrapped parser
 */

template<typename factory_t, template<typename...>class tuple>
struct ascip {


struct parser_tag {};
template<typename parser, typename act_t> struct semact_parser : parser {
	act_t act;
	constexpr const auto parse(auto&& ctx, auto src, auto& result) const requires (
	requires{ requires std::is_lvalue_reference_v<decltype(act(result))>; } && !requires{ act(); /* check if ... pattern */ }
	) {
		auto& nr = act(result);
		return parser::parse(ctx, src, nr);
	}
	constexpr const auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = parser::parse(ctx, src, result);
		if(ret >= 0) {
			if constexpr (requires{ act(ret, ctx, src, result); }) act(ret, ctx, src, result);
			else if constexpr (requires{ acct(ret, result); }) act(ret, result); 
			else act();
		}
		return ret;
	}
};
template<typename parser> struct base_parser : parser_tag {
	constexpr static int start_context_arg = 1;
	constexpr static char source_symbol = 'a';
	constexpr auto operator()(auto act) const {
		return semact_parser<parser, decltype(auto(act))>{
			static_cast<const parser&>(*this), static_cast<decltype(act)&&>(act) };
	}

#include "impl/operators.ipp"
};

constexpr static auto make_test_ctx() { return std::tuple<int>{0}; }
constexpr static auto make_test_ctx(auto arg) { return std::tuple(arg); }
friend constexpr auto make_test_ctx(const base_parser<auto>&) { return make_test_ctx(1); }
// ^^ implemented for ascip_details::parser concept 

#include "impl/make_source.ipp"
#include "impl/base_parsers.ipp"
#include "impl/wrappers.ipp"
#include "impl/variant.ipp"

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
}

}; // struct ascip (context)

#include "impl/operators.ipp"
