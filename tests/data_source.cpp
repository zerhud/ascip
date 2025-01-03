//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "factory.hpp"

constexpr static bool test_sources(auto&& s) {
	(void)( !!s        || (throw 0, 1) );
	(void)( s() == 'a' || (throw 1, 1) );
	(void)( !!s        || (throw 0, 1) );
	(void)( s() == 'b' || (throw 2, 1) );
	(void)( !s         || (throw 0, 1) );
	return true;
}
constexpr static void test_sources() {
	static_assert( p::make_source("ab").ind == 0, "source from qutoed string must to be with index" );
	static_assert( !!p::make_source("") == false, "source from array works, setp 0" );
	static_assert( test_sources( p::make_source("ab") ) );
	static_assert( test_sources( p::make_source(std::string_view("ab")) ) );
	static_assert( test_sources( p::make_source(std::string("ab")) ) );
	static_assert( ({ constexpr const char* v="ab"; test_sources( p::make_source(v) );}) );

	static_assert( []{
		auto s = p::make_source('a');
		(void)( !!s       || (throw 0, 1) );
		(void)( s() == 'a'|| (throw 1, 1) );
		(void)( !s        || (throw 2, 1) );
		return true;
	}(), "source for debug with single symbol" );

#ifndef __clang__
	static_assert((char)p::make_source("я")() == (char)0xD1);
	static_assert(({auto s=p::make_source("я");s();(char)s();}) == (char)0x8F);
#endif
	static_assert( []{
		auto s = p::make_source("я");
		(void)( !!s         || (throw 0, 1) );
		(void)( s() != s()  || (throw 1, 1) );
		(void)( !s          || (throw 2, 1) );
		return true;
	}(), "source for debug with multibyte symbol" );
}

int main(int,char**) {
	return 0;
}
