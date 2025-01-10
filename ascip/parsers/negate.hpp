#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "opt.hpp"
#include "char.hpp"
#include "base.hpp"
#include "../details/context.hpp"
#include "../details/make_source.hpp"

namespace ascip_details::prs {

template<parser parser> struct negate_parser : base_parser<negate_parser<parser>> {
	parser p;
	constexpr negate_parser() =default ;
	constexpr negate_parser(negate_parser&&) =default ;
	constexpr negate_parser(const negate_parser&) =default ;
	constexpr negate_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, std::move(src), result);
		return ret * (-1); //TODO: what if the ret is 0?
	}
};

constexpr auto operator!(parser auto&& p) {
	using p_type = std::decay_t<decltype(p)>;
	if constexpr (is_specialization_of<p_type, negate_parser>) return p.p;
	else return negate_parser<p_type>{ std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_negate() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
#ifndef __clang__
	static_assert( ({char r{};(!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);}) == -1 );
	static_assert( ({char r{};(!t<'a'>::char_).parse(make_test_ctx(), make_source('b'), r);}) ==  1 );
	static_assert( ({char r='z';(!t<'a'>::char_).parse(make_test_ctx(), make_source('b'), r);r;}) ==  'z' );

	static_assert( ({char r{};(!!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);}) == 1 );
	static_assert( ({char r{};(!!!!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);r;}) == 'a' );
#endif

	static_cast<const decltype(auto(t<'a'>::char_))&>(!!t<'a'>::char_);
	static_cast<const decltype(auto(t<'a'>::char_))&>(!!!!t<'a'>::char_);
	static_cast<const opt_parser<negate_parser<char_parser<'a'>>>&>(-(!t<'a'>::char_));
	static_cast<const opt_parser<char_parser<'a'>>&>(-(!!t<'a'>::char_));

	return true;
#pragma GCC diagnostic pop
}

}