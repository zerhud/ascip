#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<typename parser, typename act_t> struct semact_parser : base_parser<semact_parser<parser,act_t>> {
	act_t act;
	[[no_unique_address]] parser p;

	constexpr const auto parse(auto&& ctx, auto src, auto& result) const {
		constexpr bool is_any_arg_pattern = !requires{ act(); };
		if constexpr(ascip_details::is_in_concept_check(decltype(auto(ctx)){})) return 0;
		else if constexpr(std::is_same_v<ascip_details::type_any_eq_allow&, decltype(result)>)
			return p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
		else if constexpr (requires{ act(result); requires std::is_lvalue_reference_v<decltype(act(result))>; } ) {
			auto& nr = act(result);
			if constexpr (requires{ p.parse_with_user_result(ctx,src,nr); })
				return p.parse_with_user_result(static_cast<decltype(ctx)&&>(ctx),src,nr);
			else return p.parse(ctx, src, nr);
		}
		else if constexpr(requires{ act(result); requires std::is_pointer_v<decltype(act(result))>;} ) {
			auto* nr = act(result);
			if constexpr (requires{ p.parse_with_user_result(ctx,src,*nr); })
				return p.parse_with_user_result(static_cast<decltype(ctx)&&>(ctx),src,*nr);
			else return p.parse(ctx, src, *nr);
		}
		else {
			auto ret = p.parse(ctx, src, result);
			if(ret >= 0) {
				if constexpr (requires{ act(ret, ctx, src, result); }) act(ret, ctx, src, result);
				else if constexpr (requires{ acct(ret, result); }) act(ret, result);
				else act();
			}
			return ret;
		}
	}
	/*
	constexpr const auto parse(auto&& ctx, auto src, auto& result) const requires
		std::is_same_v<ascip_details::type_any_eq_allow&, decltype(result)>
	{
		return p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}
	constexpr const auto parse(auto&& ctx, auto src, auto& result) const requires (
		   requires{ static_cast<const ascip_details::type_result_for_parser_concept&>(result); }
		&& !std::is_same_v<ascip_details::type_any_eq_allow&, decltype(result)>
	)
	{ return 0; }
	constexpr const auto parse(auto&& ctx, auto src, auto& result) const requires (
		   !ascip_details::is_in_concept_check(decltype(auto(ctx)){})
		&& !std::is_same_v<ascip_details::type_any_eq_allow&, decltype(result)>
		&& !requires{ static_cast<const ascip_details::type_result_for_parser_concept&>(result); }
		&& requires{ requires std::is_lvalue_reference_v<decltype(act(result))>; }
		&& !requires{ act(); /* check if ... pattern */ /*}
	) {
		auto& nr = act(result);
		if constexpr (requires{ p.parse_with_user_result(ctx,src,nr); })
			return p.parse_with_user_result(static_cast<decltype(ctx)&&>(ctx),src,nr);
		else return p.parse(ctx, src, nr);
	}
	constexpr const auto parse(auto&& ctx, auto src, auto& result) const requires (
		   !ascip_details::is_in_concept_check(decltype(auto(ctx)){})
		&& !std::is_same_v<ascip_details::type_any_eq_allow&, decltype(result)>
		&& requires{ requires std::is_pointer_v<decltype(act(result))>; }
		&& !requires{ act(); /* check if ... pattern */ /*}
	) {
		auto* nr = act(result);
		if constexpr (requires{ p.parse_with_user_result(ctx,src,*nr); })
			return p.parse_with_user_result(static_cast<decltype(ctx)&&>(ctx),src,*nr);
		else return p.parse(ctx, src, *nr);
	}
	constexpr const auto parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (ascip_details::is_in_concept_check(decltype(auto(ctx)){})) return 0;
		else {
			auto ret = p.parse(ctx, src, result);
			if(ret >= 0) {
				if constexpr (requires{ act(ret, ctx, src, result); }) act(ret, ctx, src, result);
				else if constexpr (requires{ acct(ret, result); }) act(ret, result); 
				else act();
			}
			return ret;
		}
	}
	*/
};

constexpr static bool test_semact() {
#ifndef __clang__
	static_assert( ({char r='z',a='y';(char_<'a'> >> char_<'b'>([&a](){a='c';})).parse(make_test_ctx(),make_source("ab"),r);a;}) == 'c', "semact: calls if parsed");
	static_assert( ({char r='z',a='y';(char_<'a'> >> char_<'b'>([&a](){a='c';})).parse(make_test_ctx(),make_source("au"),r);a;}) == 'y', "semact: silent if fails");
#endif
	return true;
}
