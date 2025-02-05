#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "base.hpp"
#include "../details/hana.hpp"

#include "rv/common.hpp"
#include "rv/monomorphic.hpp"
#include "rv/rec.hpp"
#include "rv/result.hpp"
#include "rv/rv.hpp"
#include "rv/parser.hpp"
#include "rv/transform.hpp"

namespace ascip_details::prs {



template<typename p, template<auto>class t=p::template tmpl>
constexpr auto test_rvariant_simple(auto r, auto&& src, auto&&... parsers) {
	auto var = rv([](auto& r){return &r;}, parsers...);
	var.parse(make_test_ctx(), make_source(src), r);
	static_assert( var.template is_term<0>() );
	static_assert( var.template is_term<1>() );
	auto var_with_skip = inject_skipping(var, +p::space);
	static_assert( var_with_skip.template is_term<0>() );
	static_assert( var_with_skip.template is_term<1>() );
	return r;
}

template<typename p>
constexpr bool test_rvariant() {
	static_assert( test_rvariant_simple<p>(int{}, "123", p::int_, p::fp) == 123 );
	static_assert( test_rvariant_simple<p>(double{}, "1.2", p::int_, p::fp) == 1.2 );
	return true;
}

} // namespace ascip_details::prs
