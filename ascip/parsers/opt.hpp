#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"

namespace ascip_details {
template<typename type> concept optional = requires(type& o){ o.emplace(); o.reset(); };
}

namespace ascip_details::prs {

template<ascip_details::parser parser> struct opt_parser : base_parser<opt_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr opt_parser(opt_parser&&) =default ;
	constexpr opt_parser(const opt_parser&) =default ;
	constexpr opt_parser() =default ;
	constexpr opt_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		using result_type = std::decay_t<decltype(result)>;

		if(!src) return 0;
		if constexpr(optional<result_type>) {
			auto ret = p.parse(ctx, src, result.emplace());
			if(ret<0) result.reset();
			return ret * (ret >= 0);
		}
		else {
			auto ret = p.parse(ctx, src, result);
			return ret * (ret >= 0);
		}
	}
};

template<parser p> constexpr auto operator-(p&& _p) { return opt_parser<std::decay_t<p>>{ _p }; }

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_optional() {
	static_assert( test_parser_char(-t<'a'>::char_, "b", 0) == 'z' );
	static_assert( test_parser_char(-t<'a'>::char_, "a", 1) == 'a' );
	return true;
}
} // namespace ascip_details::prs

