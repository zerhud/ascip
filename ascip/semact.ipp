#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<typename parser, typename act_t> struct semact_parser : base_parser<semact_parser<parser,act_t>> {
	act_t act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr bool is_any_arg_pattern = !requires{ act(); };
		if constexpr(ascip_details::is_in_concept_check(decltype(auto(ctx)){})) return 0;
		else if constexpr(std::is_same_v<ascip_details::type_any_eq_allow&, decltype(result)>)
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
				if constexpr (requires{ act(ret, ctx, src, result); }) act(ret, ctx, src, result);
				else if constexpr (requires{ act(ret, result); }) act(ret, result);
				else act();
			}
			return ret;
		}
	}
};

constexpr static bool test_semact() {
#ifndef __clang__
	static_assert( ({char r='z',a='y';(char_<'a'> >> char_<'b'>([&a](){a='c';})).parse(make_test_ctx(),make_source("ab"),r);a;}) == 'c', "semact: calls if parsed");
	static_assert( ({char r='z',a='y';(char_<'a'> >> char_<'b'>([&a](){a='c';})).parse(make_test_ctx(),make_source("au"),r);a;}) == 'y', "semact: silent if fails");
#endif
	return true;
}

template<typename parser, typename tag, typename value_type> struct ctx_change_parser : base_parser<ctx_change_parser<parser, tag, value_type>> {
	value_type value;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto new_ctx = make_ctx<tag>(value, ctx);
		return p.parse(new_ctx, src, result);
	}
};

template<typename act_type, typename parser, typename... tags> struct ctx_use_parser : base_parser<ctx_use_parser<act_type, parser, tags...>> {
	act_type act;
	[[no_unique_address]] parser p;
	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		if(-1 < ret) act(result, by_ind_from_ctx<0, tags>(ctx)...); 
		return ret;
	}
};

template<typename parser, typename tag, typename act_type> struct exec_before_parser : base_parser<exec_before_parser<parser, tag, act_type>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto* new_result = act(search_in_ctx<tag>(ctx), result);
		return p.parse(ctx, src, *new_result);
	}
};

template<typename parser, typename tag, typename act_type> struct exec_after_parser : base_parser<exec_after_parser<parser, tag, act_type>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		act(search_in_ctx<tag>(ctx), result);
		return ret;
	}
};
