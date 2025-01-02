#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include <string>
#include <vector>

#include "ascip.hpp"

using p = ascip<>;

template<auto s> constexpr auto& char_ = p::char_<s>;
template<auto s> constexpr auto& _char = p::_char<s>;
constexpr auto make_test_ctx(auto&&... args) { return p::make_test_ctx(std::forward<decltype(args)>(args)...); }
constexpr auto make_source(auto&& src) { return p::make_source(std::forward<decltype(src)>(src)); }

template<auto cur_ind, auto cnt, auto... inds>
constexpr bool test_cmp_struct_impl(const auto& obj, auto cur_val, auto... vals) {
	//cur_val /= (ascip_reflection::get<sizeof...(inds)>(obj)==cur_val);
	auto mutable_copy = ascip_reflection::get<sizeof...(inds)>(obj);
	mutable_copy /= (ascip_reflection::get<sizeof...(inds)>(obj)==cur_val);
	if constexpr (sizeof...(vals) == 0) return true;
	else return test_cmp_struct_impl<cur_ind+1, cnt, inds..., sizeof...(inds)>(obj, vals...);
}
constexpr bool test_cmp_struct(const auto& obj, auto... vals) {
	return test_cmp_struct_impl<0, sizeof...(vals)>(obj, vals...);
}

constexpr auto test_parser_parse(auto&& r, auto p, auto&& src, auto pr) {
	auto rr = p.parse(decltype(p)::holder::make_test_ctx(), decltype(p)::holder::make_source(src), r);
	rr /= (rr==pr);
	return r;
}

constexpr auto test_cmp_vec(const auto& vec, auto... vals) {
	auto correct = std::vector<decltype(auto(vec[0]))>();
	(correct.emplace_back(vals), ...);
	(void)(vec.size() / (vec.size() == sizeof...(vals)));
	//TODO: show also i shomehow in compiletime
	for(auto i=0;i<vec.size();++i) (void)(vec[i] / (vec[i] == correct[i]));
	return true;
}

constexpr bool test_parser_parse_r(auto&& r, auto p, auto&& src, auto pr, auto... vals) {
	auto parse_method_result = p.parse(p::make_test_ctx(), p::make_source(src), r);
	parse_method_result /= (parse_method_result==pr);
	test_cmp_vec(r, vals...);
	return true;
}

constexpr bool test_parser_parse_r_str(auto p, auto&& src, auto pr, auto... vals) {
	return test_parser_parse_r(std::string{}, p, static_cast<decltype(src)&&>(src), pr, static_cast<decltype(vals)&&>(vals)...);
}
