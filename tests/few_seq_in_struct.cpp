#include "ascip.hpp"

#include <tuple>

using p = ascip<std::tuple>;

struct test_1 {
	char a, b, c, d;
};

static_assert( get<1>(ascip_details::tuple<int,char>{10, 'a'}) == 'a' );
static_assert( get<0>(ascip_details::tuple<int,char>{10, 'a'}) == 10 );

static_assert( []{
	test_1 r1;
	auto r = parse(
		p::char_<'a'>++ >> use_seq_result(skip(fnum<1>(p::char_<'b'>)++ >> p::char_<'c'>))++ >> ++p::char_<'d'>,
		p::make_source("abcd"), r1);
	return
	   (r==4) +
	2 *(r1.a == 'a') +
	4 *(r1.b == 'b') +
	8 *(r1.c == 'c') +
	16*(r1.d == 'd')
	;
}() == 31 );

int main(int,char**) {
	return 0;
}
