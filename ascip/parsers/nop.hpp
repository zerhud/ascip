#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "base.hpp"

namespace ascip_details::prs {

struct nop_parser : base_parser<nop_parser> {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&&, const auto&, auto&) {
		return 0;
	}
};

} // namespace ascip_details::prs
