#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



#include "../base.hpp"
#include "../../details/values.hpp"
#include "common.hpp"

namespace ascip_details::prs {

struct cur_shift_parser : base_parser<cur_shift_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		eq(result, *search_in_ctx<seq_shift_stack_tag>(ctx));
		return 0;
	}
};

template<auto ind>
struct seq_reqursion_parser : base_parser<seq_reqursion_parser<ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return !!src ? search_in_ctx<seq_stack_tag, ind>(ctx)->parse_without_prep(crop_ctx<ind, seq_crop_ctx_tag>(std::move(ctx)), static_cast<decltype(src)&&>(src), result) : -1;
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
