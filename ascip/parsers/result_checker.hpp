#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/context.hpp"

namespace ascip_details::prs {

template<typename good_result, parser parser>
struct result_checker_parser : base_parser<result_checker_parser<good_result, parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert(
			   std::is_same_v<type_any_eq_allow, std::decay_t<decltype(result)>>
			|| std::is_same_v<good_result, std::decay_t<decltype(result)>>
			, "can only parser to required type" );
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(auto(src))&&>(src), result);
	}
};
template<typename needed, parser type> struct cast_parser : base_parser<cast_parser<needed,type>> {
	type p;
	constexpr static auto& check_result(auto& result) {
		if constexpr( std::is_same_v<std::decay_t<decltype(result)>, type_any_eq_allow> ) return result;
		else {
			static_assert(requires{ static_cast<needed&>(result); }, "the result must to be castable to needed type" );
			return static_cast<needed&>(result);
		}
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(ctx, src, check_result(result));
	}
};

template<typename result, parser type> constexpr auto check(type&& p) {
	return result_checker_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename result, parser type> constexpr auto cast(type&& p){
	return cast_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_checkers() {
  constexpr auto ca = t<'a'>::char_;
	//NOTE: for c++23 we can only test if it works and cannot test if it fails
	static_assert( requires(char& r) {
		check<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(char& r) {
		cast<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(type_any_eq_allow& r) {
		check<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(type_any_eq_allow& r) {
		cast<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	return true;
}

}

namespace ascip_details {
template<typename mutator, typename type, typename parser>
constexpr static auto transform_special(prs::cast_parser<type,parser>&& src, auto& ctx) {
	return transform_apply<mutator>(cast<type>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
template<typename mutator, typename type, typename parser>
constexpr static auto transform_special(prs::result_checker_parser<type,parser>&& src, auto& ctx) {
	return transform_apply<mutator>(check<type>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
}
