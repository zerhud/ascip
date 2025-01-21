#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "concepts.hpp"

namespace ascip_details {

constexpr auto back_to_nl(auto& src, auto& ind) {
	auto prev = ind;
	while (ind>0 &&  src[--ind]!='\n');
	return prev - ++ind;
}

template<ascip_details::string_view type>
constexpr static auto make_source(type&& src) {
	struct strsrc {
		type src;
		unsigned ind = 0;
		constexpr auto operator()(){ return src[ind++]; }
		constexpr explicit operator bool() const { return ind < src.size(); }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
		constexpr auto back_to_nl() { return ascip_details::back_to_nl(src, ind); }
		constexpr static bool can_read_after_end() { return false; }
	} ret{ std::forward<decltype(src)>(src) };
	return ret;
}

constexpr static auto make_source(std::integral auto sym) {
	struct gcc_requires_name_for_tu_bug { // https://stackoverflow.com/a/66966679/24870413
		decltype(sym) val; bool where_is_more=true;
		constexpr auto operator()(){ where_is_more=false; return val; }
		constexpr explicit operator bool() const { return where_is_more; }
		constexpr auto& operator += (int v) { (void)( v==1 || (throw 1,1) ); where_is_more=false; return *this; }
	} ret{ sym };
	return ret;
}

constexpr auto strlen(const auto* vec){ unsigned ret = 0; while(vec[ret])++ret; return ++ret; }
constexpr static auto make_source(const auto* vec) {
	struct gcc_requires_name_for_tu_bug { // https://stackoverflow.com/a/66966679/24870413
		decltype(vec) val;
		unsigned sz;
		unsigned ind = 0;
		constexpr auto operator()(){ return val[ind++]; }
		constexpr explicit operator bool() const { return ind < sz-1; }
		constexpr auto& operator += (const int v) { ind+=v; return *this; }
		constexpr auto back_to_nl() { return ascip_details::back_to_nl(val, ind); }
		constexpr static bool can_read_after_end() { return true; }
	} ret{ vec, strlen(vec) };
	return ret;
}

template<typename src_type> constexpr bool can_read_after_end() {
	if constexpr (requires{src_type::can_read_after_end();})
		return src_type::can_read_after_end();
	else return false;
}

constexpr auto pos(const auto& src) {
	//TODO: remove this function and use information from context
	if constexpr ( requires{ position(src); } ) return position(src);
	else return src.ind;
}

} // namespace ascip_details
