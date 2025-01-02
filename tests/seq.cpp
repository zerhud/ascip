//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "factory.hpp"

template<auto s> constexpr auto& char_ = p::char_<s>;
template<auto s> constexpr auto& _char = p::_char<s>;
constexpr auto make_test_ctx(auto&&... args) { return p::make_test_ctx(std::forward<decltype(args)>(args)...); }
constexpr auto make_source(auto&& src) { return p::make_source(std::forward<decltype(src)>(src)); }

constexpr void test_seq_simple_case() {
	constexpr auto p_ab = char_<'a'> >> char_<'b'>;
	static_assert( test_parser_parse_r_str(p_ab, "ab", 2, 'a', 'b') );
	static_assert( test_parser_parse_r_str(p_ab, "ac", -1, 'a'), "we store parsed part even on fail" );
	static_assert( test_parser_parse_r_str(p_ab, "abc", 2, 'a', 'b'), "don't catch a tail" );
	static_assert( test_parser_parse_r_str(p_ab, "cab", -1) );
	static_assert( test_parser_parse_r_str(p::any >> omit(p::space) >> p::any, "1 2", 3, '1', '2') );
	static_assert( test_parser_parse_r_str(char_<'a'> >> 'b', "ab", 2, 'a', 'b') );

	static_assert( []{ char r='z', l='a';
		(char_<'a'> >> char_<'b'> >> [&](auto& result, auto src, auto line, auto msg) {
		 src.ind /= (src.ind==2);
		 result /= (result=='b');
		 line /= (line==1);
		 l = 'u' * (result == 'b');
		 }).parse(make_test_ctx(), make_source("ab"), r);
	return l;}() == 'u');
	static_assert(
		test_parser_parse_r_str(char_<'a'> >> char_<'b'> >> [](auto& result, auto src, auto line, auto msg){return 1;}, "ab", 3, 'a', 'b'),
		"lambda value is added to position" );
	static_assert(
		test_parser_parse_r_str(char_<'a'> >> [](auto&& ctx, auto src, auto& result){return 1;}, "a", 2, 'a'),
		"lambda value is added to position" );
}

constexpr void test_seq_result_fields() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
	struct with_2_chars { char a, b; };
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>, "ab", 2), 'a', 'b' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, ++char_<'a'> >> --char_<'b'>, "ab", 2), 'b', 'a' ) );
	static_cast<const p::opt_seq_parser<p::char_parser<'a'>, p::char_parser<'b'>, p::char_parser<'c'>>&>(char_<'a'> >> char_<'b'> >> char_<'c'>);
	static_cast<const p::opt_seq_parser<p::char_parser<'a'>, p::int_parser, p::char_parser<'c'>, p::int_parser>&>(char_<'a'> >> p::int_ >> char_<'c'> >> p::int_);
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
#ifndef __clang__ // cannot since clang17 :(
	static_assert( test_parser_parse(with_1_field{}, fnum<0>(char_<'a'>) >> char_<'b'>, "ab", 2).val[0] == 'a' );
	static_assert( test_parser_parse(with_1_field{}, finc<0>(char_<'a'>) >> char_<'b'>, "ab", 2).val[1] == 'b' );
	static_assert( test_parser_parse(with_1_field{}, p::sfs >> --char_<'a'> >> char_<'b'>, "ab", 2).val[1] == 'b' );
#endif
}

constexpr void test_seq_fnum() {
	struct with_2_chars { char a, b; };
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>++ >> fnum<0>(char_<'c'>), "abc", 3), 'c', 'b' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>++ >> fnum<1>(char_<'c'>), "abc", 3), 'a', 'c' ) );
}

constexpr void test_seq_req() {
	constexpr auto ab_req = (char_<'a'>|'b') >> -(_char<'('> >> p::req<1> >> _char<')'>);
	static_assert( test_parser_parse(char{}, ab_req, "a(b)", 4) == 'b' );
	static_assert( test_parser_parse_r_str(ab_req, "a(b(a))", 7, 'a', 'b', 'a') );
	static_assert( test_parser_parse_r_str((char_<'a'>|'b'|'c') >> _char<'('> >> -p::req<0> >> _char<')'>, "a(b(c()))", 9, 'a', 'b', 'c') );

	struct semact_req_tester { char n='z'; semact_req_tester* ptr=nullptr; };
	static_assert( []{
		semact_req_tester r, r2; char ok='u';
		auto p=((char_<'a'>|'b')++ >> -(_char<'('> >> use_seq_result(p::req<1>([&r2,&ok](auto& r){ok='o';return &r2;})) >> _char<')'>));
		p.parse(make_test_ctx(), make_source("a(b)"), r);
		return r.n * (r2.n == 'b') * (ok=='o');
	}() == 'a', "check semact for create value");
	static_assert( []{
		semact_req_tester r;
		auto p=(char_<'a'>++ >> -(omit(char_<'('>) >> use_seq_result(p::req<1>([](auto& r){r=new semact_req_tester('b');return r;})) >> omit(char_<')'>)));
		p.parse(make_test_ctx(), make_source("a(a)"), r);
		char ret = r.ptr->n * (r.ptr->ptr == nullptr);
		delete r.ptr;
		return ret;
	}() == 'a', "check semact for create value");
}

constexpr void test_seq_must() {
#ifndef __clang__
	static_assert(
		({char r='z';(char_<'a'> >> char_<'b'> >> must<"t">(char_<'c'>)).parse(make_test_ctx(), make_source("abd"), r);}) == -1,
		"if error and no lambda - nothing changed");
	static_assert( ({
		char r='z';
		const auto err_method = [&](...){return -10;};
		(char_<'a'> >> char_<'b'> >> must<"t">(char_<'c'>)).parse(make_test_ctx(&err_method), make_source("abe"), r);
	}) == -10, "error lambda are called");
	const auto err_method = [](
			auto& result,
			auto src_on_start,
			auto line_number,
			auto message){
		line_number /= (line_number==2);
		return (-3 * (message=="unknown")) + (-4 * (message=="test"));
	};
	static_assert( ({ char r=0x00;
		(p::any >> char_<'a'> >> char_<'b'> > char_<'c'>).parse(make_test_ctx(&err_method), make_source("\nabe"), r);
	}) == -3, "on error: sources are on start sequence and on rule where the error");
	static_assert( ({ char r=0x00;
		(p::any >> char_<'a'> >> char_<'b'> >> must<"test">(char_<'c'>)).parse(make_test_ctx(&err_method), make_source("\nabe"), r);
	}) == -4, "on error: sources are on start sequence and on rule where the error");
#endif
}

constexpr void test_seq_shift_pos() {
	static_assert( []{
		struct { char a='z', b='z'; int pos=-1; } r;
		(char_<'a'> >> ++char_<'b'> >> ++p::cur_pos).parse(make_test_ctx(), make_source("ab"), r);
		return r.pos;
	}() == 2, "can parse current position");
	static_assert( std::is_same_v<decltype(auto(char_<'a'> >> char_<'b'>)), decltype(auto(char_<'a'> >> char_<'b'>))>,
			"even if it different declarations it's the same parser and we can use one instead of other instance" );
	constexpr auto abcd = char_<'a'>++ >> char_<'b'>++ >> (char_<'c'> >> ++char_<'d'> >> ++p::cur_shift) >> ++p::cur_shift;
	static_assert( [&]{
		struct { char a='z', b='z'; struct { char c,d; int shift1;} i; int shift2; } r;
		abcd.parse(make_test_ctx(), make_source("abcd"), r);
		return r.i.shift1 + r.shift2;
	}() == 6, "can parse current shift");
	constexpr auto ab_req = (char_<'a'>|'b') >> -use_seq_result(_char<'('> >> use_seq_result(p::req<1>) >> _char<')'> >> ++p::cur_shift) >> ++p::cur_shift;
	static_assert( [&]{
		struct { char a='z'; int shift; } r;
		ab_req.parse(make_test_ctx(), make_source("a(b)"), r);
		return (r.a=='b') + 2*(r.shift==5);
	}() == 3, "can parse current shift in reqursion (the last brace is included)");
}

int main(int,char**) {
	return 0;
}
