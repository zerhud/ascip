#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "parser.hpp"

namespace ascip_details {

template<typename mutator, typename type, typename... parsers>
constexpr static auto transform(prs::rvariant_parser<type, parsers...>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>( transform_apply_to_each<mutator,prs::rvariant_parser,0>( std::move(src.seq),nctx,std::move(src.maker)), nctx );
}

}
