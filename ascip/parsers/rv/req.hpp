#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../base.hpp"
#include "../../details/values.hpp"

namespace ascip_details::prs {

struct rvariant_lreq_parser : base_parser<rvariant_lreq_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr bool need_in_result = !std::is_same_v<decltype(result), type_any_eq_allow&> ;
		if constexpr (need_in_result) {
			result = std::move(*search_in_ctx<rvariant_copied_result_tag>(ctx));
			search_in_ctx<rvariant_copied_result_tag>(ctx) = nullptr;
		}
		return 0;
	}
};

template<auto stop_ind>
struct rvariant_rreq_parser : base_parser<rvariant_rreq_parser<stop_ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return 0;
        auto* var = *search_in_ctx<rvariant_stack_tag2>(ctx);
        if constexpr(type_dc<decltype(result)> == type_c<type_any_eq_allow>)
            return var->parse_mono(stop_ind+1, std::move(src));
        else return var->parse_mono(stop_ind+1, std::move(src), result);
	}
};

struct rvariant_rreq_pl_parser : base_parser<rvariant_rreq_pl_parser> {
	constexpr static parse_result parse(auto&&, auto, auto&) { return 0; }
};

template<auto ind> struct rvariant_req_parser : base_parser<rvariant_req_parser<ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
        auto* var = *search_in_ctx<rvariant_stack_tag2, ind>(ctx);
        if constexpr(type_dc<decltype(result)> == type_c<type_any_eq_allow>)
            return var->parse_mono(0, std::move(src));
        else return var->parse_mono(0, std::move(src), result);
	}
} ;

}