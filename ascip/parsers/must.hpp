#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/context.hpp"
#include "../details/string_literal.hpp"

namespace ascip_details::prs {

template<string_literal msg, parser type> struct must_parser : base_parser<must_parser<msg, type>> {
  type p;
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
  	return call_if_error(ctx, result, ret);
  }

	constexpr static auto call_if_error(auto& ctx, auto& result, auto orig_ret) {
  	if (0 <= orig_ret) return orig_ret;
		auto err = search_in_ctx<err_handler_tag>(ctx);
  	static_assert( !std::is_same_v<std::decay_t<decltype(err)>, ctx_not_found_type>, "for using the must parser a error handler is required" );
  	if constexpr(requires{(*err)(result, 0, msg);}) return (*err)(result, new_line_count(ctx), msg);
  	else return (*err)(result, msg);
  }
};

template<parser left, typename right> constexpr auto operator>(left&& l, right&& r) {
	return l >> must<"unknown">(std::forward<decltype(r)>(r));
}

template<string_literal msg> constexpr auto must(parser auto&& p) {
  return must_parser<msg, std::decay_t<decltype(p)>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_must_parser() {
	static_assert( []{
		char r='z';
		const auto err_method = [&](...){return -10;};
		return (t<'a'>::char_ >> t<'b'>::char_ >> must<"t">(t<'c'>::char_)).parse(make_test_ctx(&err_method), make_source("abe"), r);
	}() == -10, "error lambda are called");

	const auto err_method = [](
			auto&,
			auto line_number,
			auto message){
		line_number /= (line_number==2);
		return (-3 * (message=="unknown")) + (-4 * (message=="test"));
	};
	static_assert( [&]{ char r=0x00;
		return (p::any >> t<'a'>::char_ >> t<'b'>::char_ > t<'c'>::char_).parse(make_test_ctx(&err_method), make_source("\nabe"), r);
	}() == -3, "on error: sources are on start sequence and on rule where the error");
	static_assert( [&]{ char r=0x00;
		return (p::any >> t<'a'>::char_ >> t<'b'>::char_ >> must<"test">(t<'c'>::char_)).parse(make_test_ctx(&err_method), make_source("\nabe"), r);
	}() == -4, "on error: sources are on start sequence and on rule where the error");

  return true;
}

} // namespace ascip_details::prs
