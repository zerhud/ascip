#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include "concepts.hpp"

namespace ascip_details {

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
template<typename type> constexpr auto& empback(type& r) {
	if constexpr(requires{ emplace_back(r); }) return emplace_back(r);
	else if constexpr(requires{ r.emplace_back(); }) return r.emplace_back();
	else if constexpr(string<type>) { r += typename decltype(auto(r))::value_type{}; return r.back(); }
	else return r;
}
constexpr void eq( auto& to, const auto& from) { empback(to) = from; }
constexpr void eq(bool ok, auto& to, const auto& from) {
  if constexpr (std::is_integral_v<std::decay_t<decltype(to)>>) to = from*ok + to*!ok;
	else (void)(ok && (eq(to, from),1));
}

template<auto cind> constexpr auto& variant_result(auto& result) {
	if constexpr (cind<0) return result;
	else if constexpr (requires{create<0>(result);}) return create<cind>(result);
	else if constexpr (requires{emplace<0>(result);}) return emplace<cind>(result);
	else if constexpr (requires{result.template emplace<0>();}) return result.template emplace<cind>();
	else return result;
}

}
