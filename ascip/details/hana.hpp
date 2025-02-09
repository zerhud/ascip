#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>

namespace ascip_details {

template<typename type, template<typename...>class tmpl> constexpr bool is_specialization_of = false;
template<template<typename...>class type, typename... args> constexpr bool is_specialization_of<type<args...>, type> = true;
template<template<typename...>class tmpl> constexpr static auto is_spec_checker = [](const auto* p) { return is_specialization_of<std::decay_t<decltype(*p)>, tmpl>; };

template<typename...> struct type_list {};
template<typename,auto...> struct seq_type {};
template<typename t> struct type_holder { using type = t; t operator+() const ; };
template<typename t> constexpr auto type_c = type_holder<t>{};
template<typename t> constexpr auto type_dc = type_holder<std::decay_t<t>>{};
template<typename l, typename r> constexpr bool operator==(type_holder<l>, type_holder<r>) { return false; }
template<typename t> constexpr bool operator==(type_holder<t>, type_holder<t>) { return true; }
template<typename type, typename... list> constexpr bool contains(type_list<list...>, type_holder<type> s) {
	return ((s == type_c<list>) + ... );
}
template<typename... in_list, typename... what> constexpr bool contains_all(type_list<in_list...> in, type_list<what...>) {
	return (contains(in, type_c<what>) * ...);
}
template<typename... in_list, typename... what> constexpr bool contains_any(type_list<in_list...> in, type_list<what...>) {
	return (contains(in, type_c<what>) + ...);
}

static_assert( contains(type_list<int,char>{}, type_c<int>) );
static_assert( !contains(type_list<int,char>{}, type_c<double>) );
static_assert( contains_all(type_list<int,char,double>{}, type_list<int>{}) );
static_assert( contains_all(type_list<int,char,double>{}, type_list<int,double>{}) );
static_assert( !contains_all(type_list<int,char,double>{}, type_list<int,unsigned>{}) );
static_assert( contains_any(type_list<int,char,double>{}, type_list<int,unsigned>{}) );
static_assert( !contains_any(type_list<int,char,double>{}, type_list<long,unsigned>{}) );

template<typename type, auto ind> struct tuple_value {
	type value; //NOTE: we cannot downcast for some reason in get method later, so we need in the fucky g methods
	template<auto i> constexpr type& g() requires (i==ind) { return value; }
	template<auto i> constexpr const type& g() const requires (i==ind) { return value; }
};
template<typename... types> struct tuple {
	consteval static auto mk_storage_type() {
		return []<typename fucky_clang,auto... inds>(seq_type<fucky_clang,inds...>){
			struct storage : tuple_value<types, inds>... { using tuple_value<types, inds>::g...; };
			return type_holder<storage>{};
		}(
#if __has_builtin(__integer_pack)
			seq_type<unsigned, __integer_pack(sizeof...(types))...>{}
#else
			__make_integer_seq<seq_type, unsigned, sizeof...(types)>{}
#endif
		);
	}

	using storage_type = decltype(+mk_storage_type());
	storage_type storage;

	constexpr tuple() =default ;
	constexpr tuple(tuple&& other) : storage(std::move(other.storage)) {}
	constexpr tuple(const tuple& other) : storage(other.storage) {}
	constexpr explicit tuple(auto&&... args) : storage{std::forward<decltype(args)>(args)...} {}

	template<auto ind> constexpr friend auto& get(tuple& t) { return t.storage.template g<ind>(); }
	template<auto ind> constexpr friend const auto& get(const tuple& t) { return t.storage.template g<ind>(); }
	friend constexpr auto size(const tuple&) { return sizeof...(types); }
};

template<typename... types> tuple(types&&...) -> tuple<std::decay_t<types>...>;

} // namespace ascip_details
