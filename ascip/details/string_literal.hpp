#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>

namespace ascip_details {

template<typename char_type, auto str_size>
struct string_literal {
	constexpr string_literal() =default ;
	constexpr string_literal(const char_type (&str)[str_size]) {
		for(auto i=0;i<str_size;++i) value[i] = str[i];
	}

	char_type value[str_size];

	constexpr auto size() const { static_assert( 1 <= str_size ); return str_size-1; }
	constexpr auto operator[](auto i) const { return value[i]; }

	consteval bool contains(char_type s) const {
		for (auto i=0;i<str_size;++i) if (value[i]==s) return true;
		return false;
	}

	constexpr void for_each(auto&& fnc) const {
		[&]<auto... i>(std::index_sequence<i...>){
			(fnc(value[i]), ...);
		}(std::make_index_sequence<str_size>{});
	}

	template<auto sz>
	constexpr bool operator==(const char_type(&r)[sz]) const {
		if constexpr (sz != str_size) return false;
		for(auto i=0;i<str_size;++i) if(value[i]!=r[i]) return false;
		return true;
	}
	template<typename char_t> requires (std::is_pointer_v<char_t>)
	constexpr bool operator==(char_t r) const {
		for(auto i=0;i<str_size;++i) {
			if(value[i]!=r[i]) return false;
		}
		return true;
	}

	constexpr auto zero_pos() const {
		for (auto i=0;i<str_size;++i) if (!value[i]) return i;
		return str_size;
	}

	template<auto sz>
	friend constexpr auto operator+(const string_literal& left, const char_type(&right)[sz]) {
		string_literal<char_type, str_size + sz - 1> ret;
		for (auto i=0;i<str_size-1;++i) ret.value[i] = left[i];
		for (auto i=0;i<sz;++i) ret.value[i+str_size-1] = right[i];
		return ret;
	}
};

template<string_literal str> struct test_tmpl {
	constexpr bool is_eq(const char* v) const { return str == v; }
	constexpr auto size() const { return str.size(); }
};

constexpr void test_static_string() {
	static_assert( string_literal("cstr")[0] == 'c' );
	static_assert( string_literal("cstr")[4] == 0x00 );
	static_assert( string_literal("cstr").value[4] == 0x00 );
	static_assert( string_literal("cstr") == "cstr" );
	static_assert( !(string_literal("cstr") == "cstr\0ups") );

	static_assert( string_literal("cstr").size() == 4 );

	static_assert( test_tmpl<"test">{}.is_eq("test") );
	static_assert( !test_tmpl<"test">{}.is_eq("test_ups") );
	static_assert( test_tmpl<"test">{}.is_eq("test\0ups") );
	static_assert( test_tmpl<"test">{}.size() == 4 );
	static_assert( test_tmpl<"is">{}.size() == 2 );

	static_assert(  string_literal("abc").contains('b') );
	static_assert( !string_literal("abc").contains('d') );
	static_assert( [] {
		string_literal src("src");
		auto ret = src + "_test";
		return (ret.size()==8) + 2*(ret == "src_test");
	}() == 3 );
}

}