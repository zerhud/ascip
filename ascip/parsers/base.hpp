#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>

namespace ascip_details {

struct adl_tag {};
struct seq_tag {};
using parse_result = decltype(-1);

namespace prs { template<typename parser> struct base_parser; }

template<typename type> concept parser = std::is_base_of_v<prs::base_parser<std::decay_t<type>>, std::decay_t<type>> ;
template<typename type> concept nonparser = !parser<type>;

} // namespace ascip_details

namespace ascip_details::prs {

template<typename parser> struct base_parser : adl_tag {
	using type_in_base = parser;

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert( requires(const parser& p){ p.p; }, "child parser should define own parse method or have p field" );
		return static_cast<const parser&>(*this).p.parse(static_cast<decltype(ctx)&&>(ctx), std::move(src), result);
	}

	//NOTE: to have access to child classes we must to implement body after the classes will be defined
	constexpr auto operator()(auto act) const ;
};

} // namespace ascip_details::prs

namespace ascip_details {
} // namespace ascip_details


