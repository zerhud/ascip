#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/values.hpp"
#include "../details/test_utils.hpp"

namespace ascip_details::prs {

template<auto val, parser parser> struct tmpl_as_parser : base_parser<tmpl_as_parser<val,parser>> {
	[[no_unique_address]] parser p;
	constexpr tmpl_as_parser() =default ;
	constexpr tmpl_as_parser(tmpl_as_parser&&) =default ;
	constexpr tmpl_as_parser(const tmpl_as_parser&) =default ;
	constexpr tmpl_as_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_parse_without_result r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};

template<auto val, parser parser_t> constexpr auto as( parser_t&& p) {
	using type = std::decay_t<decltype(p)>;
	return tmpl_as_parser<val, type>{ std::forward<decltype(p)>(p) };
}

template<typename value_t, parser parser> struct as_parser : base_parser<as_parser<value_t, parser>> {
	value_t val;
	[[no_unique_address]] parser p;
	constexpr as_parser() =default ;
	constexpr as_parser(as_parser&&) =default ;
	constexpr as_parser(const as_parser&) =default ;
	constexpr as_parser(value_t val, parser p) : val(std::move(val)), p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_parse_without_result r;
		auto shift = p.parse(ctx, std::move(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};

template<typename value_t, parser parser_t> constexpr auto as( parser_t&& p, value_t&& val ){
	using ptype = std::decay_t<decltype(p)>;
	using val_type = std::decay_t<decltype(val)>;
	return as_parser<val_type, ptype>( std::forward<decltype(val)>(val), std::forward<decltype(p)>(p) );
}

template<typename prs, template<auto>class t=prs::template tmpl>
constexpr static bool test_as() {
	static_assert( test_parser_char(as(t<'a'>::char_, 'b'), "a", 1) == 'b' );
	static_assert( test_parser_char(as(t<'a'>::char_, 'b'), "b", -1) == 'z' );
	static_assert( test_parser_char(as(prs::int_, 'b'), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(prs::int_), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(prs::int_), "a", -1) == 'z' );
	static_assert( []{
		auto p = t<'a'>::char_;
		return test_parser_char(as(p, 'b'), "a", 1);
	}() == 'b' );
	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename parser, typename value_type>
constexpr static auto transform_special(prs::as_parser<value_type, parser>&& src, auto&& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( prs::as_parser{ src.val, std::move(np) }, nctx );
}

template<typename mutator, typename parser, auto value>
constexpr static auto transform_special(prs::tmpl_as_parser<value, parser>&& src, auto&& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( prs::tmpl_as_parser<value, std::decay_t<decltype(np)>>{ std::move(np) }, nctx );
}

}