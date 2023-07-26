#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details {

template<typename type, template<typename...>class tmpl> constexpr const bool is_specialization_of = false;
template<template<typename...>class type, typename... args> constexpr const bool is_specialization_of<type<args...>, type> = true;

struct type_any_eq_allow {
	template<typename type> constexpr auto& operator =(const type&){ return *this; }
	template<typename type> constexpr auto& operator+=(const type&){ return *this; }
	template<typename type> constexpr auto& operator-=(const type&){ return *this; }
	template<typename type> constexpr auto& operator/=(const type&){ return *this; }
	template<typename type> constexpr auto& operator*=(const type&){ return *this; }
};
struct type_result_for_parser_concept : type_any_eq_allow {using type_any_eq_allow::operator=;};

template<typename type> concept vector = requires(const type& v){ v.size(); v[0]; v.back(); } && requires(type& v){ v.pop_back(); };
template<typename type> concept string = requires(type& s){ s += typename type::value_type{}; } && vector<type>;
template<typename type> concept string_view = requires(const type& s){ s.size(); s[0]; s.back(); };
template<typename type> concept empbackable = requires(type& r){ emplace_back(r); } || requires(type& r){ r.emplace_back(); };
template<typename type> concept parser = requires(type& p, type_result_for_parser_concept& r) {
	p.parse(make_test_ctx<1,2,3,4,5,6,7,8,' ','c','o','c','e','p','t',' ',1,2,3,4>(p), make_source(p), r) < 0; };
template<typename type> concept nonparser = !parser<type>;


template<template<typename...>class tuple, typename... args>
constexpr auto tuple_size(const tuple<args...>&) { return sizeof...(args); }

template<template<typename...>class tuple, typename... left_args, typename... extra_args>
constexpr auto push_back(tuple<left_args...>& to, extra_args&&... what) {
	using ret_t = tuple<left_args..., extra_args...>;
	return init_with_get<ret_t>(to, static_cast<extra_args&&>(what)...);
}
template<template<typename...>class tuple, typename... left_args, typename... extra_args>
constexpr auto push_front(tuple<left_args...>& to, extra_args&&... what) {
	using ret_t = tuple<extra_args..., left_args...>;
	return init_with_get_inv<ret_t>(to, static_cast<extra_args&&>(what)...);
}

constexpr bool is_in_concept_check(auto&& ctx) {
	if constexpr (tuple_size(decltype(auto(ctx)){})==1) return false;
	else return std::is_same_v<int,decltype(auto(get<tuple_size(decltype(auto(ctx)){})-2>(ctx)))>;
}

struct in_req_flag{ };
struct without_req_flag{ };
constexpr bool is_in_reqursion_check(auto&& ctx) {
	return std::is_same_v<in_req_flag,decltype(auto(get<tuple_size(decltype(auto(ctx)){})-1>(ctx)))>;
}

namespace { // tuple
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
} // namespace (anonymus) tuple
 
template<typename...> struct types_set{};
template<auto sz> constexpr auto arrsize(const auto(&)[sz]){ return sz; }
constexpr auto strlen(const auto* vec){ unsigned ret = 0; while(vec[ret++]); return ret; }

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

constexpr auto pos(const auto& src)
{
	//TODO: remove this function
	if constexpr ( requires{ position(src); } ) return position(src);
	else return src.ind;
}

template<class init_t, auto... inds, template<typename...>class src_t, typename... src_args_t, typename... args_t>
constexpr auto init_with_get(const src_t<src_args_t...>& src, args_t&&... args) {
	if constexpr (sizeof...(inds) == sizeof...(src_args_t)) return init_t{ get<inds>(src)..., static_cast<args_t&&>(args)... };
	else return init_with_get<init_t, inds..., sizeof...(inds)>(src, static_cast<args_t&&>(args)...);
}
template<class init_t, auto... inds, template<typename...>class src_t, typename... src_args_t, typename... args_t>
constexpr auto init_with_get_inv(const src_t<src_args_t...>& src, args_t&&... args) {
	if constexpr (sizeof...(inds) == sizeof...(src_args_t)) return init_t{ static_cast<args_t&&>(args)..., get<inds>(src)... };
	else return init_with_get_inv<init_t, inds..., sizeof...(inds)>(src, static_cast<args_t&&>(args)...);
}

template<typename arg, template<typename...>class tuple, typename... left_args>
constexpr const bool contains(const tuple<left_args...>*) { return (std::is_same_v<left_args,arg> || ...); }


template<auto cur, typename arg, template<typename...>class tuple, typename... left_args>
constexpr const auto find_first_impl(const tuple<left_args...>* v) {
	if constexpr (std::is_same_v<arg,decltype(auto(get<cur>(*v)))>) return cur;
	else return find_first_impl<cur+1,arg>(v);
}
template<typename arg, template<typename...>class tuple, typename... left_args>
constexpr const auto find_first(const tuple<left_args...>* v) {
	return find_first_impl<0,arg>(v);
}


#include "adl_methods.ipp"

} // namespace ascip_details


namespace ascip_reflection {
#include "get_field_from_struct.ipp"
template<auto ind> constexpr auto& get(ascip_details::type_result_for_parser_concept& r){ return r; }
} // namespace ascip_reflection
