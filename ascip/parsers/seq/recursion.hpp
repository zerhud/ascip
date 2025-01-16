#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



#include "../base.hpp"
#include "../../details/values.hpp"
#include "common.hpp"

namespace ascip_details::prs {

struct seq_enable_recursion_parser : base_parser<seq_enable_recursion_parser> {
	constexpr static parse_result parse(auto&&, auto&, auto&) { return 0; }
};

template<auto ind>
struct seq_recursion_parser : base_parser<seq_recursion_parser<ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		const auto* req = *search_in_ctx<seq_stack_tag, ind>(ctx);
		return src ? req->call_parse(src, result) : -1 ;
	}
};

}
