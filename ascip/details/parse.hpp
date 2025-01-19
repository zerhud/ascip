#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <ascip/parsers/base.hpp>

#include "values.hpp"
#include "context.hpp"

namespace ascip_details {

constexpr auto inject_skipping(auto&& to, auto&& what) ;

constexpr auto parse(auto&& parser, auto src) {
	type_parse_without_result r;
	return parse(std::forward<decltype(parser)>(parser), src, r);
}

constexpr auto parse(auto&& parser, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	parse_result nls = 1;
	return parser.parse(make_default_context(&nls), src, result);
}

constexpr auto parse(auto&& parser, const auto& skip, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	parse_result nls = 1;
	auto ctx = make_ctx<skip_parser_tag>( skip, make_default_context(&nls) );
	return inject_skipping(auto(parser), std::forward<decltype(skip)>(skip)).parse(ctx, src, result);
}

constexpr auto parse(auto&& parser, auto&& skip, auto src, auto& result, const auto& err) {
	using parser_type = std::decay_t<decltype(parser)>;
	parse_result nls = 1;
	auto ctx = make_ctx<skip_parser_tag>( skip, make_default_context(&nls, &err) );
	return inject_skipping(auto(parser), std::move(skip)).parse(ctx, src, result);
}

constexpr auto parse_with_ctx(const auto& ctx, auto&& parser, auto src, auto& result) {
	auto err = search_in_ctx<err_handler_tag>(ctx);
	auto skip = search_in_ctx<skip_parser_tag>(ctx);
	constexpr bool skip_found = !std::is_same_v<ctx_not_found_type, decltype(skip)>;
	constexpr bool err_found = !std::is_same_v<ctx_not_found_type, decltype(err)>;
	if constexpr (!skip_found && !err_found) return parse(std::forward<decltype(parser)>(parser), src, result);
	else if constexpr (skip_found && !err_found) return parse(std::forward<decltype(parser)>(parser), skip, src, result);
	else return parse(std::forward<decltype(parser)>(parser), skip, src, result, err);
}

}
