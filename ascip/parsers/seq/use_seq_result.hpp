#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "common.hpp"
#include "../use_result.hpp"

namespace ascip_details {

template<parser type> constexpr auto use_seq_result(type&& p) {
	using ptype = std::decay_t<type>;
	return prs::use_result_parser<prs::seq_result_stack_tag, ptype>{ {}, std::forward<decltype(p)>(p) };
}

}