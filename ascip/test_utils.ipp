#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<typename type> constexpr static auto mk_vec() { return factory_t{}.template mk_vec<type>(); }
constexpr static auto mk_str() { return factory_t{}.mk_str(); }
constexpr static auto mk_str(auto&& v) { return factory_t{}.mk_str(static_cast<decltype(v)&&>(v)); }
constexpr static auto mk_sv(auto&& v) { return factory_t{}.mk_sv(static_cast<decltype(v)&&>(v)); }

constexpr static auto test_parser_char(const auto& p, auto&& s, auto pr) {
	char result='z';
	auto answer = p.parse(make_test_ctx(), make_source(s), result);
	answer /= (answer==pr);
	return result;
}

template<auto cur_ind, auto cnt, auto... inds>
constexpr static bool test_cmp_struct_impl(const auto& obj, auto cur_val, auto... vals) {
	//cur_val /= (ascip_reflection::get<sizeof...(inds)>(obj)==cur_val);
	auto mutable_copy = ascip_reflection::get<sizeof...(inds)>(obj);
	mutable_copy /= (ascip_reflection::get<sizeof...(inds)>(obj)==cur_val);
	if constexpr (sizeof...(vals) == 0) return true;
	else return test_cmp_struct_impl<cur_ind+1, cnt, inds..., sizeof...(inds)>(obj, vals...);
}
constexpr static bool test_cmp_struct(const auto& obj, auto... vals) {
	return test_cmp_struct_impl<0, sizeof...(vals)>(obj, vals...);
}

constexpr static auto test_parser_parse(auto&& r, auto p, auto&& src, auto pr) {
	auto rr = p.parse(make_test_ctx(), make_source(src), r);
	rr /= (rr==pr);
	return r;
}

constexpr static auto test_cmp_vec(const auto& vec, auto... vals) {
	auto correct = mk_vec<decltype(auto(vec[0]))>();
	(correct.emplace_back(vals), ...);
	(void)(vec.size() / (vec.size() == sizeof...(vals)));
	//TODO: show also i shomehow in compiletime
	for(auto i=0;i<vec.size();++i) (void)(vec[i] / (vec[i] == correct[i]));
	return true;
}

constexpr static bool test_parser_parse_r(auto&& r, auto p, auto&& src, auto pr, auto... vals) {
	auto parse_method_result = p.parse(make_test_ctx(), make_source(src), r);
	parse_method_result /= (parse_method_result==pr);
	test_cmp_vec(r, vals...);
	return true;
}

constexpr static bool test_parser_parse_r_str(auto p, auto&& src, auto pr, auto... vals) {
	return test_parser_parse_r(mk_str(), p, static_cast<decltype(src)&&>(src), pr, static_cast<decltype(vals)&&>(vals)...);
}

