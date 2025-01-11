#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



#include "../base.hpp"
#include "../../details/values.hpp"
#include "common.hpp"

namespace ascip_details::prs {

template<auto ind>
struct seq_reqursion_parser : base_parser<seq_reqursion_parser<ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return !!src ? search_in_ctx<seq_stack_tag, ind>(ctx)->parse_without_prep(crop_ctx<ind, seq_crop_ctx_tag>(std::move(ctx)), static_cast<decltype(src)&&>(src), result) : -1;
	}
};

}
