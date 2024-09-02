#include "ascip.hpp"

#include <tuple>

using parser = ascip<std::tuple>;

struct single_field {
	char f;

	constexpr static auto gram() {
		return parser::_char<'b'> >> -fnum<0>(parser::char_<'a'>);
	}
};
using gram_type = std::decay_t<decltype(single_field::gram())>;
constexpr auto simple_seq = parser::_char<'b'> >> -fnum<0>(parser::char_<'a'>);
constexpr auto simple_seq_inj = parser::inject_skipping(simple_seq, +parser::space);
constexpr auto neasted_parser = parser::char_<'a'> >> *(parser::char_<'a'> >> fnum<0>(parser::char_<'b'>));
struct single_field2 {
	char f;

	constexpr static auto gram() {
		return simple_seq;
		//return parser::_char<'b'> >> -fnum<0>(parser::char_<'a'>);
	}
};

//static_assert( single_field::gram().bar() );
static_assert( parser::exists_in(static_cast<const gram_type*>(nullptr),
			[](const auto* p){
				return ascip_details::is_specialization_of<std::decay_t<decltype(*p)>, parser::seq_num_rfield_val>;
			},
			[](const auto* p){ return false; }
			) );

static_assert( !parser::exists_in(&neasted_parser,
			[](const auto* p){
				return ascip_details::is_specialization_of<std::decay_t<decltype(*p)>, parser::seq_num_rfield_val>;
			},
			[](const auto* p){
				return requires{ p->seq; } && !requires{ static_cast<decltype(&neasted_parser)>(p); };
			}
			) );
static_assert( parser::inject_skipping(parser::char_<'b'> >> fnum<0>(parser::char_<'a'>), +parser::space).is_struct_requires_pd );
static_assert( parser::inject_skipping(single_field2::gram(), +parser::space).is_struct_requires_pd );
static_assert( single_field::gram().is_struct_requires_pd );
static_assert( single_field::gram().is_num_field_val<gram_type> );
using cur_t = std::decay_t<decltype(parser::inject_skipping(single_field::gram(), +parser::space))>;
static_assert( []{
	single_field r;
	auto pr = parse(single_field::gram(), +parser::space, parser::make_source("ba"), r) ;
	return  (pr==2) + 2*(r.f=='a');
}() == 3 );

int main(int,char**) {
	return 0;
}
