#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "common.hpp"
#include "../base.hpp"

namespace ascip_details::prs::seq_details {

template<typename source, typename result> struct monomorphic {
  virtual ~monomorphic() =default ;
  virtual parse_result parse_mono(source src, result& r) const =0 ;
};

template<typename parser, typename context, typename source, typename result>
struct mono_for_seq final : monomorphic<source, result> {
	using base_type = monomorphic<source, result>;
	const parser* self;
	mutable context ctx;
	constexpr mono_for_seq(const parser* self, context ctx) : self(self), ctx(std::move(ctx)) {}
	constexpr parse_result parse_mono(source src, result& r) const override {
		return self->parse_without_prep(ctx, src, r);
	}
};

constexpr auto mk_mono(const auto* parser, auto ctx, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
	return mono_for_seq<std::decay_t<decltype(*parser)>, decltype(ctx), decltype(src), std::decay_t<decltype(result)>>( parser, ctx );
}

}