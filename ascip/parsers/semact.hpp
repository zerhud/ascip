#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/make_source.hpp"
#include "../details/values.hpp"

namespace ascip_details::prs {

template<typename parser, typename act_t> struct semact_parser : base_parser<semact_parser<parser,act_t>> {
	act_t act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr bool is_any_arg_pattern = !requires{ act(); };
		if constexpr(std::is_same_v<type_any_eq_allow&, decltype(result)>)
			return p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
		else if constexpr(requires{ act(result); requires std::is_pointer_v<decltype(act(result))>;} ) {
			auto* nr = act(result);
			return p.parse(ctx, src, *nr);
		}
		else if constexpr (requires{ act(result); requires std::is_lvalue_reference_v<decltype(act(result))>; } ) {
			auto& nr = act(result);
			return p.parse(ctx, src, nr);
		}
		else if constexpr(requires{ act(result); } && !requires{act(0, ctx, src, result);} ) {
			auto nr = act(result);
			return p.parse(ctx, src, nr);
		}
		else {
			auto ret = p.parse(ctx, src, result);
			if(ret >= 0) {
				if constexpr (requires{ act(ret, &ctx, src, result); }) act(ret, &ctx, src, result);
				else if constexpr (requires{ act(ret, result); }) act(ret, result);
				else act();
			}
			return ret;
		}
	}
};

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_semact() {
#ifndef __clang__
	static_assert( ({char r='z',a='y';(t<'a'>::char_ >> t<'b'>::char_([&a](){a='c';})).parse(make_test_ctx(),make_source("ab"),r);a;}) == 'c', "semact: calls if parsed");
	static_assert( ({char r='z',a='y';(t<'a'>::char_ >> t<'b'>::char_([&a](){a='c';})).parse(make_test_ctx(),make_source("au"),r);a;}) == 'y', "semact: silent if fails");
#endif
	return true;
}

template<typename parser, typename act_type, typename tag> struct exec_before_parser : base_parser<exec_before_parser<parser, act_type, tag>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto* new_result = act(search_in_ctx<tag>(ctx), result);
		return p.parse(ctx, src, *new_result);
	}
};

template<typename parser, typename act_type, typename tag> struct exec_after_parser : base_parser<exec_after_parser<parser, act_type, tag>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		act(search_in_ctx<tag>(ctx), result);
		return ret;
	}
};

template<typename tag, parser type> constexpr auto exec_before(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return exec_before_parser<type, act_type, tag>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename tag, parser type> constexpr auto exec_after(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return exec_after_parser<type, act_type, tag>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool context_parsers() {
	struct tag {};
	static_assert( []{
		char r{};
		int r_inner{};
		char val=3;
		parse(add_to_ctx<tag>(&val,
			t<'a'>::char_ >>
			exec_before<tag>([&](auto* val, auto& _p){r_inner=_p*(*val);return &_p;}, t<'b'>::char_)
		), p::make_source("ab"), r);
		return r_inner;
	}() == 'a'*3 );
	static_assert( []{
		char r{};
		int r_inner{};
		char val=3;
		parse(add_to_ctx<tag>(&val,
			t<'a'>::char_ >>
			exec_after<tag>([&](auto* val, auto& _p){r_inner=_p*(*val);}, t<'b'>::char_)
		), p::make_source("ab"), r);
		return r_inner;
	}() == 'b'*3 );

	return true;
}

} // namespace ascip_details::prs
