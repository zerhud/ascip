#pragma once

//          Copyright Hudyaev Alexey 2004 - 2006.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <tuple>
#include <utility>

namespace ascip {

/*TODO list:
 *  handle errors
 */
struct parser_tag {};
template<typename type> concept parser = requires(const type& p){ static_cast<const parser_tag&>(p); };

template<typename parser, typename act_t> struct semact_parser : parser {
	act_t act;
	constexpr const auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = parser::parse(ctx, src, result);
		if(ret >= 0) {
			if constexpr (requires{ act(ret, ctx, src, result); }) act(ret, ctx, src, result);
			else if constexpr (requires{ acct(ret, result); }) act(ret, result); 
			else act();
		}
		return ret;
	}
};

template<typename parser>
struct base_parser : parser_tag {
	constexpr auto operator()(auto act) const {
		return semact_parser<parser, decltype(auto(act))>{ static_cast<const parser&>(*this), static_cast<decltype(act)&&>(act) };
	}
};

template<typename type> concept vector = requires(const type& v){ v.size(); v[0]; v.back(); } && requires(type& v){ v.pop_back(); };
template<typename type> concept string = requires(type& s){ s += typename type::value_type{}; } && vector<type>;
template<typename type> concept string_view = requires(const type& s){ s.size(); s[0]; s.back(); };
template<typename type> concept empbackable = requires(type& r){ emplace_back(r); } || requires(type& r){ r.emplace_back(); };

namespace details {
template<typename...> struct types_set{};
template<auto sz> constexpr auto arrsize(const auto(&)[sz]){ return sz; }
constexpr auto strlen(const auto* vec){ unsigned ret = 0; while(vec[ret++]); return ret; }
constexpr auto test_data_size(const auto* v){ return strlen(v)-1; }
constexpr auto test_data_size(auto v){ return 1; }
struct type_any_eq_allow {
	template<typename type> constexpr auto& operator =(const type&){ return *this; }
	template<typename type> constexpr auto& operator+=(const type&){ return *this; }
	template<typename type> constexpr auto& operator-=(const type&){ return *this; }
	template<typename type> constexpr auto& operator/=(const type&){ return *this; }
	template<typename type> constexpr auto& operator*=(const type&){ return *this; }
};
constexpr auto& pop_back(type_any_eq_allow& v){ return v; };
constexpr auto& emplace_back(type_any_eq_allow& v){ return v; };
constexpr void pop(auto& r) requires requires{ pop_back(r); } { pop_back(r); }
constexpr void pop(auto& r) requires requires{ r.pop_back(); } { r.pop_back(); }
constexpr void pop(auto& r) { }
constexpr auto& empback(empbackable auto& r) requires requires{ emplace_back(r); } { return emplace_back(r); }
constexpr auto& empback(empbackable auto& r) requires requires{ r.emplace_back(); } { return r.emplace_back(); }
constexpr auto& empback(string auto& r) { r += typename decltype(auto(r))::value_type{}; return r.back(); }
constexpr auto& empback(auto& r) requires( !empbackable<decltype(auto(r))> && !string<decltype(auto(r))> ){ return r; }
inline constexpr auto& eq( auto& to, const auto& from) { return empback(to) = from; }

template<auto ind, typename t> struct tuple_base { t val;
	template<auto i> constexpr auto& get() const requires (i==ind) { return val; }
	template<auto i> constexpr std::enable_if_t<i==ind, t>& _get() const { return val; }
};
template<auto cur, typename... list> struct tuple_recursion {};
template<auto cur, typename t, typename... list> struct tuple_recursion<cur,t,list...> : tuple_base<cur, t>, tuple_recursion<cur+1,list...> { };
template<typename... types> struct tuple :  tuple_recursion<0, types...> { };
template<auto ind, auto cur, typename type, typename... types> constexpr const auto& get_impl(const auto& src) {
	if constexpr (ind==cur) return static_cast<const tuple_base<ind,type>*>(&src)->val;
	else return get_impl<ind, cur+1, types...>(src);
}
template<auto ind, typename... types> constexpr const auto& get(const tuple<types...>& src) {
	static_assert( ind < sizeof...(types), "tuple: out of range" );
	//return src.template _get<ind>();
	//return src.template get<ind>();
	return get_impl<ind, 0, types...>(src);
}

template<typename type, template<typename...>class tmpl> constexpr const bool is_specialization_of = false;
template<template<typename...>class type, typename... args> constexpr const bool is_specialization_of<type<args...>, type> = true;

struct exception { };

} // namespace details
// ===============================
//          reflection part
// ===============================
namespace reflect {
constexpr struct {
	template<typename t> operator t() const ;
} any ;

template<typename type, auto... args> constexpr auto size_impl() {
	if constexpr ( requires{ type{ args..., any }; })
		return size_impl<type, args..., any>();
	else return sizeof...(args);
}

template<typename t> constexpr auto size = size_impl<t>();
template<auto ind,auto cur=0> constexpr auto& nth(auto& first, auto&... args) { if constexpr (cur==ind) return first; else return nth<ind,cur+1>(args...); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 1) { auto&[f1]=r; return f1; }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 2) { auto&[f1,f2]=r; return nth<ind>(f1,f2); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 3) { auto&[f1,f2,f3]=r; return nth<ind>(f1,f2,f3); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 4) { auto&[f1,f2,f3,f4]=r; return nth<ind>(f1,f2,f3,f4); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 5) { auto&[f1,f2,f3,f4,f5]=r; return nth<ind>(f1,f2,f3,f4,f5); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 6) { auto&[f1,f2,f3,f4,f5,f6]=r; return nth<ind>(f1,f2,f3,f4,f5,f6); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 7) { auto&[f1,f2,f3,f4,f5,f6,f7]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 8) { auto&[f1,f2,f3,f4,f5,f6,f7,f8]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 9) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 10) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 11) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 12) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 13) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 14) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 15) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 16) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 17) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 18) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 19) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 20) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 21) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 22) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 23) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 24) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 25) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 26) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 27) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 28) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 29) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 30) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 31) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 32) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 33) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 34) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 35) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 36) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 37) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 38) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 39) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 40) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 41) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 42) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 43) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 44) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 45) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 46) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 47) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 48) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 49) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 50) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 51) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 52) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 53) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 54) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 55) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 56) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 57) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 58) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 59) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 60) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 61) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 62) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 63) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 64) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 65) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 66) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 67) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 68) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 69) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 70) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 71) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 72) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 73) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 74) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 75) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 76) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 77) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 78) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 79) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 80) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 81) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 82) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 83) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 84) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 85) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 86) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 87) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 88) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 89) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 90) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 91) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 92) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 93) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 94) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 95) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 96) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 97) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 98) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> == 99) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==100) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==101) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==102) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==103) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==104) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==105) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==106) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==107) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==108) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==109) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==110) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==111) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==112) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==113) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==114) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==115) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==116) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==117) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==118) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==119) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==120) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==121) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==122) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==123) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==124) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==125) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==126) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==127) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127); }
template<auto ind> constexpr auto& get(auto& r) requires (size<decltype(auto(r))> ==128) { auto&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127,f128]=r; return nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127,f128); }
} // namespace reflect

template<typename type> concept struct_sem_act = requires(const type& c){c(reflect::any, reflect::any, reflect::any, reflect::any);};

// ===============================
//          details next part
// ===============================
namespace details {

constexpr bool is_eq(const auto& l, const auto& r) requires requires{ l==r; } { return l==r; }
constexpr bool is_eq(const auto& l, const auto& r) requires (!requires{ l==r; }) { return false; }

template<class init_t, auto... inds, template<typename...>class src_t, typename... src_args_t, typename... args_t>
constexpr auto init_with_get(const src_t<src_args_t...>& src, args_t&&... args) {
	if constexpr (sizeof...(inds) == sizeof...(src_args_t)) return init_t{ get<inds>(src)..., static_cast<args_t&&>(args)... };
	else return init_with_get<init_t, inds..., sizeof...(inds)>(src, static_cast<args_t&&>(args)...);
}
template<class init_t, auto... inds, template<typename...>class src_t, typename... src_args_t, typename... args_t>
constexpr auto init_with_get_i(const src_t<src_args_t...>& src, args_t&&... args) {
	if constexpr (sizeof...(inds) == sizeof...(src_args_t)) return init_t{ static_cast<args_t&&>(args)..., get<inds>(src)... };
	else return init_with_get_i<init_t, inds..., sizeof...(inds)>(src, static_cast<args_t&&>(args)...);
}

template<template<typename...>class tuple, typename... left_args, typename... extra_args>
constexpr auto push_back(tuple<left_args...>& to, extra_args&&... what) {
	using ret_t = tuple<left_args..., extra_args...>;
	return init_with_get<ret_t>(to, static_cast<extra_args&&>(what)...);
}
template<template<typename...>class tuple, typename... left_args, typename... extra_args>
constexpr auto push_front(tuple<left_args...>& to, extra_args&&... what) {
	using ret_t = tuple<extra_args..., left_args...>;
	return init_with_get_i<ret_t>(to, static_cast<extra_args&&>(what)...);
}
template<auto... inds, template<typename...>class tuple, typename... left_args>
constexpr auto exists_impl(const tuple<left_args...>& to, auto&& what) {
	if constexpr (sizeof...(inds) == sizeof...(left_args))
		return (is_eq(get<inds>(to), what) || ... );
	else return exists_impl<inds..., sizeof...(inds)>(to, what);
}
template<template<typename...>class tuple, typename... left_args>
constexpr auto exists(const tuple<left_args...>& to, auto&& what) {
	return exists_impl(to, what);
}


template<typename arg, template<typename...>class tuple, typename... left_args>
constexpr const bool contains(const tuple<left_args...>&) { return (std::is_same_v<left_args,arg> || ...); }

constexpr auto pos(const auto& src)
{
	if constexpr ( requires{ position(src); } ) return position(src);
	else return src.ind;
}

} // namespace details

/*
template<auto size, typename type>
constexpr auto make_source(const type(&src)[size]) requires true {
	struct arsrc {
		const type* arr;
		unsigned ind = 0;
		constexpr auto operator()(){ return arr[ind++]; }
		constexpr explicit operator bool() const { return ind < size-1; }
		constexpr auto& operator += (auto v) { ind+=v; return *this; }
	} ret{ src };
	return ret;
}
*/

template<string_view type>
constexpr auto make_source(type&& src) {
	struct strsrc {
		type src;
		unsigned ind = 0;
		constexpr auto operator()(){ return src[ind++]; }
		constexpr explicit operator bool() const { return ind < src.size(); }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
	} ret{ src };
	return ret;
}

constexpr auto make_source(auto sym) requires( !string_view<decltype(sym)> && !vector<decltype(sym)> ){
	struct {
		decltype(sym) val;
		constexpr auto operator()(){ return val; }
		constexpr explicit operator bool() const { return false; }
		constexpr auto& operator += (int v) { return *this; }
	} ret{ sym };
	return ret;
}

constexpr auto make_source(const auto* vec) {
	struct {
		decltype(vec) val;
		unsigned sz;
		unsigned ind = 0;
		constexpr auto operator()(){ return val[ind++]; }
		constexpr explicit operator bool() const { return ind < sz-1; }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
	} ret{ vec, details::strlen(vec) };
	return ret;
}

template<typename factory_t>
void test_src() {
	static_assert( !!make_source("") == false, "source from array works, setp 0" );
	static_assert( make_source("ab")() == 'a', "source from array works, setp 1" );
	static_assert( ({auto s=make_source("ab");s();s();}) == 'b', "source from array works, setp 2" );
	static_assert( ({auto s=make_source("ab");s();s();s();}) == 0x00, "source from array works, setp 3" );
	static_assert( ({auto s=make_source("ab");!!s;}) == true, "source from array works, setp 4" );
	static_assert( ({auto s=make_source("ab");s();!!s;}) == true, "source from array works, setp 5" );
	static_assert( ({auto s=make_source("ab");s();s();!!s;}) == false, "source from array works, setp 6" );

	static_assert( !!make_source(factory_t{}.mk_str()) == false, "source from string works, step 0" );
	static_assert( make_source(factory_t{}.mk_str("ab"))() == 'a', "source from string works, step 1" );
	static_assert( ({auto s=make_source(factory_t{}.mk_str("ab"));s();s();}) == 'b', "source from string works, step 2" );
	static_assert( ({auto s=make_source(factory_t{}.mk_str("ab"));!!s;}) == true, "source from string works, step 3" );
	static_assert( ({auto s=make_source(factory_t{}.mk_str("ab"));s();!!s;}) == true, "source from string works, step 4" );
	static_assert( ({auto s=make_source(factory_t{}.mk_str("ab"));s();s();!!s;}) == false, "source from string works, step 5" );

	static_assert( !!make_source(factory_t{}.mk_sv("")) == false, "source from string_view works, step 0" );
	static_assert( make_source(factory_t{}.mk_sv("ab"))() == 'a', "source from string_view works, step 1" );

	static_assert( make_source('a')() == 'a', "debug source works, setp 1");
	static_assert( !!make_source('a') == false, "debug source works, setp 2");

	static_assert( ({constexpr const char* v="a";make_source(v)();}) == 'a', "can extact value from const char*" );
	static_assert( ({constexpr const char* v="a";auto s=make_source(v);!!s;}) == true, "can extact value from const char*" );
	static_assert( ({constexpr const char* v="a";auto s=make_source(v);s();!!s;}) == false, "can extact value from const char*" );
}

// ===============================
//          parsers part
// ===============================

template<auto sym> struct char_parser : base_parser<char_parser<sym>> {
	constexpr auto parse(auto&&, auto src, auto& result) const {
		const bool ok = src() == sym;
		if(ok) details::eq(result, sym);
		return -1 + (2 * ok);
	}

	constexpr static const decltype(sym) valid_data[] = {sym, sym, sym};
	constexpr static const decltype(sym) valid_result[] = {sym, sym, sym};
	constexpr static const decltype(sym) invalid_data[] = {sym-1, sym+1};
};
template<auto val> constexpr const auto char_ = char_parser<val>{};

template<typename t> struct value_parser : base_parser<value_parser<t>> {
	t val;
	constexpr value_parser(t v) : val(v) {}
	constexpr auto parse(auto&&, auto src, auto& result) const {
		const bool ok = src() == val;
		if(ok) details::eq(result, val);
		return -2 * !ok + 1;
	}
};

template<parser type1, parser type2> struct variant_parser : base_parser<variant_parser<type1, type2>> {
	type1 p1;
	type2 p2;
	constexpr variant_parser( type1 v1, type2 v2 ) : p1(v1), p2(v2) {}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto r1 = p1.parse(ctx, src, result);
		return 0 <= r1 ? r1 : p2.parse(ctx, src, result);
	}
};
constexpr auto operator|(const auto& p1, const auto& p2) { return variant_parser<decltype(auto(p1)), decltype((p2))>{ p1, p2 }; }
constexpr auto operator|(const auto& p1, char p2) { value_parser p(p2); return variant_parser{ p1, p }; }

template<auto from, auto to> struct range_parser : base_parser<range_parser<from,to>> { 
	constexpr auto parse(auto&&, auto src, auto& result) const {
		auto sym = src();
		const bool ok = from <= sym && sym <= to;
		if(ok) details::eq( result, sym );
		return -2 * !ok + 1;
	}

	constexpr static const decltype(to) valid_data[] = {from, from+1, to-1, to};
	constexpr static const decltype(to) valid_result[] = {from, from+1, to-1, to};
	constexpr static const decltype(to) invalid_data[] = {from-1, to+1};
};

constexpr struct space_parser : base_parser<space_parser> {
	constexpr auto parse(auto&&,auto src, auto&) const {
		auto sym = src();
		return 0x07 < sym && sym < '!'; // 0x08 is a backspace
	}
} space ;

constexpr struct any_parser : base_parser<any_parser> {
	constexpr auto parse(auto&&, auto src, auto& result) const {
		details::eq( result, src() );
		return 1;
	}

	constexpr static const char valid_data[] = {'a', '1', 'Z'};
	constexpr static const char valid_result[] = {'a', '1', 'Z'};
} any ;
constexpr const auto lower = range_parser<'a','z'>{};
constexpr const auto upper = range_parser<'A','Z'>{};
constexpr const auto digit = range_parser<'0','9'>{};
constexpr const auto d10 = range_parser<'0', '9'>{};
constexpr const auto letter = lower | upper;

constexpr struct int_parser : base_parser<int_parser> {
	constexpr bool is_int(auto s) const { return '0' <= s && s <= '9'; }
	constexpr bool next(auto cur, auto& result) const {
		const bool isint = is_int(cur);
		result += ((int)(cur - '0')) * isint;
		result /= (!isint * 9) + 1;
		return isint;
	}
	constexpr auto parse(auto&&, auto src, auto& _result)  const {
		auto sign = src();
		if(sign != '-' && sign != '+' && !is_int(sign)) return -1;
		int signer = sign == '-' ? -1 : is_int(sign) || sign=='+';
		auto& result = details::eq(_result, is_int(sign) ? sign-'0' : 0);
		auto ret = 1;
		while(src && next(src(), result *= 10)) ++ret;
		result *= signer;
		return ret;
	}

        constexpr static const char* valid_data[] = { "-1", "+1", "1", "0", "103", "-100" };
        constexpr static const int valid_result[] = {  -1,    1,   1,   0,   103,   -100 };
        constexpr static const char invalid_data[] = { 'a', '%' };
} int_ ;

template<typename parser> struct negate_parser : parser {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = parser::parse(ctx, static_cast<decltype(auto(src))&&>(src), result);
		return -1 * (0 <= ret);
	}
};

template<parser type> constexpr auto operator!(const type& p) {
	return negate_parser<decltype(auto(p))>{ p };
}

template<typename value_t, parser parser_t> struct as_parser : parser_t {
	value_t val;
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		details::type_any_eq_allow r;
		auto shift = parser_t::parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};
template<typename value_t, parser parser_t> constexpr auto as( parser_t&& p, value_t&& val ){
	return as_parser<decltype(auto(val)), decltype(auto(p))>{ p, val };
}

template<parser parser_t> struct omit_parser : parser_t {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		details::type_any_eq_allow r;
		return parser_t::parse(ctx, src, r);
	}
};
constexpr auto omit(auto&& p) { return omit_parser<decltype(auto(p))>{ p }; }

template<parser parser_t, typename lparser> struct unary_list_parser : parser_t {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = static_cast<const lparser*>(this)->start_parse(ctx, src, result);
		src += ret * (0 <= ret);
		auto cur_r = ret;
		while(src && 0<=cur_r) {
			cur_r = parser_t::parse(ctx, src, details::empback(result));
			ret += cur_r * (0<=cur_r);
			src += cur_r * (0<=cur_r);
		}
		if(src) details::pop(result);
		return ret;
	}
};
template<parser parser_t> struct unary_opt_list_parser : unary_list_parser<parser_t, unary_opt_list_parser<parser_t>> {
	constexpr auto start_parse(auto&& ctx, auto src, auto& result) const { return 0; }
};
template<parser parser_t> struct unary_req_list_parser : unary_list_parser<parser_t, unary_req_list_parser<parser_t>> {
	constexpr auto start_parse(auto&& ctx, auto src, auto& result) const {
		return parser_t::parse(ctx, src, details::empback(result));
	}
};
template<parser parser_t> constexpr auto operator+(const parser_t& p) { return unary_req_list_parser<parser_t>{ p }; }
template<parser parser_t> constexpr auto operator*(const parser_t& p) { return unary_opt_list_parser<parser_t>{ p }; }
template<parser left, parser right> constexpr auto operator%(const left& l, const right& r) { return l >> *(r >> l); }
template<parser left> constexpr auto operator%(const left& l, char r) { return l >> *(value_parser{r} >> l); }

constexpr struct cur_pos_parser : base_parser<cur_pos_parser> {
	constexpr auto parse(auto&&, auto src, auto& result) const {
		details::eq(result, details::pos(src));
		return 0;
	}
} cur_pos;

template<auto cnt, parser p> struct _seq_inc_rfield_val : p { constexpr static auto inc_val = cnt; };
template<parser p> struct seq_inc_rfield_val : p { };
template<typename type> constexpr auto inc_field_val = 0;
template<parser p> constexpr auto inc_field_val<seq_inc_rfield_val<p>> = p::inc_val; 

struct seq_tag {};
struct seq_inc_rfield : base_parser<seq_inc_rfield> {constexpr auto parse(auto&&,auto,auto&)const {return 0;} } sfs ;
template<parser p> struct seq_inc_rfield_after : p {};
template<parser p> struct seq_inc_rfield_before : p {};
template<parser p> struct seq_dec_rfield_after : p {};
template<parser p> struct seq_dec_rfield_before : p {};
template<typename concrete, typename... parsers> struct com_seq_parser : base_parser<concrete>, seq_tag {
	std::tuple<parsers...> seq;

	constexpr com_seq_parser(auto&&... args) requires (sizeof...(parsers) == sizeof...(args)) : seq( static_cast<decltype(args)&&>(args)... ) {}

	template<typename type> constexpr static bool is_field_separator = requires(type&p){ static_cast<const seq_inc_rfield&>(p); };
	template<typename type> constexpr static bool is_inc_field_val = details::is_specialization_of<type, seq_inc_rfield_val>;
	template<typename type> constexpr static bool is_inc_field_after = details::is_specialization_of<type, seq_inc_rfield_after>;
	template<typename type> constexpr static bool is_inc_field_before = details::is_specialization_of<type, seq_inc_rfield_before>;
	template<typename type> constexpr static bool is_dec_field_after = details::is_specialization_of<type, seq_dec_rfield_after>;
	template<typename type> constexpr static bool is_dec_field_before = details::is_specialization_of<type, seq_dec_rfield_before>;
	template<typename... types> constexpr static bool is_struct_requires = 
		((is_field_separator<types> + ...) + (is_inc_field_val<types> + ...) +
		 (is_inc_field_after<types> + ...) + (is_inc_field_before<types> + ...) +
		 (is_dec_field_after<types> + ...) + (is_dec_field_before<types> + ...)
		) > 0;

	constexpr auto on_error(auto val) const { return static_cast<const concrete*>(this)->on_error(val); }

	template<auto find> constexpr auto call_parse(parser auto& p, auto&& ctx, auto src, auto& result) const requires is_struct_requires<parsers...> {
		if constexpr (is_field_separator<decltype(auto(p))>) return p.parse(ctx, src, result);
		else {
			return p.parse(ctx, src, reflect::get<find>(result));
		}
	}
	template<auto find> constexpr auto call_parse(parser auto& p, auto&& ctx, auto src, auto& result) const requires (!is_struct_requires<parsers...>) {
		return p.parse(ctx, src, result);
	}
	template<auto find> constexpr auto call_parse(auto& p, auto&& ctx, auto src, auto& result) const requires (!parser<decltype(auto(p))>) {
		if constexpr (requires{ p(src, src, result, ctx);})
			return p(*get<2>(ctx), src, result, ctx);
		else if constexpr (!requires{ { p(src, src, result) } -> std::same_as<void>; })
			return p(*get<2>(ctx), src, result);
		else
			return p(*get<2>(ctx), src, result), 0;
	}
	template<auto find, auto pind, typename cur_t, typename... tail> constexpr auto parse_seq(auto&& ctx, auto src, auto& result) const {
		constexpr const auto cur_field = find + is_inc_field_before<cur_t> + (-1*is_dec_field_before<cur_t>) + inc_field_val<cur_t>;
		constexpr const auto nxt_field = cur_field + is_inc_field_after<cur_t> + (-1*is_dec_field_after<cur_t>) + is_field_separator<cur_t>;

		auto& cur = get<pind>(seq);
		auto ret = call_parse<cur_field>(cur, ctx, src, result);
		src += ret * (0 <= ret);
		if constexpr (pind+1 == sizeof...(parsers)) return on_error(ret); 
		else {
			if( ret < 0 ) return on_error(ret);
			auto req = parse_seq<nxt_field, pind+1, tail...>(ctx, src, result);
			if(req < 0) return on_error(req);
			return ret + req;
		}
	}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return -1;
		if constexpr (details::contains<decltype(this)>(decltype(auto(ctx)){}))
			return parse_seq<0, 0, parsers...>(ctx, src, result);
		else {
			auto cur_ctx = details::push_front(ctx, this, &result, &src);
			return parse_seq<0, 0, parsers...>(cur_ctx, src, result);
		}
	}
};
template<typename... parsers> struct opt_seq_parser : com_seq_parser<opt_seq_parser<parsers...>, parsers...> {
	using base_t = com_seq_parser<opt_seq_parser<parsers...>, parsers...>;
	constexpr opt_seq_parser(auto&&... args) requires (sizeof...(parsers)==sizeof...(args)): base_t(static_cast<decltype(args)&&>(args)...) {}
	constexpr auto on_error(auto val) const { return val; }
};
template<auto ind, auto ctx_chunk_size, auto ctx_result_pos> struct seq_reqursion_parser : base_parser<seq_reqursion_parser<ind, ctx_chunk_size, ctx_result_pos>> {
	static_assert( ctx_chunk_size > ctx_result_pos, "we need to extract result from ctx"  );
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto& ctx_r = *get<ind*ctx_chunk_size+ctx_result_pos>(ctx);
		return !!src ? get<ind*ctx_chunk_size>(ctx)->parse(ctx, static_cast<decltype(src)&&>(src), ctx_r) : -1;
	}
};
template<auto ind> const auto req = seq_reqursion_parser<ind, 3, 1>{};
constexpr struct cur_shift_parser : base_parser<cur_shift_parser> {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto& ctx_src = *get<2>(ctx);
		details::eq(result, details::pos(src) - details::pos(ctx_src));
		return 0;
	}
} cur_shift;

template<parser type> struct seq_error_parser : type {
	template<template<typename...>class tuple, typename... args>
	constexpr auto& extract_first(const tuple<args...>& ctx) const {
		return get<sizeof...(args)-1>(ctx);
	}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = type::parse(static_cast<decltype(ctx)&&>(ctx), src, result);
		auto& first = extract_first(ctx);
		if constexpr (!requires{ (*first)(src,src,result,ret); } ) return ret;
		else return (*first)(*get<2>(ctx), src, result, ret);
	}
	//TODO: implement operator [] here for we can call it like
	//  foo >> bar > char_<'a'>["some name"]
	// it will rise an error with name
	// for example
	template<typename value_t> constexpr auto operator[](value_t&& val) const {
		struct error_with_val : seq_error_parser<type> { value_t val; };
		return error_with_val{ val };
	}
};

template<parser left, typename right> constexpr auto operator>>(const left& l, const right& r) { return opt_seq_parser<left, right>{ l, r }; }
template<typename... left, typename right> constexpr auto operator>>(const opt_seq_parser<left...>& l, const right& r) { return details::init_with_get<opt_seq_parser<left..., right>>(l.seq, r); }
template<parser left> constexpr auto operator>>(const left& l, char r) { return l >> value_parser( r ); }
template<typename... left, parser right> constexpr auto operator>(const opt_seq_parser<left...>& l, const right& r) {return l >> seq_error_parser{r}; }
template<parser p> constexpr auto operator++(const p& l) { return seq_inc_rfield_before{ l }; }
template<parser p> constexpr auto operator++(const p& l,int) { return seq_inc_rfield_after{ l }; }
template<parser p> constexpr auto operator--(const p& l) { return seq_dec_rfield_before{ l }; }
template<parser p> constexpr auto operator--(const p& l,int) { return seq_dec_rfield_after{ l }; }
template<auto cnt, parser type> constexpr auto finc(const type& p) { return seq_inc_rfield_val{_seq_inc_rfield_val<cnt,type>{p}}; }
template<parser type> constexpr auto must(const type& p) { return seq_error_parser{ p }; }

template<parser p> struct opt_parser : p {
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return 0;
		auto ret = p::parse(ctx, src, result);
		return ret * (ret >= 0);
	}
};

template<parser p> constexpr auto operator-(const p& l){ return opt_parser{l}; }

template<parser left, parser right> struct different_parser : base_parser<different_parser<left, right>> {
	left lp;
	right rp;
	constexpr different_parser( left l, right r ) : lp(l), rp(r) {}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		details::type_any_eq_allow fake_result;
		if(rp.parse(ctx, src, fake_result) >= 0) return -1;
		return lp.parse(ctx, src, result);
	}
};

template<parser left, parser right> constexpr auto operator-(const left& l, const right& r) { return different_parser( l, r ); }

template<parser p> struct skip_parser : p {};
template<parser p> struct lexeme_parser : p {};
constexpr auto skip(const auto& p) { return skip_parser{ p }; }
constexpr auto lexeme(const auto& p) { return lexeme_parser{ p }; }

namespace literals {

template<typename char_t, char_t... chars>
constexpr auto operator""_lex() {
	return lexeme( omit( (... >> char_<chars>) ) );
}

} // namespace literals

// ===============================
//          injection part
// ===============================

template<bool apply,auto... inds, template<typename...>class result_t, typename... parsers, typename injection_t>
constexpr const auto make_seq_injection(const result_t<parsers...>& p, const injection_t& inject, auto&&... args) {
        if constexpr (sizeof...(inds) == sizeof...(parsers))
                return result_t<decltype(auto(args))...>( args... );
        else {
                auto& new_arg = get<sizeof...(inds)>(p.seq);
                if constexpr (apply) return make_seq_injection<apply,inds..., sizeof...(inds)>(
                        p, inject, std::forward<decltype(args)>(args)...,
                        inject, inject_parser<apply>(new_arg, inject)
                );
                else return make_seq_injection<apply,inds..., sizeof...(inds)>(
                        p, inject, std::forward<decltype(args)>(args)...,
                        inject_parser<apply>(new_arg, inject)
                );
        }
}

//template<bool apply> constexpr auto inject_to_list(const auto& p, const auto& s){ return s >> inject_parser<apply>(p,s); }
template<bool apply, parser... plist, template<typename...>class seq_parser>
constexpr auto inject_to_list_seq(const seq_parser<plist...>& p, const auto& s) { return inject_parser<apply>(p,s); }

template<bool apply> constexpr auto& inject_parser(const auto& p, const auto& s) { return p; }
template<bool apply, parser... parsers> constexpr auto inject_parser(const opt_seq_parser<parsers...>& p, const auto& s) { return make_seq_injection<apply>(p,s); }
template<bool apply, template<typename>class wrapper, parser parser_t, typename injection_t>
constexpr const auto inject_parser(const wrapper<parser_t>& p, const injection_t& inject) {
	return wrapper{ inject_parser<apply>(static_cast<const parser_t&>(p), inject) };
}
template<bool apply, template<typename,typename>class wrapper, parser p1, parser p2, typename injection_t>
constexpr const auto inject_parser(const wrapper<p1,p2>& p, const injection_t& inject)
requires (!requires{ static_cast<const opt_seq_parser<p1,p2>&>(p); }) {
	const auto& [a,b] = p;
	return wrapper{ inject_parser<apply>(a,inject), inject_parser<apply>(b,inject) };
}
template<bool apply, typename parser, typename injection_t>
constexpr const auto inject_parser(const unary_opt_list_parser<parser>& p, const injection_t& inject) {
	if constexpr (requires{ inject_to_list_seq<apply>(static_cast<const parser&>(p), inject); })
		return *inject_to_list_seq<apply>(static_cast<const parser&>(p), inject);
	else return *(inject >> static_cast<const parser&>(p));
}
template<bool apply, typename parser, typename injection_t>
constexpr const auto inject_parser(const unary_req_list_parser<parser>& p, const injection_t& inject) {
	if constexpr (requires{ static_cast<const seq_tag&>(p); })
		return +inject_to_list_seq<apply>(static_cast<const parser&>(p), inject);
	else return +(inject >> static_cast<const parser&>(p));
}
template<bool apply, typename parser, typename injection_t>
constexpr const auto inject_parser(const skip_parser<parser>& p, const injection_t& inject) {
	return inject_parser<true>(static_cast<const parser&>(p), inject);
}
template<bool apply, typename parser, typename injection_t>
constexpr const auto inject_parser(const lexeme_parser<parser>& p, const injection_t& inject) {
	return inject_parser<false>(static_cast<const parser&>(p), inject);
}
template<bool apply, typename parser, typename action, typename injection_t>
constexpr const auto inject_parser(const semact_parser<parser, action>& p, const injection_t& inject) {
	return inject_parser<apply>(static_cast<const parser&>(p), inject)(p.act);
}

// ===============================
//          tests part
// ===============================

constexpr auto make_test_ctx() { return std::tuple<int>{0}; }
constexpr auto make_test_ctx(auto arg) { return std::tuple(arg); }

template<auto ind, typename parser> constexpr void test_base_parser() {
	static_assert( ({
		auto result = parser::valid_result[0];
		parser{}.parse(make_test_ctx(), make_source(parser::valid_data[ind]), result);
		result;
	}) == parser::valid_result[ind], "parse valid data and get valid result");
	static_assert( ({
		auto result = parser::valid_result[0];
		parser{}.parse(make_test_ctx(), make_source(parser::valid_data[ind]), result);
	}) == details::test_data_size(parser::valid_data[ind]), "parsing valid data returns 1 - 1 symbol");
	if constexpr (ind+1 < details::arrsize(parser::valid_result)) test_base_parser<ind+1, parser>();
}
template<auto ind, typename parser> constexpr void test_base_parser_invalid() requires(!requires{parser::invalid_data;}){}
template<auto ind, typename parser> constexpr void test_base_parser_invalid() {
	static_assert( ({
		auto result = parser::valid_result[0];
		parser{}.parse(make_test_ctx(), make_source(parser::invalid_data[ind]), result);
	}) == -1, "parse invalid returns -1: parsing error sing");
	if constexpr (ind+1 < details::arrsize(parser::invalid_data)) test_base_parser_invalid<ind+1, parser>();
}
template<auto ind, typename parser> constexpr void test_negate_parser() {
	if constexpr (requires{ parser::valid_data[ind]; } && ind < details::arrsize(parser::valid_data)) static_assert( ({
		auto result = parser::valid_result[0];
		(!parser{}).parse(make_test_ctx(), make_source(parser::valid_data[ind]), result);
	}) == -1, "parsing valid data with negate parser is an error");
	if constexpr (ind+1 < details::arrsize(parser::valid_data)) test_negate_parser<ind+1, parser>();
}
template<auto ind, typename parser> constexpr void test_negate_parser_invalid() requires(!requires{parser::invalid_data;}){ }
template<auto ind, typename parser> constexpr void test_negate_parser_invalid() {
	static_assert( ({
		auto result = parser::valid_result[0];
		(!parser{}).parse(make_test_ctx(), make_source(parser::invalid_data[ind]), result);
	}) == 0, "parsing invalid data with negate parser is 0 shift");
	if constexpr (ind+1 < details::arrsize(parser::invalid_data)) test_negate_parser_invalid<ind+1,parser>();
}
template<auto ind, typename factory, typename parser> constexpr void test_as_parser() {
	static_assert( ({ char result; as(parser{}, 'a').parse(make_test_ctx(), make_source(parser::valid_data[ind]), result);
	}) > 0, "as parser can parse based parser");
	static_assert( ({ char result; as(parser{}, 'a').parse(make_test_ctx(), make_source(parser::valid_data[ind]), result); result;
	}) == 'a', "as parser results as required result");
	static_assert( ({
		auto result = factory{}.mk_str();
		as(parser{}, factory{}.mk_str("test")).parse(make_test_ctx(), make_source(parser::valid_data[ind]), result);
		result == "test";
	}) == true, "as parser results as required result (with strings)");
	if constexpr (ind+1 < details::arrsize(parser::valid_data)) test_as_parser<ind+1, factory, parser>();
}
template<auto ind, typename parser> constexpr void test_as_parser_invalid() requires(!requires{parser::invalid_data;}){}
template<auto ind, typename parser> constexpr void test_as_parser_invalid() {
	static_assert( ({
		char result;
		as(parser{}, 'a').parse(make_test_ctx(), make_source(parser::invalid_data[ind]), result);
	}) < 0, "as parser cannot parse based parser with invalid data");
	if constexpr (ind+1 < details::arrsize(parser::invalid_data)) test_as_parser_invalid<ind+1, parser>();
}

template<typename factory, typename p1, typename p2>
constexpr void test_two_parsers() { if constexpr (std::is_same_v<p1,p2>) {} else {
	static_cast<const p1&>(inject_parser<true>(p1{}, p2{}));
	static_cast<const opt_seq_parser<p2, p1, p2, p1>&>(inject_parser<true>(p1{} >> p1{}, p2{}));
	static_cast<const unary_req_list_parser<opt_seq_parser<p2, p1>>&>(inject_parser<true>(+(p1{}), p2{}));
	static_cast<const unary_req_list_parser<opt_seq_parser<p2, p1, p2, p1>>&>(inject_parser<true>(+(p1{} >> p1{}), p2{}));
	static_cast<const unary_req_list_parser<opt_seq_parser<p1, p1>>&>(inject_parser<true>(lexeme(+(p1{} >> p1{})), p2{}));
	static_cast<const opt_seq_parser< p1, opt_seq_parser<p2,p1,p2,p1> >&>(inject_parser<true>(lexeme(p1{} >> skip(p1{}>>p1{})), p2{}));
	static_cast<const opt_parser<opt_seq_parser< p2, p1, p2, p1 >>&>( inject_parser<true>(-(p1{} >> p1{}), p2{}) );
	static_cast<const opt_seq_parser<
		p2, p1, p2,
		opt_parser<
		  opt_seq_parser< p2, p1, p2, p1 >
		>
	>&>( inject_parser<true>(p1{} >> -(p1{} >> p1{}), p2{}) );
	static_cast<const opt_seq_parser<
		p2, p1, p2,
		different_parser<
		  opt_seq_parser< p2, p1, p2, p1 >,
		  p1
		>
	>&>( inject_parser<true>(p1{} >> (p1{}>>p1{}) - p1{}, p2{}) );
	static_assert( ({ char r='z'; int t=0;
		auto p = char_<'a'> >> char_<'b'>([&t](...){++t;});
		p.parse(make_test_ctx(), make_source(factory{}.mk_str("ab")), r); t;
	}) == 1, "injection works with semact" );
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_parser<true>(char_<'a'> >> char_<'b'>([&t](...){++t;}), -omit(space));
		p.parse(make_test_ctx(), make_source(factory{}.mk_str("ab")), r); t;
	}) == 1, "injection works with semact" );
}}

template<typename factory_t, typename cur_parser, template<typename...> class parsers_set, typename... parsers>
constexpr void test_two_parsers(const parsers_set<parsers...>& list) { ( test_two_parsers<factory_t, cur_parser, parsers>(), ... ); }

template<typename factory_t, template<typename...> class parsers_set, typename... parsers>
constexpr void test_base_parsers(const parsers_set<parsers...>& list) {
	( test_base_parser<0, parsers>(), ... );
	( test_base_parser_invalid<0, parsers>(), ... );
	( test_negate_parser<0, parsers>(), ... );
	( test_negate_parser_invalid<0, parsers>(), ... );
	( test_as_parser<0, factory_t, parsers>(), ... );
	( test_as_parser_invalid<0, parsers>(), ... );
	( test_two_parsers<factory_t, parsers>(list), ... );
}

template<typename factory>
constexpr void test_variant_parser() {
	static_assert( ({ char r; (char_<'a'> | char_<'b'>).parse(make_test_ctx(), make_source("a"), r);
	}) == 1, "can parse valid data");
	static_assert( ({ char r; (char_<'a'> | char_<'b'>).parse(make_test_ctx(), make_source("b"), r);
	}) == 1, "can parse valid data");
	static_assert( ({ char r; (char_<'a'> | char_<'b'> | digit).parse(make_test_ctx(), make_source("0"), r);
	}) == 1, "can parse valid data and got valid result");
	static_assert( ({ char r; (char_<'a'> | char_<'b'>).parse(make_test_ctx(), make_source("a"), r); r;
	}) == 'a', "can parse valid data and got valid result");
	static_assert( ({ char r; (char_<'a'> | char_<'b'>).parse(make_test_ctx(), make_source("b"), r); r;
	}) == 'b', "can parse valid data and got valid result");
	static_assert( ({ char r; (char_<'a'> | 'b' | digit).parse(make_test_ctx(), make_source("0"), r); r;
	}) == '0', "can parse valid data and got valid result");
	static_assert( ({ char r; (char_<'a'> | 'b').parse(make_test_ctx(), make_source("c"), r);
	}) == -1, "cannot parse invalid data");
	static_assert( ({ char r='z'; inject_parser<true>((char_<'%'> | d10 | lower), *space).parse(make_test_ctx(), make_source("a"), r); r;
	}) == 'a', "make sure the variant works with injection" );
}
template<typename factory>
constexpr void test_other_parsers() {
	static_assert( ({ char r = 'c'; omit(char_<'a'>).parse(make_test_ctx(), make_source("a"), r);
	}) == 1, "omit parser returns count of parsed symbols");
	static_assert( ({ char r = 'c'; omit(char_<'a'>).parse(make_test_ctx(), make_source("b"), r);
	}) == -1, "omit parser returns error if fails");
	static_assert( ({ char r = 'c'; omit(char_<'a'>).parse(make_test_ctx(), make_source("a"), r);r;
	}) == 'c', "omit parser leaves the result as is");

	static_assert( ({ auto r = factory{}.template mk_vec<char>(); (*char_<'a'>).parse(make_test_ctx(), make_source(""), r);
	}) == 0, "* parser is zero on no data");
	static_assert( ({ auto r = factory{}.template mk_vec<char>(); (+char_<'a'>).parse(make_test_ctx(), make_source(""), r);
	}) == -1, "+ parser fails on no data");
	static_assert( ({ auto r = factory{}.template mk_vec<char>(); (+char_<'a'>).parse(make_test_ctx(), make_source("aaa"), r);
	}) == 3, "+ parser parses valid data");
	static_assert( ({ auto r = factory{}.template mk_vec<char>(); (+(char_<'a'>|char_<'b'>)).parse(make_test_ctx(), make_source("aab"), r);r[2];
	}) == 'b', "+ parser parses valid data");
	static_assert( ({ auto r = factory{}.template mk_vec<char>(); (*(char_<'a'>|char_<'b'>)).parse(make_test_ctx(), make_source("aab"), r);r[2];
	}) == 'b', "* parser parses valid data");

	static_assert( ({char r='c';(char_<'a'> >> 'b').parse(make_test_ctx(), make_source("ab"), r); r;
	}) == 'b', "seq parser: just override if same field" );
	static_assert( ({char r='c';(char_<'a'> >> char_<'b'>).parse(make_test_ctx(), make_source("ab"), r);
	}) == 2, "seq parser: just override if same field" );
	static_assert( ({char r='c';(char_<'a'> >> char_<'b'>).parse(make_test_ctx(), make_source("ab"), r); r;
	}) == 'b', "seq parser: just override if same field" );
	static_assert( ({auto r=factory{}.mk_str();(char_<'a'> >> char_<'b'>).parse(make_test_ctx(), make_source("ab"), r); r[1];
	}) == 'b', "seq parser: just override if same field" );
	struct seq_result2 { decltype(factory{}.mk_str()) rstr; char rch='z'; };
	static_assert( ({seq_result2 r;(char_<'a'> >> char_<'b'> >> ++char_<'c'>).parse(make_test_ctx(), make_source("abc"), r);r.rch * (r.rstr=="ab");
	}) == 'c', "seq parser: can shift fields" );
	static_assert( ({seq_result2 r;(char_<'a'> >> char_<'b'>++ >> char_<'c'>).parse(make_test_ctx(), make_source("abc"), r);r.rch * (r.rstr=="ab");
	}) == 'c', "seq parser: can shift fields" );
	static_assert( ({seq_result2 r;(char_<'a'> >> char_<'b'> >> sfs >> char_<'c'>).parse(make_test_ctx(), make_source("abc"), r);r.rch * (r.rstr=="ab");
	}) == 'c', "seq parser: can shift fields" );
	static_assert( ({seq_result2 r;(char_<'a'>++ >> char_<'b'>-- >> char_<'c'>).parse(make_test_ctx(), make_source("abc"), r);r.rch * (r.rstr=="ac");
	}) == 'b', "seq parser: can shift fields" );
	static_assert( ({seq_result2 r;(++char_<'a'> >> --char_<'b'> >> char_<'c'>).parse(make_test_ctx(), make_source("abc"), r);r.rch * (r.rstr=="bc");
	}) == 'a', "seq parser: can shift fields" );
	static_assert( ({seq_result2 r;(finc<1>(char_<'a'>) >> finc<-1>(char_<'b'>) >> char_<'c'>).parse(make_test_ctx(), make_source("abc"), r);r.rch * (r.rstr=="bc");
	}) == 'a', "seq parser: can shift fields" );
	static_assert( ({char r='c';(char_<'a'> >> char_<'b'>).parse(make_test_ctx(), make_source("cc"), r);}) == -1, "seq parser: -1 if fails" );
	static_assert( ({char r='c';(char_<'a'> >> char_<'b'>).parse(make_test_ctx(), make_source("ac"), r);}) == -1, "seq parser: -1 if fails" );
	static_assert(
		({char r='c';(char_<'a'> >> char_<'u'> >> char_<'b'>).parse(make_test_ctx(), make_source("ab"), r);}) == -1,
		"seq parser: not use next if fails" );
	static_assert(
		({char r='c';(char_<'a'> >> char_<'u'> >> char_<'b'>).parse(make_test_ctx(), make_source("ab"), r);r;}) == 'a',
		"seq parser: not use next if fails" );

	static_assert( ({char r='c';(char_<'a'> >> -char_<'u'>).parse(make_test_ctx(), make_source("ab"), r);}) == 1, "can parser optional" );
	static_assert( ({char r='c';(char_<'a'> >> -char_<'u'>).parse(make_test_ctx(), make_source("ab"), r);r;}) == 'a', "can parser optional" );
	static_assert( ({char r='c';(char_<'a'> >> -char_<'u'> >> char_<'b'>).parse(make_test_ctx(), make_source("ab"), r);}) == 2, "can parser optional" );
	static_assert( ({char r='c';(char_<'a'> >> -char_<'u'> >> char_<'b'>).parse(make_test_ctx(), make_source("ab"), r);r;}) == 'b', "can parser optional" );

	static_assert( ({char r='z';(digit % ',').parse(make_test_ctx(), make_source("1,2"), r);}) == 3, "can parse binary list" );
	static_assert( ({char r='z';(digit % char_<','>).parse(make_test_ctx(), make_source("1,2"), r);}) == 3, "can parse binary list" );
	static_assert( ({char r='z';(digit % char_<','>).parse(make_test_ctx(), make_source("1,2,3,4"), r);r;}) == '4', "can parse binary list" );
	static_assert( ({char r='z';(digit % char_<','>).parse(make_test_ctx(), make_source("1,"), r);}) == 1, "can parse binary list" );

	static_assert( ({char r='z';(*(any - char_<'a'>)).parse(make_test_ctx(), make_source("#$%a"), r);}) == 3, "different parser: stops on it's excluding parser" );
	static_assert( ({char r='z';(*(any - char_<'a'>)).parse(make_test_ctx(), make_source("#$%a"), r);r;}) == '%', "different parser: excluded left result as is" );

	static_assert( ({char r='z',a='y';(char_<'a'> >> char_<'b'>([&a](){a='c';})).parse(make_test_ctx(),make_source("ab"),r);a;}) == 'c', "semact: calls if parsed");
	static_assert( ({char r='z',a='y';(char_<'a'> >> char_<'b'>([&a](){a='c';})).parse(make_test_ctx(),make_source("au"),r);a;}) == 'y', "semact: silent if fails");

	static_assert( ({
		char r='z', l='a';
		(char_<'a'> >> char_<'b'> >> [&](auto src_before, auto src_after, const auto& result) {
		 l='u' * ((src_before.ind + 2 == src_after.ind) && result == 'b');
		}).parse(make_test_ctx(), make_source(factory{}.mk_str("ab")), r);
		l; }) == 'u', "invoke method after finish parsing" );
	static_assert(
		({ char r='z'; (char_<'a'> >> char_<'b'> >> [&](auto, auto, const auto&) {}).parse(make_test_ctx(), make_source("ab"), r);}) == 2,
		"invoke method after finish parsing: no return value returns 0" );
	static_assert(
		({ char r='z'; (char_<'a'> >> char_<'b'> >> [&](auto, auto, const auto&) {return 1;}).parse(make_test_ctx(), make_source("ab"), r);}) == 3,
		"invoke method after finish parsing: return value affects" );
	static_assert(
		({ char r='z'; (char_<'a'> >> char_<'b'> >> [&](auto, auto, const auto&,auto) {return 3;}).parse(make_test_ctx(), make_source("ab"), r);}) == 5,
		"invoke method after finish parsing: if ctx accepted return valud as parser result" );

	static_assert(
		({char r='z';(char_<'a'> >> char_<'b'> >> must(char_<'c'>)).parse(make_test_ctx(), make_source("abd"), r);}) == -1,
		"if error and no lambda - nothing changed");
	static_assert( ({
		char r='z';
		const auto err_method = [&](...){return -10;};
		(char_<'a'> >> char_<'b'> >> must(char_<'c'>)).parse(make_test_ctx(&err_method), make_source("abe"), r);
	}) == -10, "error lambda are called");
	static_assert( ({
		char r=0x00;
		const auto err_method = [&](auto before, auto after, auto& result, auto ret){ return -3 * (before.ind == 0 && after.ind==2);};
		(char_<'a'> >> char_<'b'> > char_<'c'>).parse(make_test_ctx(&err_method), make_source("abe"), r);
	}) == -3, "on error: sources are on start sequence and on rule where the error");

	static_assert( ({
		struct { char a='z', b='z'; int pos; } r;
		(char_<'a'> >> ++char_<'b'> >> ++cur_pos).parse(make_test_ctx(), make_source("ab"), r); r.pos;
	}) == 2, "can parse current position");
	static_assert( ({
		struct { char a='z', b='z'; struct { char c,d; int pos;} i; } r;
		(char_<'a'>++ >> char_<'b'>++ >> (char_<'c'> >> ++char_<'d'> >> ++cur_shift)).parse(make_test_ctx(), make_source("abcd"), r); r.i.pos;
	}) == 2, "can parse current shift");

	static_assert( ({ auto r=0; (int_>>omit(lower)).parse(make_test_ctx(), make_source(factory{}.mk_str("1a")), r); r; }) == 1 );

}
template<typename factory>
constexpr void test_literals() {
	using namespace literals;
	static_cast<const lexeme_parser<omit_parser<opt_seq_parser<char_parser<'a'>,char_parser<'b'>>>>&>( "ab"_lex );
}
template<typename factory>
constexpr void test_reqursion_parsers() {
	static_assert( ({ char r='c'; ((char_<'a'>|'b') >> -(omit(char_<'('>) >> req<1> >> omit(char_<')'>))).parse(make_test_ctx(), make_source("a(b)"), r);
	}) == 4, "can parse reqursion");
	static_assert( ({ char r='c'; ((char_<'a'>|'b') >> -(omit(char_<'('>) >> req<1> >> omit(char_<')'>))).parse(make_test_ctx(), make_source("a(b)"), r);r;
	}) == 'b', "can parse reqursion");
	static_assert( ({
		auto r=factory{}.mk_str();
		((char_<'a'>|'b') >> -(omit(char_<'('>) >> req<1> >> omit(char_<')'>))).parse(make_test_ctx(), make_source("a(b)"), r);
		r[0] * (r[1]=='b');
	}) == 'a', "can parse reqursion");
	static_assert( ({
		auto r=factory{}.mk_str();
		((char_<'a'>|'b'|'c') >> -(omit(char_<'('>) >> req<1> >> omit(char_<')'>))).parse(make_test_ctx(), make_source("a(b(c))"), r);
	}) == 7, "can parse reqursion: deeper");
	static_assert( ({
		auto r=factory{}.mk_str();
		((char_<'a'>|'b'|'c') >> -(omit(char_<'('>) >> req<1> >> omit(char_<')'>))).parse(make_test_ctx(), make_source("a(b(c))"), r);
		r[0] * (r[1]=='b' && r[2]=='c');
	}) == 'a', "can parse reqursion: deeper");
	static_assert( ({ char r='c'; ((char_<'a'>|'b') >> -(omit(char_<'('>) >> -req<0> >> omit(char_<')'>))).parse(make_test_ctx(), make_source("a(())"), r);
	}) == 5, "can parse reqursion");
}

constexpr auto parse(const auto& parser, auto src, auto& result) {
	return parser.parse(make_test_ctx(), src, result);
}

constexpr auto parse(const auto& parser, const auto& skip, auto src, auto& result) {
	auto ctx = make_test_ctx();
	return inject_parser<true>(parser, skip).parse(make_test_ctx(), src, result);
}

template<typename factory_t>
void test() {
	static_assert( details::arrsize("ab") == 3 );
	static_assert( vector<decltype(factory_t{}.template mk_vec<int>())>, "vector concepts works and factory mk_vec methods returns vector" );
	static_assert( string<decltype(factory_t{}.template mk_str())>, "string concepts works and factory mk_str methods returns string" );
	static_assert( vector<decltype(factory_t{}.template mk_str())>, "string is also a vector" );
	static_assert( factory_t{}.mk_str("hello") == "hello", "we can create string with some value and compare it" );

	struct refl_test{ char f1='a'; int f2=10'000; };
	static_assert( ({refl_test r;reflect::get<0>(r);}) == 'a', "cat extract field by number" );
	static_assert( ({refl_test r;reflect::get<1>(r);}) == 10'000, "cat extract field by number" );

	static_assert( ({details::tuple<char,int> t{'a', 10'000};get<0>(t);}) == 'a', "cat use our tuple" );
	static_assert( ({details::tuple<char,int> t{'a', 10'000};get<1>(t);}) == 10'000, "cat use our tuple" );

	using base_parsers_set = details::types_set<
		char_parser<'a'>, range_parser<'a', 'c'>, any_parser, int_parser,
		decltype(auto(lower)), decltype(auto(upper)), decltype(auto(digit))>;

	test_src<factory_t>();
	test_base_parsers<factory_t>(base_parsers_set{});
	test_variant_parser<factory_t>();
	test_other_parsers<factory_t>();
	test_literals<factory_t>();
	test_reqursion_parsers<factory_t>();
}

} // namespace ascip
