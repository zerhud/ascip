#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<auto sym> struct char_parser : base_parser<char_parser<sym>> {
	constexpr auto parse(auto&&, auto src, auto& result) const {
		const bool ok = src() == sym;
		if(ok) ascip_details::eq(result, sym);
		return -1 + (2 * ok);
	}

	constexpr bool test() const {
		char r;
		parse(make_test_ctx(), make_source(sym), r) == 1           || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym+1), r) == -1        || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym-1), r) == -1        || (throw __LINE__, 1);

		r=sym+1;
		(parse(make_test_ctx(), make_source(sym), r),r) == sym     || (throw __LINE__, 1);

		r=sym+1;
		(parse(make_test_ctx(), make_source(sym-1), r),r) == sym+1 || (throw __LINE__, 1);

		return true;
	}
};
template<auto val> constexpr static const auto char_ = char_parser<val>{};

constexpr static void test_parser_char() {
	static_assert( char_<'a'>.test() ); static_assert( char_<'z'>.test() );
	static_assert( char_<'!'>.test() ); static_assert( char_<'Z'>.test() );
	static_assert( ({char r;char_<'a'>.parse(make_test_ctx(), make_source("abc"), r);}) == 1 );
	static_assert( ({char r;char_<'b'>.parse(make_test_ctx(), make_source("abc"), r);}) == -1 );
}

template<typename t> struct value_parser : base_parser<value_parser<t>> {
	t val;
	constexpr value_parser(t v) : val(v) {}
	constexpr auto parse(auto&&, auto src, auto& result) const {
		const bool ok = src() == val;
		if(ok) ascip_details::eq(result, val);
		return -2 * !ok + 1;
	}

	constexpr bool test() const {
		char r;
		parse(make_test_ctx(), make_source(val), r) == 1           || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val+1), r) == -1        || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val-1), r) == -1        || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val), r),r) == val     || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val-1), r),r) == val+1 || (throw __LINE__, 1);

		return true;
	}
};

constexpr static void test_parser_value() {
	static_assert( value_parser{ 'a' }.test() ); static_assert( value_parser{ 'Z' }.test() );
	static_assert( value_parser{ L'!' }.test() ); static_assert( value_parser{ '\n' }.test() );
	static_assert( ({char r;value_parser{'a'}.parse(make_test_ctx(), make_source("abc"), r);}) == 1 );
	static_assert( ({char r;value_parser{'b'}.parse(make_test_ctx(), make_source("abc"), r);}) == -1 );
}

constexpr static struct space_parser : base_parser<space_parser> {
	constexpr auto parse(auto&&,auto src, auto& r) const {
		auto sym = src();
		const bool is_space = 0x07 < sym && sym < '!'; // 0x08 is a backspace
		return -1 + (2 * is_space);
	}
	constexpr bool test() const {
		constexpr char r=0x00;
		parse(make_test_ctx(), make_source(' '), r) == 1    || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\n'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\t'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('!'), r) == -1   || (throw __LINE__, 1);
		r == 0x00                                           || (throw __LINE__, 1);
		return true;
	}
} space {};

constexpr static struct any_parser : base_parser<any_parser> {
	constexpr auto parse(auto&&, auto src, auto& result) const {
		auto ret = 0;
		decltype(src()) cur;
		do { 
			cur = src();
			ascip_details::eq( result, cur );
			++ret;
		}
		while(src && (cur & 0x80)) ;
		return ret;
	}
	constexpr bool test() const {
		char r=0x00; ascip_details::type_any_eq_allow rr;
		parse(make_test_ctx(), make_source(' '), r) == 1       || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('Z'), r),r) == 'Z' || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('~'), r),r) == '~' || (throw __LINE__, 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("я"), rr)  == 2);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"я"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"яz"), rr) == 1);
		return true;
	}
} any {};

constexpr static struct int_parser : base_parser<int_parser> {
	constexpr bool is_int(auto s) const { return '0' <= s && s <= '9'; }
	constexpr bool next(auto cur, auto& result) const {
		const bool isint = is_int(cur);
		result += ((int)(cur - '0')) * isint;
		result /= (!isint * 9) + 1;
		return isint;
	}
	constexpr auto parse(auto&&, auto src, auto& _result)  const {
		auto sign = src();
		if(sign != '-' && sign != '+' && !is_int(sign)) return -1;
		int signer = sign == '-' ? -1 : is_int(sign) || sign=='+';
		auto& result = ascip_details::eq(_result, is_int(sign) ? sign-'0' : 0);
		auto ret = 1;
		while(src && next(src(), result *= 10)) ++ret;
		result *= signer;
		return ret;
	}

	constexpr bool test() const {
		int_parser p; auto r=0;

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
		
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("!"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("a"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("A"), r);}) == -1 );

		return true;
	}

} int_ {};

template<auto from, auto to> struct range_parser : base_parser<range_parser<from,to>> { 
	constexpr auto parse(auto&&, auto src, auto& result) const {
		auto sym = src();
		const bool ok = from <= sym && sym <= to;
		if(ok) ascip_details::eq( result, sym );
		return -2 * !ok + 1;
	}

};
constexpr static const auto lower = range_parser<'a','z'>{};
constexpr static const auto upper = range_parser<'A','Z'>{};
constexpr static const auto digit = range_parser<'0','9'>{};
constexpr static const auto d10 = range_parser<'0', '9'>{};
constexpr static const auto ascii = range_parser<(char)0x01,(char)0x7F>{};

constexpr static bool test_range_parser() {
	static_assert( ({char r;lower.parse(make_test_ctx(), make_source("a"), r);r;}) == 'a' );
	static_assert( ({char r;lower.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source("A"), r);}) == 1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source('~'+1), r);}) == 1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source('~'+2), r);}) == -1 );
	return true;
}

