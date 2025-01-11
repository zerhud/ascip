#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../base.hpp"
#include "../../details/hana.hpp"

namespace ascip_details::prs {

template<parser parser> struct rvariant_top_result_parser : base_parser<rvariant_top_result_parser<parser>> { parser p; };
template<typename p> rvariant_top_result_parser(p) -> rvariant_top_result_parser<p>;

template<parser type> constexpr auto rv_result(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return rvariant_top_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) };
}

template<template<class>class wrapper, parser parser> constexpr static auto is_top_result_parser_helper(const wrapper<parser>& p) -> decltype(p.p);
template<typename parser>
constexpr static bool is_top_result_parser() {
	if constexpr (requires{ is_top_result_parser_helper(std::declval<parser>()); })
		return
			   is_top_result_parser<decltype(is_top_result_parser_helper(std::declval<parser>()))>()
			|| is_specialization_of<parser, rvariant_top_result_parser>;
	else return is_specialization_of<parser, rvariant_top_result_parser>;
}

}
