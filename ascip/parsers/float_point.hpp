#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "int.hpp"
#include "../details/context.hpp"
#include "../details/make_source.hpp"

namespace ascip_details::prs {

struct float_point_parser : base_parser<float_point_parser> {
	constexpr static auto pow(auto what, auto to) { const auto m = what; for(auto i=1;i<to;++i) what*=m; return what; }
	constexpr parse_result parse(auto&&, auto src, auto& result)  const {
		constexpr int_parser int_{};
		result = 0;
		auto int_pos = src;
		auto dec_pos = src;
		auto left_result = int_.parse_without_preparation(int_pos, result);
		if(left_result <= 0 && int_pos() != '.') return -1;
		dec_pos+=left_result;
		if(!dec_pos || dec_pos()!='.') return -1;
		auto right_result = int_.parse_without_preparation(dec_pos, result);
		if(right_result <= 0) return -1;
		result /= pow(10, right_result);
		return left_result + right_result + 1;
	}
	constexpr bool test() const {
		constexpr float_point_parser p{};
		auto t = [](auto&& src, auto sym_cnt, auto answer) {
			const float_point_parser p{};
			auto r = answer + 100;
			auto pr = p.parse(make_test_ctx(), make_source(src), r);
		       	pr /= (pr == sym_cnt);
			r /= (r == answer);
			return true;
		};

		static_assert( p.pow(10, 1) == 10 );
		static_assert( p.pow(10, 2) == 100 );
		static_assert( p.pow(10, 3) == 1000 );
		static_assert( t("0.5", 3, 0.5) );
		static_assert( t("1.5", 3, 1.5) );
		static_assert( t("3.075", 5, 3.075) );

#ifndef __clang__
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("0"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("a"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("1."), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("5+3"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("5-3"), r); }) == -1);
#endif

		return true;
	}
};
} // namespace ascip_details::prs

