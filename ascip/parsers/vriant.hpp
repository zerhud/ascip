#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <utility>

#include "as.hpp"
#include "base.hpp"
#include "char.hpp"
#include "../details/hana.hpp"
#include "../details/context.hpp"

namespace ascip_details::prs {

struct variant_shift_tag {};
struct variant_stack_tag {};
struct variant_stack_result_tag{};

namespace variant_details {
template <typename source, typename result>
struct monomorphic {
	virtual ~monomorphic() = default ;
	virtual parse_result parse_mono_omit(source) =0;
	virtual parse_result parse_mono_check(source) =0;
	virtual parse_result parse_mono(source, result&) =0;

	constexpr parse_result call_parse(source src, auto& r) {
		if constexpr (requires { is_parsing_without_result(r).ok; }) return parse_mono_omit(src);
		else if constexpr (requires { is_checking(r).ok; }) return parse_mono_check(src);
		else return parse_mono(src, r);
	}
};

template<typename parser, typename context, typename source, typename result>
struct monomorphic_impl : monomorphic<source, result> {
  const parser* v;
  context ctx;

  constexpr monomorphic_impl(const parser* v, context ctx) : v(v), ctx(std::move(ctx)) {}
  constexpr parse_result parse_mono_omit(source src) override {
    type_parse_without_result fr;
    return v->template parse_ind<0>(ctx, src, fr);
  }
  constexpr parse_result parse_mono_check(source src) override {
    type_check_parser fr;
    return v->template parse_ind<0>(ctx, src, fr);
  }
  constexpr parse_result parse_mono(source src, result& r) override {
    return v->template parse_ind<0>(ctx, src, r);
  }
};

template<typename parser, typename context, typename source, typename result>
constexpr auto mk_mono(const parser* p, context ctx, source src, result& r) {
  return monomorphic_impl<parser, context, source, result>( p, ctx );
}

}

template<parser parser> struct use_variant_result_parser : base_parser<use_variant_result_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		return p.parse(ctx, src, *search_in_ctx<variant_stack_result_tag>(ctx));
	}
};

template<auto ind> struct variant_shift_parser : base_parser<variant_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
		eq(result, *search_in_ctx<ind, variant_shift_tag>(ctx));
		return 0;
	}
};

template<auto ind> struct variant_recursion_parser : base_parser<variant_recursion_parser<ind>> {
	constexpr static parse_result parse(auto&& ctx, auto src, auto& result) {
		auto* var = *search_in_ctx<ind, variant_stack_tag>(ctx);
		return var->call_parse(src, result);
	}
};

template<parser... parsers> struct variant_parser : base_parser<variant_parser<parsers...>> {
	using self_type = variant_parser<parsers...>;
	tuple<parsers...> seq;

	constexpr variant_parser(const variant_parser& other) : seq(other.seq) {}
	constexpr variant_parser(variant_parser&& other) : seq(std::move(other.seq)) {}
	constexpr explicit variant_parser( parsers... l ) : seq( std::forward<parsers>(l)... ) {}

	template<auto ind, auto cnt, auto cur, typename cur_parser, typename... tail>
	constexpr static auto _cur_ind() {
		constexpr bool skip = is_specialization_of<cur_parser, use_variant_result_parser>;
		if constexpr (ind == cnt) {
			if constexpr (skip) return -1;
			else return cur;
		}
		else return _cur_ind<ind,cnt+1,cur+(!skip),tail...>();
	}
	template<auto ind> consteval static auto cur_ind() { return _cur_ind<ind,0,0,parsers...>(); }
	template<auto ind> constexpr auto parse_ind(auto&& ctx, auto& src, auto& result) const {
		auto prs = [&](auto&& r) {
			auto ret = get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(r));
			*search_in_ctx<variant_shift_tag>(ctx) = ret * (0<=ret);
			return ret;
		};
		if constexpr (ind+1 == sizeof...(parsers)) return prs(result);
		else {
			auto parse_result = prs(type_check_parser{});
			if(parse_result >= 0) return prs(result);
			return parse_ind<ind+1>(ctx, src, result);
		}
	}

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		using mono_type = variant_details::monomorphic<decltype(src), std::decay_t<decltype(result)>>;
		mono_type* mono_ptr;
		parse_result shift_storage=0;
		auto nctx = make_ctx<variant_shift_tag, any_shift_tag>(&shift_storage,
			make_ctx<variant_stack_tag>(&mono_ptr,
				make_ctx<variant_stack_result_tag>(&result, ctx)));
		auto mono = variant_details::mk_mono(this, nctx, src, result);
		mono_ptr = &mono;
		return mono.parse_mono(src, result);
	}

	constexpr auto clang_crash_workaround(auto r) {
		return std::move(*this) | value_parser(r);
	}
};

template<parser... parsers>
variant_parser(parsers...) -> variant_parser<parsers...>;


template<typename prs, template<auto>class t=prs::template tmpl>
constexpr static bool test_variant() {
	constexpr auto run_parse = [](const auto& p, auto&& src, auto& r) {
		return p.parse(make_test_ctx(), make_source(src), r);
	};
	static_assert( ((void)static_cast<const variant_parser<char_parser<'a'>, char_parser<'b'>, char_parser<'c'>>&>(t<'a'>::char_ | t<'b'>::char_ | t<'c'>::char_),1) );
#ifndef __clang__
	static_assert( ({ char r{};run_parse(t<'a'>::char_|t<'b'>::char_, "a", r);r;}) == 'a' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b', "b", r);r;}) == 'b' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "c", r);}) == 1 );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "c", r);r;}) == 'c' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "d", r);}) == -1 );

	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "b", r);r;}) == 'c' );
	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "a", r);r;}) == 'a' );
	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "a", r);  }) ==  1 );
#endif
	static_assert( [&] {
		char r='z';
		const auto pr = run_parse(prs::nop | t<'a'>::char_, "b", r);
		return (r=='z') + 2*(pr == 0);
	}() == 3 );

	static_assert( [&] {
		char r{'z'};
		const auto pr = run_parse(t<'a'>::char_ | t<'b'>::char_ >> t<0>::v_rec, "bbba", r);
		return (pr==4) + 2*(r=='a');
	}() == 3 );

	struct req_result{ char s{}; parse_result shift{}; };
	static_assert( [&] {
		req_result r;
		const auto pr = run_parse(prs::nop++ >> --t<'a'>::char_ | t<'b'>::char_++ >> use_variant_result(t<0>::v_rec) >> t<0>::variant_shift, "bbba", r);
		return (pr==4) + 2*(r.s=='a') + 4*(r.shift == 3);
	}() == 7 );

	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename... parsers>
constexpr auto operator|(prs::variant_parser<parsers...>&& left, parser auto&& right) {
  return [&]<auto... inds>(std::index_sequence<inds...>) {
   return prs::variant_parser<parsers..., std::decay_t<decltype(right)>>{ get<inds>(left.seq)..., std::forward<decltype(right)>(right) };
  }(std::make_index_sequence<sizeof...(parsers)>{});
}
constexpr auto operator|(auto&& left, parser auto&& right) {
  return prs::variant_parser( std::forward<decltype(left)>(left), std::forward<decltype(right)>(right) );
}
constexpr auto operator|(auto&& left, nonparser auto&& right) {
  using left_type = std::decay_t<decltype(left)>;
  constexpr bool is_left_variant = is_specialization_of<std::decay_t<decltype(left)>, prs::variant_parser>;
  if constexpr (is_left_variant) return std::forward<decltype(left)>(left).clang_crash_workaround(right);
  else return prs::variant_parser<left_type>(std::forward<decltype(left)>(left)).clang_crash_workaround(right);
}

template<parser type> constexpr auto use_variant_result(const type& p) {
  return prs::use_variant_result_parser<type>{ {}, p };
}

}
