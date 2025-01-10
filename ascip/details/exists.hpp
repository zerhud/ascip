#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


namespace ascip_details {
template<auto... inds, template<typename...>class wrapper, typename... parsers>
constexpr static bool exists_in_get(const wrapper<parsers...>* seq, const auto& checker, const auto& stop) {
	if constexpr (sizeof...(inds) == sizeof...(parsers)) return false;
	else return
		(!stop((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr)
		&& (
		      checker((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr)
		   || exists_in((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr, checker, stop)
		   )
		)
		|| exists_in_get<inds..., sizeof...(inds)>(seq, checker, stop)
		;
}
template<template<typename>class wrapper, typename parser>
constexpr static bool exists_in_derived(const wrapper<parser>* src, const auto& checker, const auto& stop) {
	if(stop(src)) return false;
	if(checker(src)) return true;
	return exists_in(static_cast<const parser*>(src), checker, stop);
}

constexpr static bool exists_in(auto&& src, const auto& checker, const auto& stop) {
	return exists_in(&src, checker, stop);
}

constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires (!requires{typename std::decay_t<decltype(*src)>::type_in_base;}){
	return false;
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) {
	if(stop(src)) return false;
	if(checker(src)) return true;
	if constexpr (std::is_same_v<typename std::decay_t<decltype(*src)>::type_in_base, std::decay_t<decltype(*src)>>)
		return checker(src);
	else return exists_in_derived(src, checker, stop);
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->p; } {
	using type = std::decay_t<decltype(src->p)>;
	return stop(src) ? false : checker(src) || exists_in((type*)nullptr, checker, stop);
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->lp; } {
	using ltype = std::decay_t<decltype(src->lp)>;
	using rtype = std::decay_t<decltype(src->rp)>;
	if(stop(src)) return false;
	const bool lstop = stop((ltype*)nullptr);
	const bool rstop = stop((rtype*)nullptr);
	return checker(src)
		|| (!lstop&&checker((ltype*)nullptr))
		|| (!rstop&&checker((rtype*)nullptr))
		|| (!lstop&&exists_in((ltype*)nullptr, checker, stop))
		|| (!rstop&&exists_in((rtype*)nullptr, checker, stop))
		;
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->seq; } {
	using seq_t = decltype(src->seq);
	if(stop(src)) return false;
	return checker(src) || exists_in_get((seq_t*)nullptr, checker, stop); }
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->s; src->b; } {
	//NOTE: it's for injected_parser, but without forward declaration
	if(stop(src)) return false;
	const auto* ptr = static_cast<const std::decay_t<decltype(src->b)>*>(nullptr);
	if(checker(ptr)) return true;
	return exists_in(ptr, checker, stop) ; }

template<typename p, template<auto>class t=p::template tmpl>
constexpr static bool test_exists_in() {
	auto checker = [](const auto* s){ return std::is_same_v<std::decay_t<decltype(*s)>, std::decay_t<decltype(t<'a'>::char_)>>; };
	auto pass = [](const auto* s){ return false; };
	auto stop = [](const auto* s){ return true; };
	static_assert(  exists_in(t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_, checker, stop) );
	static_assert(  exists_in(skip(t<'a'>::char_), checker, pass) );
	static_assert( !exists_in(skip(t<'a'>::char_), checker, stop) );
	static_assert( !exists_in(skip(t<'b'>::char_), checker, pass) );
	static_assert(  exists_in(t<'b'>::char_ | t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_ | t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_ | t<'x'>::char_, checker, pass) );
	static_assert(  exists_in(t<'b'>::char_ - t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_ - t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_ - t<'x'>::char_, checker, pass) );
	static_assert(  exists_in(t<'c'>::char_ >> (t<'b'>::char_ - t<'a'>::char_), checker, pass) );
	static_assert(  exists_in(t<'c'>::char_ - (t<'b'>::char_ >> t<'a'>::char_), checker, pass) );
	static_assert( !exists_in(t<'c'>::char_ - (t<'b'>::char_ >> t<'a'>::char_), checker, stop) );
	static_assert(  exists_in(skip(t<'c'>::char_ >> t<'b'>::char_ >> t<'a'>::char_++), checker, pass) );
	static_assert( !exists_in(skip(t<'c'>::char_ >> t<'b'>::char_ >> t<'a'>::char_++), checker, stop) );
	return true;
}

} // namespace ascip_details
