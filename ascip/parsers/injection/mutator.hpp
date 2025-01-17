#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "parsers.hpp"
#include "../rv.hpp"
#include "../seq.hpp"
#include "../vriant.hpp"
#include "../binary_list.hpp"
#include "../different.hpp"
#include "../../details/hana.hpp"

namespace ascip_details::prs {

template<typename skip_type>
struct injection_mutator {
	struct context{ };
	struct in_lexeme_context{ };
	struct in_lexeme_context_deep{ };
	template<typename ctx> constexpr static bool check_inside_lexeme() {
		using t = std::decay_t<ctx>;
		return std::is_same_v<t, in_lexeme_context> || std::is_same_v<t,  in_lexeme_context_deep>;
	}
	constexpr static auto create_ctx(){ return context{}; }
	constexpr static auto create_ctx(auto&& src, auto&& ctx) {
		if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, skip_parser>)
			return context{};
		else if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, lexeme_parser>) {
			if constexpr (check_inside_lexeme<decltype(ctx)>()) return in_lexeme_context_deep{};
			else return in_lexeme_context{ };
		}
		else return ctx;
	}

	template<typename type>
	constexpr static auto apply(auto&& p, auto& ctx) {
		constexpr const bool is_inside_lexeme = check_inside_lexeme<decltype(ctx)>();
		constexpr const bool is_inside_lexeme_deep = std::is_same_v<std::decay_t<decltype(ctx)>, in_lexeme_context_deep>;
		constexpr const bool is_parser_lexeme = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, lexeme_parser>;
		constexpr const bool is_parser_skip = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, skip_parser>;

		constexpr const bool is_parser_variant = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, variant_parser>;
		constexpr const bool is_parser_rvariant = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, rvariant_parser>;
		constexpr const bool is_parser_blist = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, binary_list_parser>;
		constexpr const bool is_parser_diff = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, different_parser>;
		constexpr const bool is_opt_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, opt_seq_parser>;
		constexpr const bool is_num_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, seq_num_rfield_val>;
		constexpr const bool is_inc_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, seq_inc_rfield_val>;
		constexpr const bool is_special_info_parser = requires{ p.is_special_info_parser; };

		constexpr const bool is_parser_for_skip =
			   is_opt_seq_parser
			|| is_parser_variant
			|| is_parser_rvariant
			|| is_parser_blist
			|| is_parser_diff
			|| is_num_seq_parser
			|| is_inc_seq_parser
			|| is_special_info_parser
			;

		if constexpr (is_parser_lexeme && is_inside_lexeme_deep) return p.p;
		else if constexpr (is_parser_lexeme)
			return injected_parser<skip_type, std::decay_t<decltype(p.p)>>( skip_type{}, std::move(p.p) );
		else if constexpr (is_inside_lexeme || is_parser_for_skip) return p;
		else if constexpr (is_parser_skip) return p.p;
		else if constexpr ( requires{ p.p; } ) return p;
		else return injected_parser<skip_type, std::decay_t<decltype(p)>>( skip_type{}, std::move(p) );
	}
};

}