#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include "definition_tags.hpp"

namespace ascip_details {

template<typename mutator>
constexpr auto transform_apply(auto&& src, auto& ctx) {
	if constexpr (requires{ mutator::template apply<std::decay_t<decltype(src)>>( std::forward<decltype(src)>(src), ctx ); }) {
		return mutator::template apply<std::decay_t<decltype(src)>>( std::forward<decltype(src)>(src), ctx );
	}
	else return src;
}

template<typename mutator, template<typename...>class result_t, auto... ind, template<typename...>class i_tuple, typename... tail>
constexpr auto transform_apply_to_each(i_tuple<tail...>&& src, auto& ctx, auto&&... args) {
	if constexpr (sizeof...(ind) == sizeof...(tail))
		return result_t<
			std::decay_t<decltype(args)>...,
			std::decay_t<decltype(transform<mutator>(std::move(get<ind>(src)), ctx))>...>
			(
			 std::forward<decltype(args)>(args)...,
			 transform<mutator>(std::move(get<ind>(src)), ctx)...
			);
	else return  transform_apply_to_each<mutator, result_t, ind..., sizeof...(ind)>(std::move(src), ctx, std::forward<decltype(args)>(args)...);
}

template<typename mutator>
constexpr auto transform(auto&& src) {
	auto ctx = mutator::create_ctx();
	return transform<mutator>(std::forward<decltype(src)>(src), ctx);
}

template<typename mutator>
constexpr auto transform(auto&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	if constexpr(requires{ transform_special<mutator>(std::move(src), ctx); })
		return transform_special<mutator>(std::move(src), nctx);
	else return transform_apply<mutator>(std::forward<decltype(src)>(src), nctx);
}
template<typename mutator, template<typename>class wrapper, typename inner>
constexpr auto transform(wrapper<inner>&& src, auto& ctx) requires (requires{ src.p; } && !requires{transform_special<mutator>(std::move(src), ctx);}){
	auto nctx = mutator::create_ctx(src, ctx);
	auto mp = transform<mutator>(std::move(src.p), nctx);
	if constexpr(requires{ wrapper{{}, std::move(mp)}; })
		return transform_apply<mutator>(wrapper{{}, std::move(mp)}, nctx);
	else
		return transform_apply<mutator>(wrapper{std::move(mp)}, nctx);
}
template<typename mutator, template<typename...>class semact_wrapper, typename parser, typename act_t, typename... tags>
constexpr auto transform_special(semact_wrapper<parser, act_t, tags...>&& src, auto& ctx) requires requires{ src.act; src.p; } {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( semact_wrapper<std::decay_t<decltype(np)>, std::decay_t<decltype(src.act)>, tags...>{ {}, std::move(src.act), std::move(np) }, nctx );
}

}