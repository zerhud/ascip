#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../base.hpp"

namespace ascip_details::prs {

template<parser pt> struct lexeme_parser : base_parser<lexeme_parser<pt>> { [[no_unique_address]] pt p;
	constexpr lexeme_parser() =default ;
	constexpr lexeme_parser(lexeme_parser&&) =default ;
	constexpr lexeme_parser(const lexeme_parser&) =default ;
	constexpr lexeme_parser(pt p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result);
	}
};
template<parser pt> struct skip_parser : base_parser<skip_parser<pt>> { [[no_unique_address]] pt p;
	constexpr skip_parser() =default ;
	constexpr skip_parser(skip_parser&&) =default ;
	constexpr skip_parser(const skip_parser&) =default ;
	constexpr skip_parser(pt p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result);
	}
};

template<parser skip, parser base> struct injected_parser : base_parser<injected_parser<skip,base>> {
	[[no_unique_address]] skip s;
	[[no_unique_address]] base b;
	static type_parse_without_result skip_result;
	constexpr injected_parser() noexcept =default ;
	constexpr injected_parser(injected_parser&&) noexcept =default ;
	constexpr injected_parser(const injected_parser&) noexcept =default ;
	constexpr injected_parser(skip s, base b) : s(std::forward<decltype(s)>(s)), b(std::forward<decltype(b)>(b)) {}
	constexpr static auto& get_result_for_skipper(auto& result) {
		if constexpr(requires{is_checking(result).ok;}) return result;
		else return skip_result;
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto sr = s.parse(ctx, src, get_result_for_skipper(result));
		sr *= (0<=sr);
		src += sr;
		if(!src) return -1;
		auto mr = b.parse(ctx, src, result);
		return (sr * (0<=mr)) + mr; // 0<=mr ? mr+sr : mr;
	}
};

template<parser skip, parser base> type_parse_without_result injected_parser<skip, base>::skip_result{};

} // namespace ascip_details::prs

namespace ascip_details {

template<parser type> constexpr auto lexeme(type&& p) {
	return prs::lexeme_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) };
}

template<parser type> constexpr auto skip(type&& p) {
	return prs::skip_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) };
}

}
