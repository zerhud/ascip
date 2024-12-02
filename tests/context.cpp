//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ascip.hpp"
#include <tuple>

void test_val_resetter() {
	static_assert( []{
		int val = 10;
		{
			ascip_details::val_resetter vr(&val, 20);
			val /= (val==20);
		}
		return val; }() == 10 );
}

constexpr void test_context() {
	using ascip_details::make_ctx;
	using ascip_details::ctx_not_found;

	struct v1_t{int v=1;}; struct v2_t{int v=2;}; struct v3_t{int v=4;}; struct v4_t{int v=8;};
	struct t1_t{}; struct t2_t{}; struct t3_t{}; struct t4_t{};
	static_assert( []{ constexpr v1_t v1;
		constexpr auto ctx = make_ctx<t1_t>(v1);
		return search_in_ctx<t1_t>(ctx).v;
	}()==1, "can find in single context" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t2_t>(v2, make_ctx<t1_t>(v1));
		return search_in_ctx<t1_t>(ctx2).v;
	}()==1, "can find in 2 deep context" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t2_t>(v2, make_ctx<t1_t>(v1));
		return search_in_ctx<t2_t>(ctx2).v;
	}()==2, "can find in 2 deep context" );
	static_assert( []{ v1_t v1; v3_t v3;
		auto ctx3 = make_ctx<t3_t>(v3, make_ctx<t1_t>(v1));
		return exists_in_ctx<t3_t>(ctx3) && exists_in_ctx<t1_t>(ctx3) && !exists_in_ctx<t2_t>(ctx3);
	}()==true, "exists_in_ctx by tag only" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t1_t>(v2, make_ctx<t1_t>(v1));
		return by_ind_from_ctx<0,t1_t>(ctx2).v;
	}()==2, "can find by ind" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t1_t>(v2, make_ctx<t1_t>(v1));
		return by_ind_from_ctx<1,t1_t>(ctx2).v;
	}()==1, "can find by ind" );
	static_assert(  exists_in_ctx<t1_t>(make_ctx<t1_t>(v1_t{})) );
	static_assert( !exists_in_ctx<t2_t>(make_ctx<t1_t>(v1_t{})) );
	(void)( static_cast<const decltype(ctx_not_found)&>(by_ind_from_ctx<0,t2_t>(make_ctx<t1_t>(v2_t{}, make_ctx<t1_t>(v1_t{})))) );
	static_assert( []{
		auto ctx1 = make_ctx<t1_t>(1, make_ctx<t1_t>(2, make_ctx<t2_t>(3)));
		auto ctx2 = remove_from_ctx<t1_t>(ctx1);
		return (search_in_ctx<t1_t>(ctx1)==1) + (2*(search_in_ctx<t1_t>(ctx2)==2)); }() == 3 );
}

constexpr void context_parsers() {
	using p = ascip<std::tuple>;
	struct tag {};
	static_assert( []{
		char r;
		int r_inner;
		char val=3;
		parse(add_to_ctx<tag>(&val,
			p::char_<'a'> >>
			exec_before<tag>([&](auto* val, auto& p){r_inner=p*(*val);return &p;}, p::char_<'b'>)
		), p::make_source("ab"), r);
		return r_inner;
	}() == 'a'*3 );
	static_assert( []{
		char r;
		int r_inner;
		char val=3;
		parse(add_to_ctx<tag>(&val,
			p::char_<'a'> >>
			exec_after<tag>([&](auto* val, auto& p){r_inner=p*(*val);}, p::char_<'b'>)
		), p::make_source("ab"), r);
		return r_inner;
	}() == 'b'*3 );
}


int main(int,char**) {
	return 0;
}
