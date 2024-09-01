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
//static_assert( single_field::gram().bar() );
static_assert( parser::exists_in(static_cast<const gram_type*>(nullptr),
			[](const auto* p){
				//std::decay_t<decltype(p)>::foo();
				//return ascip_details::is_specialization_of<std::decay_t<decltype(p)>, parser::seq_num_rfield_val>;
				return requires{ p->num_val; };
			},
			[](const auto* p){ return false; }
			) );

/*
static_assert( []{
	single_field r;
	auto pr = parse(single_field::gram(), +parser::space, parser::make_source("ba"), r) ;
	return  (pr==2) + 2*(r.f=='a');
}() == 3 );
*/

int main(int,char**) {
	return 0;
}
