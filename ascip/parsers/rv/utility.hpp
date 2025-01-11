#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include "common.hpp"
#include "req.hpp"
#include "../../details/hana.hpp"

namespace ascip_details::prs {

template<typename cur_parser_t> constexpr bool is_term() {
	auto checker = [](const auto* p){return std::is_same_v<std::decay_t<decltype(*p)>, rvariant_lreq_parser>;};
	auto stop = [](const auto* p){
		const bool is_rv = requires{ p->maker; };
		return is_rv && !is_specialization_of<cur_parser_t, rvariant_parser>;
	};
	return !exists_in((cur_parser_t*)nullptr, checker, stop);
}

}
