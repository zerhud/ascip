#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// operator() cannnot to be nob-member:
// https://en.cppreference.com/w/cpp/language/operators
// there will be a bug if the type_in_base will not match the final operator
constexpr auto operator()(auto act) const {
	return semact_parser<type_in_base, decltype(auto(act))>{ {},
		static_cast<decltype(act)&&>(act),
		static_cast<const type_in_base&>(*this)
	};
}

//NOTE: the clang has trouble for some reason with the following operators

constexpr auto operator!() const {
	return negate_parser<parser>{ static_cast<const parser&>(*this) };
}
template<ascip_details::parser right> constexpr auto operator%(const right& r) const {
	return binary_list_parser( static_cast<const parser&>(*this), r ); }
constexpr auto operator%(char r)const{ return binary_list_parser( static_cast<const parser&>(*this), value_parser{r} ); }

