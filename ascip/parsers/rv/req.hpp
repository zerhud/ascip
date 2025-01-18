#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../base.hpp"
#include "../../details/values.hpp"

namespace ascip_details::prs {

struct rvariant_lreq_parser : base_parser<rvariant_lreq_parser> {
  constexpr static bool is_special_info_parser=true;
  constexpr parse_result parse(auto&& ctx, auto, auto& result) const {
    if constexpr (!requires{ is_parse_non_result(result).ok; }) {
      result = std::move(*search_in_ctx<rvariant_cpy_result_tag>(ctx));
      search_in_ctx<rvariant_cpy_result_tag>(ctx) = nullptr;
    }
    return 0;
  }
};

template<auto stop_ind>
struct rvariant_rreq_parser : base_parser<rvariant_rreq_parser<stop_ind>> {
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    if(!src) return 0;
    auto* var = *search_in_ctx<rvariant_stack_tag>(ctx);
    return var->call_parse(stop_ind+1, std::move(src), result);
  }
};

struct rvariant_rreq_pl_parser : base_parser<rvariant_rreq_pl_parser> {
  constexpr static parse_result parse(auto&&, auto, auto&) { return 0; }
};

template<auto ind> struct rvariant_req_parser : base_parser<rvariant_req_parser<ind>> {
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    auto* var = *search_in_ctx<ind, rvariant_stack_tag>(ctx);
    return var->call_parse(0, std::move(src), result);
  }
} ;

}
