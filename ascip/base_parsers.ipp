#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

constexpr static struct nop_parser : base_parser<nop_parser> {
	constexpr parse_result parse(auto&&, const auto&, auto&) const {
		return 0;
	}
} nop {} ;

template<auto sym> struct char_parser : base_parser<char_parser<sym>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		const bool ok = src() == sym;
		if(ok) {
			ascip_details::eq(result, sym);
			ascip_details::count_new_line(ctx, sym, result);
		}
		return -1 + (2 * ok);
	}

	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		char r;
		parse(make_test_ctx(), make_source(sym), r) == 1           || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym+1), r) == -1        || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym-1), r) == -1        || (throw __LINE__, 1);

		r=sym+1;
		(parse(make_test_ctx(), make_source(sym), r),r) == sym     || (throw __LINE__, 1);

		r=sym+1;
		(parse(make_test_ctx(), make_source(sym-1), r),r) == sym+1 || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		if constexpr (sym=='\n') {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);
		} else {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		}
#pragma GCC diagnostic pop

		return true;
	}
};
template<auto val> constexpr static const auto char_ = char_parser<val>{};

constexpr static void test_parser_char() {
	static_assert( char_<'a'>.test() ); static_assert( char_<'z'>.test() );
	static_assert( char_<'!'>.test() ); static_assert( char_<'Z'>.test() );
	static_assert( char_<' '>.test() ); static_assert( char_<'\n'>.test() );
	static_assert( []{char r;return char_<'a'>.parse(make_test_ctx(), make_source("abc"), r);}() == 1 );
	static_assert( []{char r;return char_<'b'>.parse(make_test_ctx(), make_source("abc"), r);}() == -1 );
	static_assert( []{char r;char_<'a'>.parse(make_test_ctx(), make_source("abc"), r);return r;}() == 'a' );
}

template<ascip_details::string_literal val> struct literal_parser : base_parser<literal_parser<val>> {
	constexpr parse_result parse(auto&&, auto src, auto&) const {
		//TODO: faster? add [] operator in src for direct access (operator[](auto i){return val[ind+i];})
		auto i=-1, r=0;
		{
			auto tsrc = src;
			tsrc += val.size()-1;
			if(!tsrc) return -1;
		}
		while(++i<val.size()) r += (src() == val[i]);
		return ((r+1)*(r==val.size())) - 1;
	}
};
template<ascip_details::string_literal v> constexpr static auto lit = literal_parser<v>{};
constexpr static bool test_literal_parser() {
	char r;
	static_assert( literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd"), r) == 3 );
	static_assert( literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd_tail"), r) == 3 );
	static_assert( literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("abcd"), r) == 4 );
	static_assert( literal_parser<"abcdef">{}.parse(make_test_ctx(), make_source("abcdef"), r) == 6 );
	static_assert( literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("bbcd"), r) == -1 );
	static_assert( literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("ab"), r) == -1 );
	return true;
}

template<typename t> struct value_parser : base_parser<value_parser<t>> {
	t val;
	constexpr value_parser(t v) : val(v) {}
	constexpr parse_result parse(auto&&, auto src, auto& result) const {
		const bool ok = src() == val;
		if(ok) ascip_details::eq(result, val);
		return -2 * !ok + 1;
	}

	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		char r;
		parse(make_test_ctx(), make_source(val), r) == 1           || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val+1), r) == -1        || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val-1), r) == -1        || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val), r),r) == val     || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val-1), r),r) == val+1 || (throw __LINE__, 1);

		return true;
#pragma GCC diagnostic pop
	}
};

#ifdef __clang__
template<typename t>
value_parser(t) -> value_parser<t>;
#endif

constexpr static void test_parser_value() {
	static_assert( value_parser{ 'a' }.test() ); static_assert( value_parser{ 'Z' }.test() );
	static_assert( value_parser{ L'!' }.test() ); static_assert( value_parser{ '\n' }.test() );
#ifndef __clang__
	static_assert( ({char r;value_parser{'a'}.parse(make_test_ctx(), make_source("abc"), r);}) == 1 );
	static_assert( ({char r;value_parser{'b'}.parse(make_test_ctx(), make_source("abc"), r);}) == -1 );
#endif
}

constexpr static struct space_parser : base_parser<space_parser> {
	constexpr parse_result parse(auto&& ctx,auto src, auto& r) const {
		auto sym = src();
		const bool is_space = 0x07 < sym && sym < '!'; // 0x08 is a backspace
		ascip_details::count_new_line(ctx, sym, r);
		return -1 + (2 * is_space);
	}
	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		constexpr char r=0x00;
		parse(make_test_ctx(), make_source(' '), r) == 1    || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\n'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\t'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('!'), r) == -1   || (throw __LINE__, 1);
		r == 0x00                                           || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);

		return true;
#pragma GCC diagnostic pop
	}
} space {};

constexpr static struct any_parser : base_parser<any_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = 0;
		decltype(src()) cur;
		do { 
			cur = src();
			ascip_details::eq( result, cur );
			count_new_line(ctx, cur, result);
			++ret;
		}
		while(src && (cur & 0x80)) ;
		return ret;
	}
	constexpr bool test() const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
		char r=0x00; ascip_details::type_any_eq_allow rr;
		parse(make_test_ctx(), make_source(' '), r) == 1       || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('Z'), r),r) == 'Z' || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('~'), r),r) == '~' || (throw __LINE__, 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("я"), rr)  == 2);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"я"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"яz"), rr) == 1);

		auto ctx = make_test_ctx();
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);

		return true;
#pragma GCC diagnostic pop
	}
} any {};

struct int_base_parser {
	constexpr static bool is_int(auto s) { return '0' <= s && s <= '9'; }
	constexpr static int to_int(auto s) { return s - '0'; }
	constexpr bool next(auto cur, auto& result) const {
		const bool isint = is_int(cur);
		result *= (10*isint) + !isint;
		result += to_int(cur) * isint;
		return isint;
	}
};

constexpr static struct int_parser : base_parser<int_parser>, int_base_parser {
	constexpr parse_result parse(auto&&, auto src, auto& _result)  const {
		auto sign = src();
		if(sign != '-' && sign != '+' && !this->is_int(sign)) return -1;
		int signer = -1*(sign=='-') + this->is_int(sign) + (sign=='+');
		auto& result = ascip_details::eq(_result, this->is_int(sign) * this->to_int(sign));
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
		
#ifndef __clang__
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("!"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("a"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("-"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("-["), r);}) == -1 );
#endif

		return true;
#pragma GCC diagnostic pop
	}

} int_ {};

constexpr static struct uint_parser : base_parser<uint_parser>, int_base_parser {
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
			uint_parser p{};
			p.parse(make_test_ctx(), make_source(src), r) == sym_cnt || (throw __LINE__, 1);
			return r;
		};

		static_assert( []{ int r=0; return uint_parser{}.parse(make_test_ctx(), make_source("+"), r); }() == -1 );
		static_assert( t("1", 1) == 1 );
		static_assert( t("2", 1) == 2 );
		static_assert( t("0", 1) == 0 );
		static_assert( t("103", 3) == 103 );

		return true;
#pragma GCC diagnostic pop
	}
} uint_ {};

constexpr static struct float_point_parser : base_parser<float_point_parser> {
	constexpr static auto pow(auto what, auto to) { const auto m = what; for(auto i=1;i<to;++i) what*=m; return what; }
	constexpr parse_result parse(auto&& ctx, auto src, auto& result)  const {
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
		float_point_parser p; 
		auto t = [](auto&& src, auto sym_cnt, auto answer) {
			float_point_parser p; 
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
} fp {};

template<auto from, auto to> struct range_parser : base_parser<range_parser<from,to>> { 
	constexpr parse_result parse(auto&&, auto src, auto& result) const {
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
#ifndef __clang__
	static_assert( ({char r;lower.parse(make_test_ctx(), make_source("a"), r);r;}) == 'a' );
	static_assert( ({char r;lower.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source("A"), r);}) == 1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source('~'+1), r);}) == 1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source('~'+2), r);}) == -1 );
#endif
	return true;
}

