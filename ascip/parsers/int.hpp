#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/context.hpp"
#include "../details/make_source.hpp"

namespace ascip_details::prs {

template<auto base=10> struct int_base_parser {
	constexpr static bool is_int(auto s) {
		constexpr bool is_0 = base <= 10;
		return
			   ('0' <= s && s <= '9')
			|| (!is_0 && ('a' <= s && s <= ('a'+base-10)))
			|| (!is_0 && ('A' <= s && s <= ('A'+base-10)))
			;
	}
	constexpr static int to_int(auto s) {
		const bool is_a = 'a' <= s && s <= 'z';
		const bool is_A = 'A' <= s && s <= 'Z';
		const bool is_0 = '0' <= s && s <= '9';
		return
			  is_0 * (s - '0')
			+ is_a * (s - 'a' + 10)
			+ is_A * (s - 'A' + 10)
			;
	}
	constexpr bool next(auto cur, auto& result) const {
		const bool isint = is_int(cur);
		result *= (base*isint) + !isint;
		result += to_int(cur) * isint;
		return isint;
	}
};

struct int_parser : base_parser<int_parser>, int_base_parser<> {
	constexpr parse_result parse(auto&&, auto src, auto& _result)  const {
		auto sign = src();
		if(sign != '-' && sign != '+' && !this->is_int(sign)) return -1;
		int signer = -1*(sign=='-') + this->is_int(sign) + (sign=='+');
		ascip_details::eq(_result, this->is_int(sign) * this->to_int(sign));
		auto& result = _result;
		auto ret = 1;
		while(src && this->next(src(), result)) ++ret;
		result *= signer;
		bool bad_result = ((sign=='-')+(sign=='+')+(ret==1))==2;
		return -1*bad_result + ret*!bad_result;
	}
	constexpr auto parse_without_preparation(auto src, auto& result) const {
		auto ret = 0;
		while(src && this->next(src(), result)) ++ret;
		return ret;
	}

	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		auto t = [](auto&& src, auto sym_cnt, auto answer) {
			auto r = answer + 100;
			int_parser p{};
			p.parse(make_test_ctx(), make_source(src), r) == sym_cnt || (throw __LINE__, 1);
			r == answer                                              || (throw __LINE__, 1);
			return true;
		};

		static_assert( t("-1", 2, -1) );
		static_assert( t("1", 1, 1) );
		static_assert( t("+2", 2, 2) );
		static_assert( t("+0", 2, 0) );
		static_assert( t("0", 1, 0) );
		static_assert( t("+103", 4, 103) );
		static_assert( t("103", 3, 103) );

#ifndef __clang__
		auto r=0;
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("!"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("a"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("-"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("-["), r);}) == -1 );
#endif

		return true;
#pragma GCC diagnostic pop
	}

};

template<auto base> struct uint_parser : base_parser<uint_parser<base>>, int_base_parser<base> {
	constexpr auto parse(auto&&, auto src, auto& result) const {
		auto ret = 0;
		while(src && this->next(src(), result)) ++ret;
		return ret - (ret==0);
	}

	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		uint_parser p; auto r=0;

		auto t = [](auto&& src, auto sym_cnt) {
			int r=0;
			uint_parser<10> p{};
			p.parse(make_test_ctx(), make_source(src), r) == sym_cnt || (throw __LINE__, 1);
			return r;
		};

		static_assert( []{ int r=0; return uint_parser<10>{}.parse(make_test_ctx(), make_source("+"), r); }() == -1 );
		static_assert( t("1", 1) == 1 );
		static_assert( t("2", 1) == 2 );
		static_assert( t("0", 1) == 0 );
		static_assert( t("103", 3) == 103 );

		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("A"), r); return r; }() == 10 );
		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("A3"), r); return r; }() == 163 );
		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("FD"), r); return r; }() == 253 );
		static_assert( []{ int r=0; return uint_parser<16>{}.parse(make_test_ctx(), make_source("FD"), r); }() == 2 );

		return true;
#pragma GCC diagnostic pop
	}
} ;

} // namespace ascip_details::prs
