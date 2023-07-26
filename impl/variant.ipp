#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<ascip_details::parser... parsers> struct variant_parser : base_parser<variant_parser<parsers...>> {
	tuple<parsers...> seq;
	constexpr variant_parser( parsers... l ) : seq( std::forward<parsers>(l)... ) {}

	template<auto ind> constexpr auto& current_result(auto& result) const
	requires requires{ create<ind>(result); } {
		return create<ind>(result);
	}
	template<auto ind> constexpr auto& current_result(auto& result) const
	requires (requires{ result.template emplace<ind>(); } && !requires{ create<ind>(result); }) {
		return result.template emplace<ind>();
	}
	template<auto ind> constexpr auto& current_result(auto& result) const {
		return result;
	}
	template<auto ind> constexpr auto parse_ind(auto&& ctx, auto& src, auto& result) const {
		auto parse_result = get<ind>(seq).parse(ctx, src, current_result<ind>(result));
		if constexpr (ind+1 == sizeof...(parsers)) return parse_result;
		else {
			if(parse_result > 0) return parse_result;
			return parse_ind<ind+1>(ctx, src, result);
		}
	}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		return parse_ind<0>(ctx, src, result);
	}

	template<ascip_details::parser right>
	constexpr auto operator|(const right& p2) const {
		return ascip_details::init_with_get<variant_parser<parsers..., right>>(seq, p2); }
	constexpr auto operator|(char p2) { return *this | value_parser( p2 ); }
};

constexpr static bool test_variant() {
	constexpr const auto run_parse = [](const auto& p, auto&& src, auto& r) ->decltype(auto(r)) {
		return p.parse(make_test_ctx(), make_source(src), r);
	};
	static_assert( ({ char r;run_parse(char_<'a'>|char_<'b'>, "a", r);r;}) == 'a' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b', "b", r);r;}) == 'b' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "c", r);}) == 1 );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "c", r);r;}) == 'c' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "d", r);}) == -1 );
	return true;
}
