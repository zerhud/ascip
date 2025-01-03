#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


constexpr static auto test_parser_char(const auto& p, auto&& s, auto pr) {
	char result='z';
	auto answer = p.parse(make_test_ctx(), make_source(s), result);
	answer /= (answer==pr);
	return result;
}

