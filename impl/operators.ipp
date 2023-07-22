#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


constexpr auto operator|(const ascip_details::parser auto& p2) const {
	return variant_parser( static_cast<const parser&>(*this), p2 );
}
constexpr auto operator|(const ascip_details::nonparser auto& p2) const {
	return variant_parser( static_cast<const parser&>(*this), value_parser(p2) );
}
friend constexpr auto operator!(const parser& p) {
	return negate_parser<decltype(auto(p))>{ p };
}

/*
template<typename... left, ascip_details::parser right>
constexpr auto operator|(const variant_parser<left...>& p1, const right& p2) {
	return ascip_details::init_with_get<variant_parser<left..., right>>(p1.seq, p2); }
template<typename... left>
constexpr auto operator|(const variant_parser<left...>& p1, char p2) {
	return p1 | value_parser( p2 ); }
*/
