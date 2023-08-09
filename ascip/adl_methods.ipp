#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


struct adl_tag {};
struct seq_tag {};

template<typename value_t, ascip_details::parser parser_t> constexpr static auto as( parser_t&& p, value_t&& val ){
	return typename decltype(auto(p))::holder::template as_parser<decltype(auto(val)), decltype(auto(p))>{ p, val };
}
template<auto val, ascip_details::parser parser_t> constexpr static auto as( parser_t&& p) {
	return typename decltype(auto(p))::holder::template tmpl_as_parser<val, decltype(auto(p))>{ {}, p }; }
constexpr static auto omit(auto&& p) {
	return typename decltype(auto(p))::holder::template omit_parser<decltype(auto(p))>{ p }; }
template<typename result, parser type> constexpr auto check(const type& p) {
	return typename decltype(auto(p))::holder::template result_checker_parser<result, decltype(auto(p))>{ {}, p }; }
template<typename result, parser type> constexpr auto cast(const type& p){
	return typename decltype(auto(p))::holder::template cast_parser<result, decltype(auto(p))>{ {}, p }; }

template<auto cnt, parser type> constexpr auto finc(const type& p) {
	using inc_type = decltype(auto(p))::holder::template _seq_inc_rfield_val<cnt,type>;
	return typename decltype(auto(p))::holder::template seq_inc_rfield_val<inc_type>{ inc_type{p} }; }
template<auto cnt, parser type> constexpr auto fnum(const type& p) {
	using num_type = decltype(auto(p))::holder::template _seq_num_rfield_val<cnt,type>;
	return typename decltype(auto(p))::holder::template seq_num_rfield_val<num_type>{ num_type{p} }; }
template<string_literal msg, parser type> constexpr auto must(const type& p) {
	return typename decltype(auto(p))::holder::template seq_error_parser<msg, type>{ p }; }
template<parser type> constexpr auto lexeme(const type& p) {
	return typename decltype(auto(p))::holder::template lexeme_parser<type>{ {}, p }; }
template<parser type> constexpr auto skip(const type& p) {
	return typename decltype(auto(p))::holder::template skip_parser<type>{ {}, p }; }

template<parser type> constexpr auto use_variant_result(const type& p) {
	return typename decltype(auto(p))::holder::template use_variant_result_parser<type>{ {}, p }; }


// ===============================
//          parse part
// ===============================

constexpr auto parse(const auto& parser, auto src) {
	type_any_eq_allow r;
	return parse(parser, src, r);
}

constexpr auto parse(const auto& parser, auto src, auto& result) {
	return parser.parse(decltype(auto(parser))::holder::make_test_ctx(), src, result);
}

constexpr auto parse(const auto& parser, const auto& skip, auto src, auto& result) {
	auto ctx = decltype(auto(parser))::holder::make_test_ctx();
	return decltype(auto(parser))::holder::template
		inject_skipping<true>(parser, skip).parse(ctx, src, result);
}

constexpr auto parse(const auto& parser, const auto& skip, auto src, auto& result, const auto& err) {
	auto ctx = decltype(auto(parser))::holder::make_test_ctx(&err);
	return decltype(auto(parser))::holder::template
		inject_skipping<true>(parser, skip).parse(ctx, src, result);
}


// ===============================
//          operators part
// ===============================

template<parser left, typename right> constexpr auto operator>>(const left& l, const right& r)
	requires (!is_specialization_of<left, decltype(auto(l))::holder::template opt_seq_parser>)
{
	return typename decltype(auto(l))::holder::template opt_seq_parser<left, right>{ l, r };
}
template<parser left> constexpr auto operator>>(const left& l, char r) {
	return l >> typename decltype(auto(l))::holder::template value_parser<decltype(auto(r))>( r ); }
template<parser p> constexpr auto operator++(const p& l) {
	return typename decltype(auto(l))::holder::template seq_inc_rfield_before<p>{ l }; }
template<parser p> constexpr auto operator++(const p& l,int) {
	return typename decltype(auto(l))::holder::template seq_inc_rfield_after<p>{ l }; }
template<parser p> constexpr auto operator--(const p& l) {
	return typename decltype(auto(l))::holder::template seq_dec_rfield_before<p>{ l }; }
template<parser p> constexpr auto operator--(const p& l,int) {
	return typename decltype(auto(l))::holder::template seq_dec_rfield_after<p>{ l }; }

