#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

struct variant_pos_tag{};
struct variant_stack_tag{};
struct variant_stack_result_tag{};
template<ascip_details::parser parser> struct use_variant_result_parser : base_parser<use_variant_result_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}
};
template<ascip_details::parser... parsers> struct variant_parser : base_parser<variant_parser<parsers...>> {
	using self_type = variant_parser<parsers...>;
	tuple<parsers...> seq;

	template<typename... left, ascip_details::parser right>
	constexpr static auto mk(variant_parser<left...> l, right&& r) {
		return [&]<auto... inds>(std::index_sequence<inds...>){
			return variant_parser<left..., std::decay_t<right>>{ std::move(get<inds>(l.seq))..., std::forward<decltype(r)>(r) };
		}(std::make_index_sequence<sizeof...(left)>{});
	}

	constexpr variant_parser(const variant_parser& other) : seq(other.seq) {}
	constexpr variant_parser(variant_parser&& other) : seq(std::move(other.seq)) {}
	constexpr explicit variant_parser( parsers... l ) : seq( std::forward<parsers>(l)... ) {}

	template<auto ind, auto cnt, auto cur, typename cur_parser, typename... tail>
	constexpr static auto _cur_ind() {
		constexpr const bool skip = ascip_details::is_specialization_of<cur_parser, use_variant_result_parser>;
		if constexpr (ind == cnt) {
			if constexpr (skip) return -1;
			else return cur;
		}
		else return _cur_ind<ind,cnt+1,cur+(!skip),tail...>();
	}
	template<auto ind> consteval static auto cur_ind() { return _cur_ind<ind,0,0,parsers...>(); }
	template<auto ind> constexpr auto parse_ind(auto&& ctx, auto& src, auto& result) const {
		auto prs = [&](auto&& r){ return get<ind>(seq).parse(ctx, src, ascip_details::variant_result<cur_ind<ind>()>(r)); };
		if constexpr (ind+1 == sizeof...(parsers)) return prs(result);
		else {
			auto parse_result = prs(ascip_details::type_any_eq_allow{});
			if(parse_result > 0) return prs(result);
			return parse_ind<ind+1>(ctx, src, result);
		}
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (exists_in_ctx<self_type>(decltype(auto(ctx)){})) 
			return parse_ind<0>(ctx, src, result);
		else {
			auto variant_ctx =
				make_ctx<variant_stack_result_tag>(&result,
					make_ctx<variant_stack_tag>(this, ctx)
				)
			;
			auto nctx = make_ctx<self_type>(&variant_ctx, variant_ctx);
			return parse_ind<0>(nctx, src, result);
		}
	}

	constexpr auto clang_crash_workaround(auto r) {
		return std::move(*this) | value_parser(r);
	}
};

template<ascip_details::parser... parsers>
variant_parser(parsers...) -> variant_parser<parsers...>;

constexpr static const auto alpha = lower | upper;

constexpr static bool test_variant() {
	constexpr const auto run_parse = [](const auto& p, auto&& src, auto& r) ->decltype(auto(r)) {
		return p.parse(make_test_ctx(), make_source(src), r);
	};
	static_assert( ascip_details::variant_parser<variant_parser<char_parser<'a'>, char_parser<'b'>>> );
	static_assert( ((void)static_cast<const variant_parser<char_parser<'a'>, char_parser<'b'>, char_parser<'c'>>&>(char_<'a'> | char_<'b'> | char_<'c'>),1) );
#ifndef __clang__
	static_assert( ({ char r;run_parse(char_<'a'>|char_<'b'>, "a", r);r;}) == 'a' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b', "b", r);r;}) == 'b' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "c", r);}) == 1 );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "c", r);r;}) == 'c' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "d", r);}) == -1 );

	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "b", r);r;}) == 'c' );
	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "a", r);r;}) == 'a' );
	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "a", r);  }) ==  1 );
#endif

	return true;
}
