#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "common.hpp"

namespace ascip_details::prs::rv_utils {

template<typename source, typename result> struct monomorphic {
  virtual ~monomorphic() =default ;
  virtual parse_result parse_mono_omit(int ind, source src) const =0 ;
  virtual parse_result parse_mono_check(int ind, source src) const =0 ;
  virtual parse_result parse_mono(int ind, source src, result& r) const =0 ;

  constexpr parse_result call_parse(int ind, source src, auto& r) const {
    if constexpr(requires{ is_parsing_without_result(r).ok; })
      return parse_mono_omit(ind, std::move(src));
    else if constexpr(requires{ is_checking(r).ok; })
      return parse_mono_check(ind, std::move(src));
    else
      return parse_mono(ind, std::move(src), r);
  }
};

template<auto parsers_count, typename parser, typename context, typename source, typename result>
struct mono_for_rv final : monomorphic<source, result> {
	using base_type = monomorphic<source, result>;
	const parser* self;
	mutable context ctx;
	constexpr mono_for_rv(const parser* self, context ctx) : self(self), ctx(std::move(ctx)) {}
	template<auto ind> constexpr parse_result call(source src, auto& r) const {
		return self->template parse_without_prep<ind>(ctx, src, r);
	}
	template<auto cur> constexpr parse_result call(const int ind, source src, auto& r) const {
		if constexpr(cur==parsers_count) return -1;
		else return cur==ind ? call<cur>(std::move(src), r) : call<cur+1>(ind, std::move(src), r);
	}
	constexpr parse_result parse_mono_omit(const int ind, source src) const override {
		type_parse_without_result r;
		return call<0>(ind, std::move(src), r);
	}
	constexpr parse_result parse_mono_check(const int ind, source src) const override {
		type_check_parser r;
		return call<0>(ind, std::move(src), r);
	}
	constexpr parse_result parse_mono(const int ind, source src, result& r) const override {
		return call<0>(ind, std::move(src), r);
	}
};

constexpr auto mk_mono(const auto* parser, auto ctx, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
	using parser_t = std::decay_t<decltype(*parser)>;
	using result_t = std::decay_t<decltype(result)>;
	return mono_for_rv<parser_t::size(), parser_t, decltype(ctx), decltype(src), result_t>( parser, ctx );
}

}