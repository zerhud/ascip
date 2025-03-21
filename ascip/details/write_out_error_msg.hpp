#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include "string_literal.hpp"

namespace ascip_details {

template<typename char_type, auto sz>
constexpr auto& print_to(auto& os, const ascip_details::string_literal<char_type, sz>& what) { return os << what.value; }
constexpr auto& print_to(auto& os, const auto& what) { return os << what; }
constexpr void write_out_error_msg(
		auto& os,
		auto fn,
		auto msg,
		auto expt,
		auto src,
		auto ln
		) {
	os << "Error in ";
	print_to(os, fn) << ':' << ln << ' ';
	print_to(os, msg) << "\n";
	auto shift = src.back_to_nl();
	for(auto cur = src(); src; cur = src()) os << cur;
	os << "\n";
	for(auto i=0;i<shift;++i) os << '-';
	os << "^\n";
}

}