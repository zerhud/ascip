#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<ascip_details::parser parser> struct negate_parser : base_parser<negate_parser<parser>> {
	parser p;
	constexpr negate_parser() =default ;
	constexpr negate_parser(negate_parser&&) =default ;
	constexpr negate_parser(const negate_parser&) =default ;
	constexpr negate_parser(parser p) : p(std::move(p)) {}
	constexpr auto operator!() const { return p; }
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), result);
		return ret * (-1);
	}
};

constexpr static bool test_negate() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
#ifndef __clang__
	static_assert( ({char r;(!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);}) == -1 );
	static_assert( ({char r;(!char_<'a'>).parse(make_test_ctx(), make_source('b'), r);}) ==  1 );
	static_assert( ({char r='z';(!char_<'a'>).parse(make_test_ctx(), make_source('b'), r);r;}) ==  'z' );

	static_assert( ({char r;(!!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);}) == 1 );
	static_assert( ({char r;(!!!!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);r;}) == 'a' );
#endif

	static_cast<const decltype(auto(char_<'a'>))&>(!!char_<'a'>);
	static_cast<const decltype(auto(char_<'a'>))&>(!!!!char_<'a'>);
	static_cast<const opt_parser<negate_parser<char_parser<'a'>>>&>(-(!char_<'a'>));
	static_cast<const opt_parser<char_parser<'a'>>&>(-(!!char_<'a'>));

	return true;
#pragma GCC diagnostic pop
}

template<ascip_details::parser parser> struct opt_parser : base_parser<opt_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr opt_parser(opt_parser&&) =default ;
	constexpr opt_parser(const opt_parser&) =default ;
	constexpr opt_parser() =default ;
	constexpr opt_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return 0;
		auto ret = p.parse(ctx, src, result);
		return ret * (ret >= 0);
	}
};

constexpr static bool test_optional() {
	static_assert( test_parser_char(-char_<'a'>, "b", 0) == 'z' );
	static_assert( test_parser_char(-char_<'a'>, "a", 1) == 'a' );
	return true;
}

template<ascip_details::parser parser> struct omit_parser : base_parser<omit_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr omit_parser() =default ;
	constexpr omit_parser(omit_parser&&) =default ;
	constexpr omit_parser(const omit_parser&) =default ;
	constexpr omit_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		return p.parse(ctx, src, r);
	}
};
#ifdef __clang__
template<typename t> negate_parser(t) -> negate_parser<t>;
template<typename t> omit_parser(t) -> omit_parser<t>;
template<typename t> opt_parser(t) -> opt_parser<t>;
#endif
template<auto val> constexpr static const auto _char = omit(char_<val>);
constexpr static const auto nl = _char<'\n'>;

constexpr static bool test_omit() {
	static_assert( test_parser_char(omit(char_<'a'>), "a", 1) == 'z' );
	static_assert( test_parser_char(omit(char_<'a'>), "b", -1) == 'z' );
	static_assert( test_parser_char(_char<'a'>, "a", 1) == 'z' );
	static_assert( test_parser_char(_char<'a'>, "b", -1) == 'z' );
	return true;
}

template<auto val, ascip_details::parser parser> struct tmpl_as_parser : base_parser<tmpl_as_parser<val,parser>> {
	[[no_unique_address]] parser p;
	constexpr tmpl_as_parser() =default ;
	constexpr tmpl_as_parser(tmpl_as_parser&&) =default ;
	constexpr tmpl_as_parser(const tmpl_as_parser&) =default ;
	constexpr tmpl_as_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};
template<typename value_t, ascip_details::parser parser> struct as_parser : base_parser<as_parser<value_t, parser>> {
	value_t val;
	[[no_unique_address]] parser p;
	constexpr as_parser() =default ;
	constexpr as_parser(as_parser&&) =default ;
	constexpr as_parser(const as_parser&) =default ;
	constexpr as_parser(value_t val, parser p) : val(std::move(val)), p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};
constexpr static bool test_as() {
	static_assert( test_parser_char(as(char_<'a'>, 'b'), "a", 1) == 'b' );
	static_assert( test_parser_char(as(char_<'a'>, 'b'), "b", -1) == 'z' );
	static_assert( test_parser_char(as(int_, 'b'), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(int_), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(int_), "a", -1) == 'z' );
	static_assert( []{
		auto p = char_<'a'>;
		return test_parser_char(as(p, 'b'), "a", 1);
	}() == 'b' );
	return true;
}

template<typename good_result, ascip_details::parser parser>
struct result_checker_parser : base_parser<result_checker_parser<good_result, parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert(
			   ascip_details::is_in_concept_check(decltype(auto(ctx)){})
			|| std::is_same_v<ascip_details::type_any_eq_allow, decltype(auto(result))>
			|| std::is_same_v<good_result, decltype(auto(result))>
			, "can only parser to required type" );
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(auto(src))&&>(src), result);
	}
};
template<typename needed, ascip_details::parser type> struct cast_parser : base_parser<cast_parser<needed,type>> {
	type p;
	constexpr static auto& check_result(auto& result) {
		if constexpr( std::is_same_v<std::decay_t<decltype(result)>, ascip_details::type_any_eq_allow> ) return result;
		else {
			static_assert(requires{ static_cast<needed&>(result); }, "the result must to be castable to needed type" );
			return static_cast<needed&>(result);
		}
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr ( ascip_details::is_in_concept_check(decltype(auto(ctx)){}) ) return 0;
		else return p.parse(static_cast<decltype(ctx)&&>(ctx), src, check_result(result));
	}
};
constexpr static bool test_checkers() {
	//NOTE: for c++23 we can only test if it works and cannot test if it fails
	static_assert( requires(char& r) {
		check<char>(char_<'a'>).parse(make_test_ctx(),make_source('a'),r);
		});
	static_assert( requires(char& r) {
		cast<char>(char_<'a'>).parse(make_test_ctx(),make_source('a'),r);
		});
	static_assert( requires(ascip_details::type_any_eq_allow& r) {
		check<char>(char_<'a'>).parse(make_test_ctx(),make_source('a'),r);
		});
	static_assert( requires(ascip_details::type_any_eq_allow& r) {
		cast<char>(char_<'a'>).parse(make_test_ctx(),make_source('a'),r);
		});
	return true;
}

template<ascip_details::parser left, ascip_details::parser right> struct different_parser : base_parser<different_parser<left, right>> {
	left lp;
	right rp;
	constexpr different_parser( left l, right r ) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow fake_result;
		if(rp.parse(ctx, src, fake_result) >= 0) return -1;
		return lp.parse(ctx, src, result);
	}
};
template<ascip_details::parser left, ascip_details::parser right>
different_parser(left, right) -> different_parser<left, right>;
template<typename v, typename p> as_parser(v,p) -> as_parser<v,p>;

constexpr static bool test_different() {
#ifndef __clang__
	static_assert( ({char r='z';(*(any - char_<'a'>)).parse(make_test_ctx(), make_source("#$%a"), r);}) == 3, "different parser: stops on it's excluding parser" );
	static_assert( ({char r='z';(*(any - char_<'a'>)).parse(make_test_ctx(), make_source("#$%a"), r);r;}) == '%', "different parser: excluded left result as is" );
#endif
	return true;
}

