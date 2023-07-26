#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

struct variant_stack_tag{};
struct variant_stack_result_tag{};
template<ascip_details::parser... parsers> struct variant_parser : base_parser<variant_parser<parsers...>> {
	using self_type = variant_parser<parsers...>;
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
	template<auto item, auto pos>
	constexpr auto parse_parse_from_only(auto&& ctx, auto src, auto& result) const {
		if(!src) return -1;
		return get<item>(seq).template parse_from<pos>(ctx, src, current_result<item>(result));
	}
	template<auto ind>
	constexpr auto parse_from(auto&& ctx, auto src, auto& result) const {
		static_assert( exists_in_ctx<self_type>(decltype(auto(ctx)){}), "this method must to be called from reqursion parser" );
		return parse_ind<ind>(static_cast<decltype(ctx)&&>(ctx), src, result);
	}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (exists_in_ctx<self_type>(decltype(auto(ctx)){})) 
			return parse_ind<0>(ctx, src, result);
		else {
			auto nctx = make_ctx<variant_stack_result_tag>(&result, make_ctx<variant_stack_tag>(this, make_ctx<self_type>(this,ctx)));
			return parse_ind<0>(nctx, src, result);
		}
	}

	template<ascip_details::parser right>
	constexpr auto operator|(const right& p2) const {
		return ascip_details::init_with_get<variant_parser<parsers..., right>>(seq, p2); }
	constexpr auto operator|(char p2) { return *this | value_parser( p2 ); }
};

constexpr static const auto alpha = lower | upper;

constexpr static bool test_variant() {
	constexpr const auto run_parse = [](const auto& p, auto&& src, auto& r) ->decltype(auto(r)) {
		return p.parse(make_test_ctx(), make_source(src), r);
	};
	static_assert( ({ char r;run_parse(char_<'a'>|char_<'b'>, "a", r);r;}) == 'a' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b', "b", r);r;}) == 'b' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "c", r);}) == 1 );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "c", r);r;}) == 'c' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "d", r);}) == -1 );

	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "b", r);r;}) == 'c' );
	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "a", r);r;}) == 'a' );
	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "a", r);  }) ==  1 );

	return true;
}
