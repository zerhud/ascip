#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include "rec.hpp"
#include "parser.hpp"

namespace ascip_details::prs {

template<auto ind>
struct rvariant_mutator {
	struct context{};
	constexpr static auto create_ctx() { return context{}; }
	constexpr static auto create_ctx(auto&&,auto&&) { return context{}; }
	template<typename type>
	constexpr static auto apply(rvariant_rrec_pl_parser&&,auto&&) {
		return rvariant_rrec_parser<ind>{};
	}
};

template<parser type, parser... types> constexpr auto rv(auto&& maker, type&& first, types&&... list) {
	return [&maker]<auto... inds>(std::index_sequence<inds...>, auto&&... parsers) {
		return rvariant_parser(
				std::move(maker),
				transform<rvariant_mutator<inds>>(std::move(parsers))...
				);
	}(
			std::make_index_sequence<sizeof...(list)+1>{}
		, std::forward<decltype(first)>(first)
		, std::forward<decltype(list)>(list)...
	);
}

}
