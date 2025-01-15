#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ascip/details.hpp"
#include "ascip/parsers.hpp"

struct ascip {
  using parse_result = ascip_details::parse_result;
  template<typename t> using base_parser = ascip_details::prs::base_parser<t>;

  static constexpr auto nop = ascip_details::prs::nop_parser{};
  template<auto s> static constexpr auto char_ = ascip_details::prs::char_parser<s>{};
  template<auto val> constexpr static auto _char = omit(char_<val>);
  template<ascip_details::string_literal s> static constexpr auto lit = ascip_details::prs::literal_parser<s>{};
  static constexpr auto space = ascip_details::prs::space_parser{};
  static constexpr auto any = ascip_details::prs::any_parser{};
  constexpr static auto nl = _char<'\n'>;

  static constexpr auto int_ = ascip_details::prs::int_parser{};
  template<auto base=10> constexpr static auto uint_ = ascip_details::prs::uint_parser<base>{};
  static constexpr auto fp = ascip_details::prs::float_point_parser{};

  // ranges
  constexpr static auto lower = ascip_details::prs::range_parser<'a','z'>{};
  constexpr static auto upper = ascip_details::prs::range_parser<'A','Z'>{};
  constexpr static auto digit = ascip_details::prs::range_parser<'0','9'>{};
  constexpr static auto d10 = ascip_details::prs::range_parser<'0', '9'>{};
  constexpr static auto ascii = ascip_details::prs::range_parser<(char)0x01,(char)0x7F>{};
  constexpr static auto alpha = lower | upper;

  // sequence
  constexpr static auto cur_pos = ascip_details::prs::cur_pos_parser{};
  constexpr static auto cur_shift = ascip_details::prs::cur_shift_parser{};
  template<auto ind> constexpr static auto req = ascip_details::prs::seq_recursion_parser<ind>{};
  constexpr static auto seq_enable_recursion = ascip_details::prs::seq_enable_recursion_parser{};
  constexpr static ascip_details::prs::seq_inc_rfield sfs{} ;

  // functions
  constexpr static auto make_test_ctx(auto&&... args) { return ascip_details::make_test_ctx(std::forward<decltype(args)>(args)...); }
  constexpr static auto make_source(auto&& src) { return ascip_details::make_source(std::forward<decltype(src)>(src)); }
  constexpr static auto inject_skipping(auto&& p, auto&& s) { return ascip_details::inject_skipping(std::forward<decltype(p)>(p), std::forward<decltype(s)>(s) ); }
  constexpr static auto fwd(auto& o) { return ascip_details::fwd( o ); }
  constexpr static void write_out_error_msg(auto& os, auto fn, auto msg, auto expt, auto src, auto ln ) {
    ascip_details::write_out_error_msg(os, std::move(fn), std::move(msg), std::move(expt), std::move(src), std::move(ln));
  }

  // rvariant
  constexpr static ascip_details::prs::rvariant_lreq_parser rv_lreq{};
  constexpr static ascip_details::prs::rvariant_rreq_pl_parser rv_rreq{};
  template<auto stop_ind> constexpr static ascip_details::prs::rvariant_rreq_parser<stop_ind> _rv_rreq{};
  template<auto ind> constexpr static auto rv_req = ascip_details::prs::rvariant_req_parser<ind>{};

  template<auto ind> constexpr static auto r_req  = ascip_details::prs::variant_recursion_parser<ind>{};

  constexpr static auto dquoted_string = lexeme(_char<'"'> >> *(as<'"'>(char_<'\\'> >> char_<'"'>)| (ascip::any - char_<'"'>)) >> _char<'"'>);
  constexpr static auto squoted_string = lexeme(_char<'\''> >> *(as<'\''>(char_<'\\'> >> char_<'\''>)| (ascip::any - char_<'\''>)) >> _char<'\''>);
  constexpr static auto quoted_string = lexeme(squoted_string | dquoted_string);

  // helpers
  template<auto s> struct tmpl {
    constexpr static auto& nop = ascip::nop;
    constexpr static auto& char_ = ascip::char_<s>;
    constexpr static auto& _char = ascip::_char<s>;
    constexpr static auto& space = ascip::space;
    constexpr static auto& any = ascip::any;
    constexpr static auto& nl = ascip::nl;
    constexpr static auto& req = ascip::req<s>;
    constexpr static auto& _rv_rreq = ascip::_rv_rreq<s>;
    constexpr static auto& rv_req = ascip::rv_req<s>;
    constexpr static auto& r_req = ascip::r_req<s>;
    constexpr static auto& uint_ = ascip::uint_<s>;
  };
};

template<typename parser> constexpr auto ascip_details::prs::base_parser<parser>::operator()(auto act) const {
  return semact_parser<parser, decltype(auto(act))>{ {},
    static_cast<decltype(act)&&>(act),
    static_cast<const parser&>(*this)
  };
}
