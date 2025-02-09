//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "factory.hpp"

namespace prs = ascip_details::prs;

constexpr void test_seq_simple_case() {
	constexpr auto p_ab = char_<'a'> >> char_<'b'>;
	static_assert( test_parser_parse_r_str(p_ab, "ab", 2, 'a', 'b') );
	static_assert( test_parser_parse_r_str(p_ab, "ac", -1, 'a'), "we store parsed part even on fail" );
	static_assert( test_parser_parse_r_str(p_ab, "abc", 2, 'a', 'b'), "don't catch a tail" );
	static_assert( test_parser_parse_r_str(p_ab, "cab", -1) );
	static_assert( test_parser_parse_r_str(p::any >> omit(p::space) >> p::any, "1 2", 3, '1', '2') );
	static_assert( test_parser_parse_r_str(char_<'a'> >> 'b', "ab", 2, 'a', 'b') );
}

constexpr void test_seq_result_fields() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
	struct with_2_chars { char a, b; };
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>, "ab", 2), 'a', 'b' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, ++char_<'a'> >> --char_<'b'>, "ab", 2), 'b', 'a' ) );
	static_cast<const prs::opt_seq_parser<prs::char_parser<'a'>, prs::char_parser<'b'>, prs::char_parser<'c'>>&>(char_<'a'> >> char_<'b'> >> char_<'c'>);
	static_cast<const prs::opt_seq_parser<prs::char_parser<'a'>, prs::int_parser, prs::char_parser<'c'>, prs::int_parser>&>(char_<'a'> >> p::int_ >> char_<'c'> >> p::int_);
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>-- >> char_<'c'>, "abc", 3), 'c', 'b' ) );
#pragma GCC diagnostic pop
}

constexpr void test_seq_finc() {
	struct with_3_chars { char a, b, c; };
	static_assert( test_cmp_struct( test_parser_parse(with_3_chars{}, finc<2>(char_<'a'>) >> --char_<'b'> >> --char_<'c'>, "abc", 3), 'c', 'b', 'a' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_3_chars{}, ++char_<'a'> >> ++char_<'b'> >> finc<-2>(char_<'c'>), "abc", 3), 'c', 'a', 'b' ) );
}

constexpr void test_seq_single_field() {
	struct with_1_field { std::string val; };
	static_assert( test_parser_parse(with_1_field{}, fnum<0>(char_<'a'>) >> char_<'b'>, "ab", 2).val[0] == 'a' );
	static_assert( test_parser_parse(with_1_field{}, finc<0>(char_<'a'>) >> char_<'b'>, "ab", 2).val[1] == 'b' );
	static_assert( test_parser_parse(with_1_field{}, p::sfs >> --char_<'a'> >> char_<'b'>, "ab", 2).val[1] == 'b' );
}

constexpr void test_seq_fnum() {
	struct with_2_chars { char a, b; };
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>++ >> fnum<0>(char_<'c'>), "abc", 3), 'c', 'b' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>++ >> fnum<1>(char_<'c'>), "abc", 3), 'a', 'c' ) );
}

constexpr void test_seq_req() {
	constexpr auto ab_req = p::seq_enable_recursion >> (char_<'a'>|'b') >> -(_char<'('> >> p::rec<0> >> _char<')'>);
	static_assert( ab_req.is_recursion_enabled );
	static_assert( (char_<'a'> >> p::rec<0>).is_recursion_enabled == false );
	static_assert( (char_<'a'> >> char_<'b'>).is_recursion_enabled == false );
	static_assert( test_parser_parse(char{}, ab_req, "a(b)", 4) == 'b' );
	static_assert( test_parser_parse_r_str(ab_req, "a(b(a))", 7, 'a', 'b', 'a') );
	static_assert( test_parser_parse_r_str((char_<'a'>|'b'|'c') >> _char<'('> >> p::seq_enable_recursion >> -p::rec<0> >> _char<')'>, "a(b(c()))", 9, 'a', 'b', 'c') );

	struct semact_req_tester { char n='z'; semact_req_tester* ptr=nullptr; };
	static_assert( []{
		semact_req_tester r, r2; char ok='u';
		auto p=(p::seq_enable_recursion >> (char_<'a'>|'b')++ >> -(_char<'('> >> use_seq_result(p::rec<0>([&r2,&ok](auto& r){ok='o';return &r2;})) >> _char<')'>));
		p.parse(make_test_ctx(), make_source("a(b)"), r);
		return r.n * (r2.n == 'b') * (ok=='o');
	}() == 'a', "check semact for create value");
	static_assert( []{
		semact_req_tester r;
		auto p=(p::seq_enable_recursion >> char_<'a'>++ >> -(omit(char_<'('>) >> use_seq_result(p::rec<0>([](auto& r){r=new semact_req_tester('b');return r;})) >> omit(char_<')'>)));
		p.parse(make_test_ctx(), make_source("a(a)"), r);
		char ret = r.ptr->n * (r.ptr->ptr == nullptr);
		delete r.ptr;
		return ret;
	}() == 'a', "check semact for create value");
}

constexpr void test_seq_shift_pos() {
	static_assert( []{
		struct { char a='z', b='z'; int pos=-1; } r;
		(char_<'a'> >> ++char_<'b'> >> ++p::cur_pos).parse(make_test_ctx(), make_source("ab"), r);
		return r.pos;
	}() == 2, "can parse current position");
	static_assert( std::is_same_v<decltype(auto(char_<'a'> >> char_<'b'>)), decltype(auto(char_<'a'> >> char_<'b'>))>,
			"even if it different declarations it's the same parser and we can use one instead of other instance" );
	constexpr auto abcd = char_<'a'>++ >> char_<'b'>++ >> (char_<'c'> >> ++char_<'d'> >> ++p::seq_shift<0>) >> ++p::seq_shift<0>;
	static_assert( [&]{
		struct { char a='z', b='z'; struct { char c{},d{}; int shift1{};} i; int shift2{}; } r;
		abcd.parse(make_test_ctx(), make_source("abcd"), r);
		return (r.i.shift1==1) + 2*(r.shift2==2);
	}() == 3, "can parse current shift");
	constexpr auto ab_req = p::seq_enable_recursion >> (char_<'a'>|'b') >> -use_seq_result(_char<'('> >> use_seq_result(p::rec<0>) >> _char<')'> >> ++p::seq_shift<0>) >> ++p::seq_shift<0>;
	static_assert( [&]{
		struct { char a='z'; int shift; } r;
		ab_req.parse(make_test_ctx(), make_source("a(b)"), r);
		return (r.a=='b') + 2*(r.shift==3); // (b) == 3
	}() == 3, "can parse current shift in recursion (the last brace is included)");
}

struct test_1 {
	char a{}, b{}, c{}, d{};
};

static_assert( get<1>(ascip_details::tuple<int,char>{10, 'a'}) == 'a' );
static_assert( get<0>(ascip_details::tuple<int,char>{10, 'a'}) == 10 );

static_assert( []{
	test_1 r1;
	auto r = parse(
		p::char_<'a'>++ >> use_seq_result(skip(fnum<1>(char_<'b'>)++ >> char_<'c'>))++ >> ++char_<'d'>,
		p::make_source("abcd"), r1);
	return
	   (r==4) +
	2 *(r1.a == 'a') +
	4 *(r1.b == 'b') +
	8 *(r1.c == 'c') +
	16*(r1.d == 'd')
	;
}() == 31 );
static_assert( [] {
	test_1 r1;
	auto pr = parse(
		p::char_<'a'>++ >> use_seq_result(p::char_<'a'>++ >> p::char_<'c'> >> use_seq_result(p::char_<'a'>++ >> p::char_<'b'>)),
		p::make_source("aacab"), r1);
	return (pr==5) + 2*(r1.b=='b');
}() == 3 );

constexpr auto ab_req = p::seq_enable_recursion >> (char_<'a'>|'b') >> -(_char<'('> >> p::rec<0> >> _char<')'>);
static_assert( []{
	char r1{};
	const auto r = parse(ab_req, +p::space, p::make_source("a ( b )"), r1);
	return
	    (r==7)
	+ 2*(r1=='b')
	;
}() == 3 );

static_assert( [] {
	char r1{};
	const auto r = parse(p::seq_enable_recursion >> p::char_<'a'> >> (p::char_<'b'> | (p::char_<'r'> >> p::rec<0>)), p::make_source("arab"), r1);
	return (r==4)
	+ 2*(r1=='b')
	;
}() == 3, "can use recursion with | parser" );
static_assert( [] {
	char r{};
	int line_count=0;
	auto ctx = p::make_default_context(&line_count);
	auto parser = p::inject_skipping((char_<'a'> >> char_<'b'> >> char_<'c'>) | (char_<'a'> >> char_<'b'> >> char_<'d'>), +p::space);
	auto pr = parser.parse(ctx, p::make_source("\na\n\nb\n\nd"), r);
	return (pr==8) + 2*(new_line_count(ctx)==5);
}() == 3 );

int main(int,char**) {
	return 0;
}