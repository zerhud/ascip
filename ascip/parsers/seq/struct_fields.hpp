#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "common.hpp"
#include "../base.hpp"

namespace ascip_details::prs {

template<auto cnt> struct _seq_rfield_val { constexpr static auto num_val = cnt; };
template<ascip_details::parser parser, typename val> struct seq_inc_rfield_val : base_parser<seq_inc_rfield_val<parser, val>> {
	constexpr seq_inc_rfield_val() =default ;
	constexpr seq_inc_rfield_val(seq_inc_rfield_val&&) noexcept =default ;
	constexpr seq_inc_rfield_val(const seq_inc_rfield_val&) noexcept =default ;
	constexpr explicit seq_inc_rfield_val(parser p) : p(std::move(p)) {}
	parser p;

	constexpr static auto value = val::num_val;
};
template<ascip_details::parser parser, typename val> struct seq_num_rfield_val : base_parser<seq_num_rfield_val<parser, val>> {
	constexpr seq_num_rfield_val() =default ;
	constexpr seq_num_rfield_val(seq_num_rfield_val&&) noexcept =default ;
	constexpr seq_num_rfield_val(const seq_num_rfield_val&) noexcept =default ;
	constexpr explicit seq_num_rfield_val(parser p) : p(std::move(p)) {}
	parser p;

	constexpr static auto value = val::num_val;
};

struct seq_inc_rfield : base_parser<seq_inc_rfield> {constexpr parse_result parse(auto&&,auto,auto&)const {return 0;} };
template<ascip_details::parser parser> struct seq_inc_rfield_after : base_parser<seq_inc_rfield_after<parser>> {
	constexpr seq_inc_rfield_after() =default ;
	constexpr seq_inc_rfield_after(seq_inc_rfield_after&&) noexcept =default ;
	constexpr seq_inc_rfield_after(const seq_inc_rfield_after&) noexcept =default ;
	constexpr explicit seq_inc_rfield_after(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_inc_rfield_before : base_parser<seq_inc_rfield_before<parser>> {
	constexpr seq_inc_rfield_before() =default ;
	constexpr seq_inc_rfield_before(seq_inc_rfield_before&&) noexcept =default ;
	constexpr seq_inc_rfield_before(const seq_inc_rfield_before&) noexcept =default ;
	constexpr explicit seq_inc_rfield_before(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_dec_rfield_after : base_parser<seq_dec_rfield_after<parser>> {
	constexpr seq_dec_rfield_after() =default ;
	constexpr seq_dec_rfield_after(seq_dec_rfield_after&&) noexcept =default ;
	constexpr seq_dec_rfield_after(const seq_dec_rfield_after&) noexcept =default ;
	constexpr explicit seq_dec_rfield_after(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_dec_rfield_before : base_parser<seq_dec_rfield_before<parser>> {
	constexpr seq_dec_rfield_before() =default ;
	constexpr seq_dec_rfield_before(seq_dec_rfield_before&&) noexcept =default ;
	constexpr seq_dec_rfield_before(const seq_dec_rfield_before&) noexcept =default ;
	constexpr explicit seq_dec_rfield_before(parser p) : p(std::move(p)) {}
	parser p;
};
template<typename p> seq_inc_rfield_after(p) -> seq_inc_rfield_after<p>;
template<typename p> seq_inc_rfield_before(p) ->  seq_inc_rfield_before<p>;
template<typename p> seq_dec_rfield_after(p) ->  seq_dec_rfield_after<p>;
template<typename p> seq_dec_rfield_before(p) -> seq_dec_rfield_before<p>;

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename value, typename type>
constexpr static auto transform_special(prs::seq_inc_rfield_val<type, value>&& src, auto& ctx) {
	constexpr auto num = prs::seq_inc_rfield_val<type, value>::value;
	return transform_apply<mutator>(finc<num>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
template<typename mutator, parser ptype, typename value>
constexpr static auto transform_special(prs::seq_num_rfield_val<ptype, value>&& src, auto& ctx) {
	constexpr auto num = prs::seq_num_rfield_val<ptype, value>::value;
	return transform_apply<mutator>(fnum<num>(transform<mutator>(std::move(src.p), ctx)), ctx);
}

}