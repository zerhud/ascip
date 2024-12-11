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

//TODO: can't we use tuple instead of linked list as context?
//      with tuple some methods don't need to be recursive - the [](auto...){} can to be used
//      also instead of make_context the push_back and push_front can to be used
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
		return exists_in_ctx<t3_t>(ctx3) + 2*exists_in_ctx<t1_t>(ctx3) + 4*!exists_in_ctx<t2_t>(ctx3);
	}()==7, "exists_in_ctx by tag only" );
	static_assert( [] {
		auto ctx = make_ctx<t2_t>(v2_t{}, make_ctx<t2_t>(v2_t{}, make_ctx<t1_t>(v1_t{})));
		return (count_in_ctx<t1_t>(ctx)==1) + 2*(count_in_ctx<t2_t>(ctx)==2) + 4*(count_in_ctx<t3_t>(ctx)==0);
	}() == 7 );
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
	//TODO: no test for  replace_by_tag and replace_by_tag_and_val_type replaces all found values, not only first or last one
	static_assert( [] {
		constexpr v1_t v1;
		auto ctx = make_ctx<t1_t>(v1);
		auto ctx_r = replace_by_tag<t1_t>(v2_t{}, ctx);
		auto ctx_rr = replace_by_tag<t2_t>(v3_t{}, make_ctx<t2_t>(v2_t{}, ctx));
		auto ctx_nr = replace_by_tag<t4_t>(v4_t{}, make_ctx<t2_t>(v2_t{}, ctx));
		return
		  (search_in_ctx<t1_t>(ctx_r).v==v2_t{}.v)
		+ 2*(search_in_ctx<t2_t>(ctx_rr).v==v3_t{}.v)
		+ 4*!exists_in_ctx<t4_t>(ctx_nr)
		+ 8*exists_in_ctx<t2_t>(ctx_nr)
		+ 16*(count_in_ctx<t2_t>(ctx_rr)==1)
		;
	}() == 31 );
	static_assert( [] {
		auto ctx = make_ctx<t2_t>(v2_t{}, make_ctx<t1_t>(v1_t{}));
		v1_t new_v{7};
		v2_t new_v_wrong{11};
		auto ctx_r = replace_by_tag_and_val_type<t1_t>(new_v, ctx);
		auto ctx_nr_t = replace_by_tag_and_val_type<t4_t>(new_v, ctx);
		auto ctx_nr_v = replace_by_tag_and_val_type<t1_t>(new_v_wrong, ctx);
		return
		  (search_in_ctx<t1_t>(ctx_r).v==7)
		+ 2*!exists_in_ctx<t4_t>(ctx_nr_t)
		+ 4*!exists_in_ctx<t4_t>(ctx_nr_v)
		+ 8*exists_in_ctx<t2_t>(ctx_nr_v)
		;
	}() == 15 );
	static_assert( [] {
		using ascip_details::type_dc;
		auto _ctx = make_ctx<t2_t>(v2_t{}, make_ctx<t1_t>(v1_t{}));
		auto _ctx2 = make_ctx<t1_t>(v3_t{}, _ctx);
		auto ctx = type_dc<decltype(_ctx)>;
		auto ctx2 = type_dc<decltype(_ctx2)>;
		return
		  !exists_in_ctx_constexpr(ascip_details::type_c<t1_t>, type_dc<v2_t>, ctx)
		+ 2*exists_in_ctx_constexpr(ascip_details::type_c<t2_t>, type_dc<v2_t>, ctx)
		+ 4*exists_in_ctx_constexpr(ascip_details::type_c<t1_t>, type_dc<v3_t&>, ctx2)
		;
	}() == 7);
	static_assert( [] {
		using ascip_details::type_dc;
		return exists_in_ctx_constexpr(type_dc<t1_t>, type_dc<v1_t>, type_dc<decltype(make_ctx<t1_t>(v2_t{}, make_ctx<t2_t>(v2_t{}, make_ctx<t1_t>(v1_t{}))))>);
	}() );
	static_assert( [] {
		using ascip_details::type_dc;
		auto ctx = make_ctx<t2_t>(v2_t{}, make_ctx<t1_t>(v1_t{}));
		v1_t new_v{7};
		v2_t new_v_wrong{11};
		auto ctx_r = add_or_replace_by_tag_and_val_type<t1_t>(new_v, ctx);
		auto ctx_nr_t = add_or_replace_by_tag_and_val_type<t4_t>(new_v, ctx);
		auto ctx_nr_v = add_or_replace_by_tag_and_val_type<t1_t>(new_v_wrong, ctx);
		auto ctx_rd = add_or_replace_by_tag_and_val_type<t1_t>(v1_t{3}, ctx_nr_v);
		return (search_in_ctx<t1_t>(ctx_r).v==7)
		+ 2*(search_in_ctx<t4_t>(ctx_nr_t).v==7)
		+ 4*(search_in_ctx<t1_t>(ctx_nr_v).v==11)
		+ 8*(by_ind_from_ctx<1, t1_t>(ctx_rd).v==3)
		;
	}() == 15 );
	static_assert( [] {
		auto ctx = make_ctx<t2_t>(v2_t{}, make_ctx<t1_t>(v1_t{}));
		auto [ctx_a, ctx_a_old] = add_or_replace<t3_t>(v3_t{}, ctx);
		auto [ctx_r, cxt_r_old] = add_or_replace<t2_t>(v2_t{7}, ctx);
		return (search_in_ctx<t3_t>(ctx_a).v==v3_t{}.v)
		+ 2*std::is_same_v<ascip_details::ctx_not_found_type, decltype(ctx_a_old)>
		+ 4*(cxt_r_old.v == v2_t{}.v)
		+ 8*(search_in_ctx<t2_t>(ctx_r).v==7)
		;
	}() == 15 );
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
