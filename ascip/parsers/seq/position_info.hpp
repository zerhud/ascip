#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../base.hpp"
#include "../../details/values.hpp"
#include "common.hpp"

namespace ascip_details::prs {

template<auto ind> struct seq_shift_parser : base_parser<seq_shift_parser<ind>>, any_shift_parser_tag {
	constexpr static bool is_special_info_parser=true;
	constexpr parse_result parse(auto&& ctx, auto, auto& result) const {
		eq(result, *search_in_ctx<ind, seq_shift_stack_tag>(ctx));
		return 0;
	}
};

//TODO: dose we realy need the pos parser?
struct cur_pos_parser : base_parser<cur_pos_parser> {
	constexpr parse_result parse(auto&&, auto src, auto& result) const {
		//TODO: extract the info from context or from parent's object
		//      sequence may sotre it in context
		//      sequence may have mutable field and
		//        pass it to parse method here or
		//        store it to result on it's own
		//      if use context - make it with tags
		eq(result, pos(src));
		return 0;
	}
};

}
