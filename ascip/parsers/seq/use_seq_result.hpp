#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "common.hpp"

namespace ascip_details::prs {
template<parser parser>
struct use_seq_result_parser : base_parser<use_seq_result_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		auto& result = *search_in_ctx<seq_result_stack_tag>(ctx);
		return p.parse(ctx, src, result);
	}
};
}

namespace ascip_details {
template<parser type> constexpr auto use_seq_result(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return prs::use_seq_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) }; }
}