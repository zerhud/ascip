#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#include "base.hpp"
#include "../details/context.hpp"

namespace ascip_details::prs {

template<typename tag, parser parser> struct use_result_parser : base_parser<use_result_parser<tag, parser>> {
	parser p;
	constexpr static auto use_result_parser_tag = type_c<tag>;
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		auto& result = *search_in_ctx<tag>(ctx);
		return p.parse(ctx, src, result);
	}
};

template<typename tag, typename type, template<typename...>class tmpl> constexpr static auto exists_use_result() {
	return exists_in((std::decay_t<type>*)nullptr, [](const auto* p) {
		//TODO: clang issue, it should to be requires{ p->use_result_parser_tag; requires  p->use_result_parser_tag == type_c<tag>;}
		if constexpr(requires{p->use_result_parser_tag;}) return p->use_result_parser_tag == type_c<tag>;
		else return false;
	}, is_spec_checker<tmpl>);
}

}