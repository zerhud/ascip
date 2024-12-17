#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<ascip_details::string_view type>
constexpr static auto make_source(type&& src) {
	struct strsrc {
		type src;
		unsigned ind = 0;
		constexpr auto operator()(){ return src[ind++]; }
		constexpr explicit operator bool() const { return ind < src.size(); }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
	} ret{ std::forward<decltype(src)>(src) };
	return ret;
}

constexpr static auto make_source(std::integral auto sym) {
	struct {
		decltype(sym) val; bool where_is_more=true;
		constexpr auto operator()(){ where_is_more=false; return val; }
		constexpr explicit operator bool() const { return where_is_more; }
		constexpr auto& operator += (int v) { (void)( v==1 || (throw 1,1) ); where_is_more=false; return *this; }
	} ret{ sym };
	return ret;
}

constexpr static auto make_source(const auto* vec) {
	struct {
		decltype(vec) val;
		unsigned sz;
		unsigned ind = 0;
		constexpr auto operator()(){ return val[ind++]; }
		constexpr explicit operator bool() const { return ind < sz-1; }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
	} ret{ vec, ascip_details::strlen(vec) };
	return ret;
}

constexpr static bool test_sources(auto&& s) {
	(void)( !!s        || (throw 0, 1) );
	(void)( s() == 'a' || (throw 1, 1) );
	(void)( !!s        || (throw 0, 1) );
	(void)( s() == 'b' || (throw 2, 1) );
	(void)( !s         || (throw 0, 1) );
	return true;
}
constexpr static void test_sources() {
	static_assert( make_source("ab").ind == 0, "source from qutoed string must to be with index" );
	static_assert( !!make_source("") == false, "source from array works, setp 0" );
	static_assert( test_sources( make_source("ab") ) );
	static_assert( test_sources( make_source(factory_t{}.mk_sv("ab")) ) );
#ifndef __clang__
	static_assert( test_sources( make_source(factory_t{}.mk_str("ab")) ) );
#endif
	static_assert( ({ constexpr const char* v="ab"; test_sources( make_source(v) );}) );

	static_assert( []{
		auto s = make_source('a');
		(void)( !!s       || (throw 0, 1) );
		(void)( s() == 'a'|| (throw 1, 1) );
		(void)( !s        || (throw 2, 1) );
		return true;
	}(), "source for debug with single symbol" );

#ifndef __clang__
	static_assert((char)make_source("я")() == (char)0xD1);
	static_assert(({auto s=make_source("я");s();(char)s();}) == (char)0x8F);
#endif
	static_assert( []{
		auto s = make_source("я");
		(void)( !!s         || (throw 0, 1) );
		(void)( s() != s()  || (throw 1, 1) );
		(void)( !s          || (throw 2, 1) );
		return true;
	}(), "source for debug with multibyte symbol" );
}
template<typename char_type, auto sz>
constexpr static auto& print_to(auto& os, const ascip_details::string_literal<char_type, sz>& what) { return os << what.value; }
constexpr static auto& print_to(auto& os, const auto& what) { return os << what; }
constexpr static void write_out_error_msg(
		auto& os,
		auto fn,
		auto msg,
		auto expt,
		auto src,
		auto ln
		) {
	print_to(os, msg) << '\n';
	print_to(os, fn) << ':' << ln << ' ';
	auto shift = 0;
	do{ ++shift; src += -2; } while(src()!='\n');
	os << '\n';
	while(src) {
		auto cur = src();
		if(cur == '\n') break;
		os << cur;
	}
	os << '\n';
	for(auto i=0;i<shift;++i) os << '-';
	os << "^\n";
}
