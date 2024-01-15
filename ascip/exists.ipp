#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<auto... inds, template<typename...>class wrapper, typename... parsers>
constexpr static bool exists_in_get(const wrapper<parsers...>* seq, const auto& checker) {
	if constexpr (sizeof...(inds) == sizeof...(parsers)) return false;
	else return
		   checker((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr)
		|| exists_in((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr, checker)
		|| exists_in_get<inds..., sizeof...(inds)>(seq, checker)
		;
}
constexpr static bool exists_in(auto&& src, const auto& checker) { return exists_in(&src, checker); }
template<template<typename>class wrapper, typename parser>
constexpr static bool exists_in_drived(const wrapper<parser>* src, const auto& checker) {
	return exists_in(static_cast<const parser*>(src), checker); }
constexpr static bool exists_in(auto* src, const auto& checker) {
	if constexpr (std::is_same_v<typename std::decay_t<decltype(*src)>::type_in_base, std::decay_t<decltype(*src)>>)
		return checker(src);
	else return exists_in_drived(src, checker);
}
constexpr static bool exists_in(auto* src, const auto& checker) requires requires{ src->p; } {
	using type = std::decay_t<decltype(src->p)>;
	return checker(src) || exists_in((type*)nullptr, checker); }
constexpr static bool exists_in(auto* src, const auto& checker) requires requires{ src->lp; } {
	using ltype = std::decay_t<decltype(src->lp)>;
	using rtype = std::decay_t<decltype(src->rp)>;
	return checker(src)
		|| checker((ltype*)nullptr) || checker((rtype*)nullptr)
		|| exists_in((ltype*)nullptr, checker) || exists_in((rtype*)nullptr, checker)
		;
}
constexpr static bool exists_in(auto* src, const auto& checker) requires requires{ src->seq; } {
	using seq_t = decltype(src->seq);
	return checker(src) || exists_in_get((seq_t*)nullptr, checker); }
constexpr static bool exists_in(auto* src, const auto& checker) requires requires{ src->s; src->b; } {
	//NOTE: it's for injected_parser, but without forward declaration
	return exists_in((std::decay_t<decltype(src->b)>*)nullptr, checker) ; }

constexpr static bool test_exists_in() {
	auto checker = [](const auto* s){ return std::is_same_v<std::decay_t<decltype(*s)>, std::decay_t<decltype(char_<'a'>)>>; };
	static_assert(  exists_in(char_<'a'>, checker) );
	static_assert( !exists_in(char_<'b'>, checker) );
	static_assert(  exists_in(skip(char_<'a'>), checker) );
	static_assert( !exists_in(skip(char_<'b'>), checker) );
	static_assert(  exists_in(char_<'b'> | char_<'a'>, checker) );
	static_assert( !exists_in(char_<'b'> | char_<'x'>, checker) );
	static_assert(  exists_in(char_<'b'> - char_<'a'>, checker) );
	static_assert( !exists_in(char_<'b'> - char_<'x'>, checker) );
	static_assert(  exists_in(char_<'c'> >> (char_<'b'> - char_<'a'>), checker) );
	static_assert(  exists_in(char_<'c'> - (char_<'b'> >> char_<'a'>), checker) );
	static_assert(  exists_in(skip(char_<'c'> >> char_<'b'> >> char_<'a'>++), checker) );
	return true;
}
