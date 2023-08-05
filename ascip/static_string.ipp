#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<typename char_type, auto size>
struct string_literal {
	constexpr string_literal(const char_type (&str)[size]) {
		for(auto i=0;i<size;++i) value[i] = str[i];
	}

	char_type value[size];

	template<auto sz>
	constexpr bool operator==(const char_type(&r)[sz]) const {
		if(sz != size) return false;
		for(auto i=0;i<size;++i) if(value[i]!=r[i]) return false;
		return true;
	}
	template<typename char_t> requires (std::is_pointer_v<char_t>)
	constexpr bool operator==(char_t r) const {
		for(auto i=0;i<size;++i) {
			if(value[i]!=r[i]) return false;
		}
		return true;
	}
};
template<string_literal str> struct test_tmpl{ constexpr bool is_eq(const char* v) const {
	return str == v;
} };

constexpr void test_static_string() {
	static_assert( string_literal("cstr").value[4] == 0x00 );
	static_assert( string_literal("cstr") == "cstr" );
	static_assert( !(string_literal("cstr") == "cstr\0ups") );

	static_assert( test_tmpl<"test">{}.is_eq("test") );
	static_assert( !test_tmpl<"test">{}.is_eq("test_ups") );
	static_assert( test_tmpl<"test">{}.is_eq("test\0ups") );
}
