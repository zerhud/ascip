#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

constexpr auto operator()(auto act) const {
	return semact_parser<type_in_base, decltype(auto(act))>{ {},
		static_cast<decltype(act)&&>(act),
		static_cast<const type_in_base&>(*this)
	};
}

constexpr auto operator|(const ascip_details::parser auto& p2) const {
	return variant_parser( static_cast<const parser&>(*this), p2 );
}
constexpr auto operator|(const ascip_details::nonparser auto& p2) const {
	return variant_parser( static_cast<const parser&>(*this), value_parser(p2) );
}
constexpr auto operator!() const {
	return negate_parser<parser>{ static_cast<const parser&>(*this) };
}
constexpr auto operator-()const{ return opt_parser<parser>{{}, static_cast<const parser&>(*this)}; }
constexpr auto operator+()const{ return unary_list_parser<parser>{ {}, static_cast<const parser&>(*this) }; }
constexpr auto operator*()const{ return -( +(static_cast<const parser&>(*this)) ); }

template<ascip_details::parser right> constexpr auto operator%(const right& r) const {
	return binary_list_parser( static_cast<const parser&>(*this), r ); }
constexpr auto operator%(char r)const{ return binary_list_parser( static_cast<const parser&>(*this), value_parser{r} ); }

template<ascip_details::parser right> constexpr auto operator-(const right& r)const{
	return different_parser( static_cast<const parser&>(*this), r ); }

