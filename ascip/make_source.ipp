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
