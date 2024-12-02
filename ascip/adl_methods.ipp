#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


struct adl_tag {};
struct seq_tag {};

template<typename value_t, ascip_details::parser parser_t> constexpr static auto as( parser_t&& p, value_t&& val ){
	using ptype = std::decay_t<decltype(p)>;
	using valtype = std::decay_t<decltype(val)>;
	return typename ptype::holder::template as_parser<valtype, ptype>( std::forward<decltype(val)>(val), std::forward<decltype(p)>(p) );
}
template<auto val, ascip_details::parser parser_t> constexpr static auto as( parser_t&& p) {
	using type = std::decay_t<decltype(p)>;
	return typename type::holder::template tmpl_as_parser<val, type>{ std::forward<decltype(p)>(p) }; }
constexpr static auto omit(auto&& p) {
	using type = std::decay_t<decltype(p)>;
	return typename type::holder::template omit_parser<type>{ std::forward<decltype(p)>(p) }; }
template<typename result, parser type> constexpr auto check(type&& p) {
	return typename std::decay_t<type>::holder::template result_checker_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) }; }
template<typename result, parser type> constexpr auto cast(type&& p){
	return typename std::decay_t<type>::holder::template cast_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) }; }

template<parser type> constexpr auto use_seq_result(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return typename ptype::holder::template use_seq_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) }; }
template<auto cnt, parser type> constexpr auto finc(type&& p) {
	using p_type = std::decay_t<type>;
	using inc_type = typename std::decay_t<type>::holder::template _seq_rfield_val<cnt>;
	return typename std::decay_t<type>::holder::template seq_inc_rfield_val<p_type, inc_type>{ std::forward<decltype(p)>(p) }; }
template<auto cnt, parser type> constexpr auto fnum(type&& p) {
	using p_type = std::decay_t<type>;
	using num_type = typename std::decay_t<type>::holder::template _seq_rfield_val<cnt>;
	return typename std::decay_t<type>::holder::template seq_num_rfield_val<p_type, num_type>{ std::forward<decltype(p)>(p) }; }
template<string_literal msg, parser type> constexpr auto must(type&& p) {
	return typename std::decay_t<type>::holder::template seq_error_parser<msg, std::decay_t<type>>{ std::forward<decltype(p)>(p) }; }
template<parser type> constexpr auto lexeme(type&& p) {
	return typename std::decay_t<type>::holder::template lexeme_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) }; }
template<parser type> constexpr auto skip(type&& p) {
	return typename std::decay_t<type>::holder::template skip_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) }; }

template<parser type> constexpr auto use_variant_result(const type& p) {
	return typename decltype(auto(p))::holder::template use_variant_result_parser<type>{ {}, p }; }
template<parser type, parser... types> constexpr auto rv(auto&& maker, type&& first, types&&... list) {
	return [&maker]<auto... inds>(std::index_sequence<inds...>, auto&&... parsers) {
		return typename std::decay_t<type>::holder::rvariant_parser(
				std::move(maker),
				std::decay_t<type>::holder::template transform<
					typename std::decay_t<type>::holder::template rvariant_mutator<inds>
					>(std::move(parsers))...
				);
	}(
		  std::make_index_sequence<sizeof...(list)+1>{}
		, std::forward<decltype(first)>(first)
		, std::forward<decltype(list)>(list)...
	);
}
template<parser type> constexpr auto rv_result(type&& p) { 
	using ptype = std::decay_t<decltype(p)>;
	return typename ptype::holder::template rvariant_top_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) }; }

template<typename tag, typename value_type, parser type> constexpr auto add_to_ctx(value_type&& value, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return typename ptype::holder::template ctx_change_parser<type, tag, value_type>{ {}, std::forward<decltype(value)>(value), std::forward<decltype(p)>(p) }; }
template<typename tag, parser type> constexpr auto exec_before(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return typename ptype::holder::template exec_before_parser<type, tag, act_type>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) }; }
template<typename tag, parser type> constexpr auto exec_after(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return typename ptype::holder::template exec_after_parser<type, tag, act_type>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) }; }

// ===============================
//          parse part
// ===============================

constexpr auto parse(auto&& parser, auto src) {
	type_any_eq_allow r;
	return parse(std::forward<decltype(parser)>(parser), src, r);
}

constexpr auto parse(auto&& parser, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	return parser.parse(parser_type::holder::make_test_ctx(), src, result);
}

constexpr auto parse(auto&& parser, const auto& skip, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	auto ctx = parser_type::holder::make_test_ctx();
	return parser_type::holder::template
		inject_skipping(auto(parser), std::forward<decltype(skip)>(skip)).parse(ctx, src, result);
}

constexpr auto parse(auto&& parser, auto&& skip, auto src, auto& result, const auto& err) {
	using parser_type = std::decay_t<decltype(parser)>;
	auto ctx = parser_type::holder::make_test_ctx(&err);
	return parser_type::holder::template
		inject_skipping(auto(parser), std::move(skip)).parse(ctx, src, result);
}


// ===============================
//          operators part
// ===============================

template<parser left, typename right> constexpr auto operator>>(left&& l, right&& r)
	requires (!is_specialization_of<std::decay_t<left>, std::decay_t<left>::holder::template opt_seq_parser>)
{
	return typename std::decay_t<left>::holder::template opt_seq_parser<std::decay_t<left>, std::decay_t<right>>{ std::forward<left>(l), std::forward<right>(r) };
}
template<parser left> constexpr auto operator>>(left&& l, char r) {
	return std::forward<left>(l) >> typename std::decay_t<left>::holder::template value_parser<decltype(auto(r))>( r ); }
template<parser p> constexpr auto operator++(p&& l) {
	return typename std::decay_t<p>::holder::template seq_inc_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator++(p&& l,int) {
	return typename std::decay_t<p>::holder::template seq_inc_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l) {
	return typename std::decay_t<p>::holder::template seq_dec_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l,int) {
	return typename std::decay_t<p>::holder::template seq_dec_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator-(p&& _p) {
	return typename std::decay_t<p>::holder::template opt_parser<std::decay_t<p>>{ _p }; }
template<parser p> constexpr auto operator+(p&& _p) {
	return typename std::decay_t<p>::holder::template unary_list_parser<std::decay_t<p>>{ _p }; }
template<parser p> constexpr auto operator*(p&& _p) {
	return -( +(std::forward<decltype(_p)>(_p)) ); }

template<parser left, parser right> constexpr auto operator-(left&& l, right&& r) {
	return typename std::decay_t<left>::holder::different_parser( std::forward<decltype(l)>(l), std::forward<decltype(r)>(r)); }

constexpr auto operator|(ascip_details::variant_parser auto&& left, ascip_details::parser auto&& right) {
	return std::decay_t<decltype(left)>::mk(std::forward<decltype(left)>(left), std::forward<decltype(right)>(right)); }
constexpr auto operator|(auto&& left, ascip_details::parser auto&& right) {
	return typename std::decay_t<decltype(left)>::holder::variant_parser( std::forward<decltype(left)>(left), std::forward<decltype(right)>(right) ); }
constexpr auto operator|(auto&& left, ascip_details::nonparser auto&& right) {
	using holder = std::decay_t<decltype(left)>::holder;
	using left_type = std::decay_t<decltype(left)>;
	if constexpr (ascip_details::variant_parser<decltype(left)>) return std::forward<decltype(left)>(left).clang_crash_workaround(right);
	else return typename holder::template variant_parser<left_type>(std::forward<decltype(left)>(left)).clang_crash_workaround(right);
}
/*
#ifndef __clang__ // clang crashes here
constexpr auto operator|(auto&& left, ascip_details::nonparser auto&& right) {
	using holder = std::decay_t<decltype(left)>::holder;
	auto r = typename holder::value_parser(static_cast<decltype(right)&&>(right));
	return std::forward<decltype(left)>(left) | std::move(r);
}
#endif
*/
