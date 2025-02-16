//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ascip.hpp"

#include <string>

static_assert( []{ ascip_details::tuple t{1, 'z'}; return get<1>(t); }() == 'z' );
static_assert( [] { constexpr ascip_details::tuple t{1, 'z'}; return get<1>(t); }() == 'z' );

constexpr static bool test_sources(auto&& s) {
	(void)( !!s        || (throw 0, 1) );
	(void)( s() == 'a' || (throw 1, 1) );
	(void)( !!s        || (throw 0, 1) );
	(void)( s() == 'b' || (throw 2, 1) );
	(void)( !s         || (throw 0, 1) );
	return true;
}
constexpr static void test_sources() {
	static_assert( ascip_details::make_source("ab").ind == 0, "source from qutoed string must to be with index" );
	static_assert( !!ascip_details::make_source("") == false, "source from array works, setp 0" );
	static_assert( test_sources( ascip_details::make_source("ab") ) );
	static_assert( test_sources( ascip_details::make_source(std::string_view("ab")) ) );
	static_assert( test_sources( ascip_details::make_source(std::string("ab")) ) );
	static_assert( ({ constexpr const char* v="ab"; test_sources( ascip_details::make_source(v) );}) );

	static_assert( []{
		auto s = ascip_details::make_source('a');
		(void)( !!s       || (throw 0, 1) );
		(void)( s() == 'a'|| (throw 1, 1) );
		(void)( !s        || (throw 2, 1) );
		return true;
	}(), "source for debug with single symbol" );

#ifndef __clang__
	static_assert((char)ascip_details::make_source("я")() == (char)0xD1);
	static_assert(({auto s=ascip_details::make_source("я");s();(char)s();}) == (char)0x8F);
#endif
	static_assert( []{
		auto s = ascip_details::make_source("я");
		(void)( !!s         || (throw 0, 1) );
		(void)( s() != s()  || (throw 1, 1) );
		(void)( !s          || (throw 2, 1) );
		return true;
	}(), "source for debug with multibyte symbol" );
}

constexpr void test_context() {
	using ascip_details::make_ctx;
	using ascip_details::ctx_not_found;

	struct v1_t{int v=1;}; struct v2_t{int v=2;}; struct v3_t{int v=4;}; struct v4_t{int v=8;};
	struct t1_t{}; struct t2_t{}; struct t3_t{}; struct t4_t{};
	static_assert( []{ constexpr v1_t v1;
		auto ctx = make_ctx<t1_t>(v1);
		return search_in_ctx<t1_t>(ctx).v;
	}()==1, "can find in single context" );
	static_assert( [] {
		auto ctx1 = make_ctx<t2_t>(v2_t{}, make_ctx<t1_t>(v1_t{}));
		auto ctx2 = ctx1;
		return search_in_ctx<t1_t>(ctx2).v;
	}() == 1, "can copy context" );
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
		return search_in_ctx<0, t1_t>(ctx2).v;
	}()==2, "can find by ind" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t1_t>(v2, make_ctx<t1_t>(v1));
		return search_in_ctx<1, t1_t>(ctx2).v;
	}()==1, "can find by ind" );
	static_assert(  exists_in_ctx<t1_t>(make_ctx<t1_t>(v1_t{})) );
	static_assert( !exists_in_ctx<t2_t>(make_ctx<t1_t>(v1_t{})) );
	(void)( static_cast<const decltype(ctx_not_found)&>(search_in_ctx<0, t2_t>(make_ctx<t1_t>(v2_t{}, make_ctx<t1_t>(v1_t{})))) );
	static_assert( [] { v1_t v1; v2_t v2; v3_t v3;
		auto ctx2 = make_ctx<t2_t>(v2, make_ctx<t1_t>(v1));
		auto ctx = ascip_details::replace_in_ctx<true, t2_t>(v3, ctx2);
		auto t2_t_count = repack(ctx, [](auto... f){return (contains(f.tags, ascip_details::type_c<t2_t>) + ...);});
		return (search_in_ctx<0, t2_t>(ctx).v==4) + 2*(t2_t_count==1);
	}() == 3 );
	static_assert( [] { v1_t v1; v2_t v2; v3_t v3;
		auto ctx2 = make_ctx<t2_t>(v2, make_ctx<t1_t>(v1));
		auto ctx = remove_from_ctx<t1_t>(ctx2);
		auto t2_t_count = repack(ctx, [](auto... f){return (contains(f.tags, ascip_details::type_c<t2_t>) + ...);});
		auto t1_t_count = repack(ctx, [](auto... f){return (contains(f.tags, ascip_details::type_c<t1_t>) + ...);});
		return (t1_t_count==0) + 2*(t2_t_count==1);
	}() == 3);
}

int main(int,char**) {
}
