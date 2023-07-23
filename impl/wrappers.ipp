#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<ascip_details::parser parser> struct negate_parser : base_parser<negate_parser<parser>> {
	parser p;
	constexpr auto operator!() const { return p; }
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), result);
		return ret * (-1);
	}
};

constexpr static bool test_negate() {
	static_assert( ({char r;(!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);}) == -1 );
	static_assert( ({char r;(!char_<'a'>).parse(make_test_ctx(), make_source('b'), r);}) ==  1 );
	static_assert( ({char r='z';(!char_<'a'>).parse(make_test_ctx(), make_source('b'), r);r;}) ==  'z' );

	static_assert( ({char r;(!!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);}) == 1 );
	static_assert( ({char r;(!!!!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);r;}) == 'a' );

	static_cast<const decltype(auto(char_<'a'>))&>(!!char_<'a'>);
	static_cast<const decltype(auto(char_<'a'>))&>(!!!!char_<'a'>);
	static_cast<const opt_parser<negate_parser<char_parser<'a'>>>&>(-(!char_<'a'>));
	static_cast<const opt_parser<char_parser<'a'>>&>(-(!!char_<'a'>));

	return true;
}

template<ascip_details::parser parser> struct opt_parser : base_parser<opt_parser<parser>> {
	[[no_unique_address]] parser p;
	//opt_parser(parser p) : p(p) {}
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return 0;
		auto ret = p.parse(ctx, src, result);
		return ret * (ret >= 0);
	}
};

constexpr static auto test_parser_char(const auto& p, auto&& s, auto pr) {
	char result='z';
	auto answer = p.parse(make_test_ctx(), make_source(s), result);
	return (answer==pr) * result;
}

constexpr static bool test_optional() {
	static_assert( test_parser_char(-char_<'a'>, "b", 0) == 'z' );
	static_assert( test_parser_char(-char_<'a'>, "a", 1) == 'a' );
	return true;
}

template<ascip_details::parser parser> struct omit_parser : base_parser<omit_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		return p.parse(ctx, src, r);
	}
};
constexpr static auto omit(auto&& p) { return omit_parser<decltype(auto(p))>{ p }; }
template<auto val> constexpr static const auto _char = omit(char_<val>);

constexpr static bool test_omit() {
	static_assert( test_parser_char(omit(char_<'a'>), "a", 1) == 'z' );
	static_assert( test_parser_char(omit(char_<'a'>), "b", -1) == 'z' );
	static_assert( test_parser_char(_char<'a'>, "a", 1) == 'z' );
	static_assert( test_parser_char(_char<'a'>, "b", -1) == 'z' );
	return true;
}

template<auto val, ascip_details::parser parser> struct tmpl_as_parser : base_parser<tmpl_as_parser<val,parser>> {
	[[no_unique_address]] parser p;
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};
template<typename value_t, ascip_details::parser parser> struct as_parser : base_parser<as_parser<value_t, parser>> {
	value_t val;
	[[no_unique_address]] parser p;
	constexpr auto parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};
template<typename value_t, ascip_details::parser parser_t> constexpr static auto as( parser_t&& p, value_t&& val ){
	return as_parser<decltype(auto(val)), decltype(auto(p))>{ p, val };
}
template<auto val, ascip_details::parser parser_t> constexpr static auto as( parser_t&& p) { return tmpl_as_parser<val, decltype(auto(p))>{ p }; }

constexpr static bool test_as() {
	static_assert( test_parser_char(as(char_<'a'>, 'b'), "a", 1) == 'b' );
	static_assert( test_parser_char(as(char_<'a'>, 'b'), "b", -1) == 'z' );
	static_assert( test_parser_char(as(int_, 'b'), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(int_), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(int_), "a", -1) == 'z' );
	return true;
}
