#include "ascip.hpp"

#include <tuple>

using parser = ascip<std::tuple>;

struct single_parser {
	char f;
	constexpr static auto gram() {
		return parser::nop >> fnum<0>(parser::char_<'a'>);
	}
};

struct single_field {
	char f;

	constexpr static auto gram() {
		return parser::_char<'b'> >> -fnum<0>(parser::char_<'a'>);
	}
};
using gram_type = std::decay_t<decltype(single_field::gram())>;

struct neasted_field {
	char f;
	struct {
		char f;
	} neasted;

	constexpr static auto gram() {
		return parser::char_<'a'> >> *(parser::char_<'a'> >> fnum<0>(parser::char_<'b'>));
	}
};


static_assert( !neasted_field::gram().is_struct_requires_pd );
static_assert( single_field::gram().is_struct_requires_pd );
static_assert( single_parser::gram().is_struct_requires_pd );
static_assert( parser::inject_skipping(single_parser::gram(), +parser::space).is_struct_requires_pd );
static_assert( parser::inject_skipping(single_field::gram(), +parser::space).is_struct_requires_pd );
static_assert( (parser::char_<'a'> >> parser::sfs >> parser::char_<'b'>).is_struct_requires_pd );
static_assert( []{
	single_parser r;
	auto pr = parse(single_parser::gram(), +parser::space, parser::make_source("a"), r) ;
	return  (pr==1) + 2*(r.f=='a');
}() == 3 );
static_assert( []{
	single_field r;
	auto pr = parse(single_field::gram(), +parser::space, parser::make_source("ba"), r) ;
	return  (pr==2) + 2*(r.f=='a');
}() == 3 );

struct dec_and_inc {
	char a;
	char b;
	char c;
	constexpr static auto gram() {
		// 1. a=>1, ind=1
		// 2. ind=0, b=>a, ind=1
		// 3. c=>b
		return parser::char_<'a'>++ >> --parser::char_<'b'>++ >> parser::char_<'c'>;
	}
};

static_assert( []{
	dec_and_inc r;
	auto pr = parse(dec_and_inc::gram(), +parser::space, parser::make_source("abc"), r);
	return (pr==3) + 2*(r.a=='b') + 4*(r.b=='c');
 }() == 7 );

int main(int,char**) {
	return 0;
}
