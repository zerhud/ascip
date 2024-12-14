#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details {

template<typename,auto...> struct seq_type {};
template<typename t> struct type_holder { using type = t; t operator+() const ; };
template<typename type, auto ind> struct tuple_value { type value; };
template<typename... types> struct inner_tuple {
	consteval static auto mk_storage_type() {
		return []<typename fucky_clang,auto... inds>(seq_type<fucky_clang,inds...>){
			struct storage : tuple_value<types, inds>... {};
			return type_holder<storage>{};
		}(
#if __has_builtin(__integer_pack)
			seq_type<unsigned, __integer_pack(sizeof...(types))...>{}
#else
			__make_integer_seq<seq_type, unsigned, sizeof...(types)>{}
#endif
		);
	}

	using storage_type = decltype(+mk_storage_type());
	storage_type storage;

	constexpr inner_tuple(auto&&... args) : storage(std::forward<decltype(args)>(args)...) {}

	template<auto ind> constexpr auto& get(inner_tuple& t) {
		return []<typename type>(tuple_value<type, ind>& v){return v.value;}(t.storage);
	}
};

template<typename type>
struct val_resetter {
	type* val;
	type old;
	constexpr val_resetter(type* val, type nv) : val(val), old(*val) {
		*val = nv;
	}
	constexpr ~val_resetter() {
		*val = old;
	}
};

template<typename tag, typename val, typename next_ctx> struct context {
	using tag_t = tag;
	using next_t = next_ctx;
	val v; next_ctx _next;
	constexpr bool has_next() const {return true; } constexpr next_ctx& next() {return _next;}
};
template<typename tag, typename val, typename next_ctx> struct context_ptr {
	using tag_t = tag;
	using next_t = next_ctx;
	val v; next_ctx* _next=nullptr;
	constexpr bool has_next() const {return _next!=nullptr; } constexpr next_ctx& next() {return *_next;}
};
template<typename tag, typename val> struct last_context { using tag_t = tag; val v; };
constexpr const struct  ctx_not_found_type {} ctx_not_found;

template<typename tag, typename value>
constexpr auto make_ctx(value&& val) {
	return last_context<tag, decltype(auto(val))>{ static_cast<value&&>(val) };
}
template<typename tag, typename value>
constexpr auto make_ctx(value&& val, auto&& ctx) {
	if constexpr (std::is_lvalue_reference_v<decltype(ctx)>)
		return context_ptr<tag, decltype(auto(val)), decltype(auto(ctx))>{
			static_cast<value&&>(val), &ctx };
	else
		return context<tag, decltype(auto(val)), decltype(auto(ctx))>{
			static_cast<value&&>(val), static_cast<decltype(ctx)&&>(ctx) };
}
template<typename tag>
constexpr bool exists_in_ctx(auto&& ctx) {
	using ctx_type = std::decay_t<std::remove_pointer_t<decltype(ctx)>>;
	if constexpr (std::is_same_v<typename ctx_type::tag_t, tag>) return true;
	else if constexpr (requires{ ctx.next(); }) {
		if(ctx.has_next()) return exists_in_ctx<tag>(ctx.next());
		return exists_in_ctx<tag>(typename decltype(auto(ctx))::next_t{});
	}
	else return false;
}
template<typename tag>
constexpr auto search_in_ctx_constexpr(auto&& ctx) {
	if constexpr (std::is_same_v<typename decltype(auto(ctx))::tag_t, tag>) return ctx.v;
	else if constexpr (requires{ ctx.next(); }) return search_in_ctx<tag>(typename decltype(auto(ctx))::next_t{});
	else return ctx_not_found;
}
template<typename tag>
constexpr auto& search_in_ctx(auto&& ctx) {
	if constexpr (std::is_same_v<typename decltype(auto(ctx))::tag_t, tag>) return ctx.v;
	else if constexpr (requires{ ctx.next(); }) return search_in_ctx<tag>(ctx.next());
	else return ctx_not_found;
}
template<auto ind, typename tag, auto cur=0>
constexpr auto& by_ind_from_ctx(auto&& ctx) {
	if constexpr (std::is_same_v<typename decltype(auto(ctx))::tag_t, tag>) {
		if constexpr (ind == cur) return ctx.v;
		else return by_ind_from_ctx<ind,tag,cur+1>(ctx.next());
	}
	else if constexpr (requires{ ctx.next(); }) return by_ind_from_ctx<ind,tag,cur>(ctx.next());
	else return ctx_not_found;
}
template<typename tag, auto ind=0>
constexpr auto crop_ctx(auto&& ctx) {
	auto* cropped = by_ind_from_ctx<ind, tag>(ctx);
	static_assert( !std::is_same_v<std::decay_t<decltype(*cropped)>, decltype(ctx_not_found)>, "crop frame not found" );
	return make_ctx<tag>(cropped, *cropped);
}

template<typename tag>
constexpr auto remove_from_ctx(auto&& ctx) {
	using cur_t = decltype(auto(ctx));
	constexpr const bool last = !requires{ ctx.next(); };
	constexpr const bool match = std::is_same_v<typename decltype(auto(ctx))::tag_t, tag>;
	if constexpr (match) {
		//NOTE: cannot yet remove the last one
		auto next_ctx = ctx.next();
		return make_ctx<typename decltype(auto(next_ctx))::tag_t>(next_ctx.v, next_ctx.next());
	}
	else {
		if constexpr (last) return make_ctx<cur_t::tag_t>(ctx.v);
		else return make_ctx<typename cur_t::tag_t>(ctx.v, ctx.next());
	}
}

#include "static_string.ipp"

/************
 * utils
 ************/

template<typename type, template<typename...>class tmpl> constexpr const bool is_specialization_of = false;
template<template<typename...>class type, typename... args> constexpr const bool is_specialization_of<type<args...>, type> = true;

struct type_any_eq_allow {
	template<typename type> constexpr auto& operator =(const type&){ return *this; }
	template<typename type> constexpr auto& operator+=(const type&){ return *this; }
	template<typename type> constexpr auto& operator-=(const type&){ return *this; }
	template<typename type> constexpr auto& operator/=(const type&){ return *this; }
	template<typename type> constexpr auto& operator*=(const type&){ return *this; }
};
struct type_result_for_parser_concept : type_any_eq_allow {using type_any_eq_allow::operator=;};
struct parser_concept_check_tag {};

template<typename type> concept vector = requires(const type& v){ v.size(); v[0]; v.back(); } && requires(type& v){ v.pop_back(); };
template<typename type> concept string = requires(type& s){ s += typename type::value_type{}; } && vector<type>;
template<typename type> concept string_view = requires(const type& s){ s.size(); s[0]; s.back(); };
template<typename type> concept empbackable = requires(type& r){ emplace_back(r); } || requires(type& r){ r.emplace_back(); };
template<typename type> concept parser = requires(type& p, type_result_for_parser_concept& r) {
	p.parse(make_test_ctx<1,2,3,4,5,6,7,8,' ','c','o','c','e','p','t',' ',1,2,3,4>(p), make_source(p), r) < 0; };
template<typename type> concept nonparser = !parser<type>;
template<typename type> concept optional = requires(type& p){ p.has_value(); *p; p.emplace(); };
template<typename type> concept variant_parser = parser<std::decay_t<type>> &&
		is_specialization_of<std::decay_t<type>, std::decay_t<type>::holder::template variant_parser>;

constexpr bool is_in_concept_check(auto&& ctx) {
	return exists_in_ctx<parser_concept_check_tag>(ctx);
}

struct in_req_flag{ };
struct err_handler_tag{};
struct new_line_count_tag{};
constexpr bool is_in_reqursion_check(auto&& ctx) {
	return exists_in_ctx<in_req_flag>(ctx);
}

constexpr void count_new_line(auto& ctx, auto sym, auto& r) {
	constexpr bool need_count_new_lines =
		   exists_in_ctx<new_line_count_tag>(decltype(auto(ctx)){})
		&& !std::is_same_v<std::decay_t<decltype(r)>, type_any_eq_allow>
		;
	if constexpr (need_count_new_lines)
		search_in_ctx<new_line_count_tag>(ctx) += (sym == '\n');
}

namespace { // tuple
	template<auto ind, typename t> struct tuple_base { t val;
		constexpr tuple_base() {}
		constexpr tuple_base(t&& val) : val(static_cast<t&&>(val)) {}
		template<auto i> constexpr auto& get() const requires (i==ind) { return val; }
		template<auto i> constexpr std::enable_if_t<i==ind, t>& _get() const { return val; }
	};
	template<auto cur, typename... list> struct tuple_recursion {};
	template<auto cur, typename t, typename... list> struct tuple_recursion<cur,t,list...> : tuple_base<cur, t>, tuple_recursion<cur+1,list...> {
		constexpr tuple_recursion() {}
		constexpr tuple_recursion(t&& cv, list&&... lv) : tuple_base<cur,t>(static_cast<t&&>(cv)),tuple_recursion<cur+1,list...>(static_cast<list&&>(lv)...) {}
		template<auto i> constexpr const auto& __get() const {
			if constexpr (i == cur) return tuple_base<cur, t>::val;
			else return tuple_recursion<cur+1,list...>::template __get<i>();
		}
		template<auto i> constexpr auto& __get() {
			if constexpr (i == cur) return tuple_base<cur, t>::val;
			else return tuple_recursion<cur+1,list...>::template __get<i>();
		}
	};
	template<typename... types> struct tuple :  tuple_recursion<0, types...> {
		constexpr tuple() : tuple_recursion<0, types...>() {}
		constexpr tuple(types&&... vals) : tuple_recursion<0, types...>(static_cast<types&&>(vals)...) {}
	};
	/*
	template<auto ind, auto cur, typename type, typename... types> constexpr const auto& get_impl(const auto& src) {
		if constexpr (ind==cur) return static_cast<const tuple_base<ind,type>*>(&src)->val;
		else return get_impl<ind, cur+1, types...>(src);
	}
	*/
	template<auto ind, typename... types> constexpr const auto& get(const tuple<types...>& src) {
		static_assert( ind < sizeof...(types), "tuple: out of range" );
		return src.template __get<ind>();
		//return src.template _get<ind>();
		//return src.template get<ind>();
		//return get_impl<ind, 0, types...>(src);
	}
	template<auto ind, typename... types> constexpr auto& get(tuple<types...>& src) {
		static_assert( ind < sizeof...(types), "tuple: out of range" );
		return src.template __get<ind>();
	}
} // namespace (anonymus) tuple
 
template<typename...> struct types_set{};
template<auto sz> constexpr auto arrsize(const auto(&)[sz]){ return sz; }
constexpr auto strlen(const auto* vec){ unsigned ret = 0; while(vec[ret])++ret; return ++ret; }

constexpr auto& pop_back(type_any_eq_allow& v){ return v; };
constexpr auto& emplace_back(type_any_eq_allow& v){ return v; };
constexpr void pop(auto& r) requires requires{ pop_back(r); } { pop_back(r); }
constexpr void pop(auto& r) requires requires{ r.pop_back(); } { r.pop_back(); }
constexpr void pop(auto& r) { }
template<typename type> constexpr auto& empback(type& r) {
	if constexpr(requires{ emplace_back(r); }) return emplace_back(r);
	else if constexpr(requires{ r.emplace_back(); }) return r.emplace_back();
	else if constexpr(string<type>) { r += typename decltype(auto(r))::value_type{}; return r.back(); }
	else return r;
}
inline constexpr auto& eq( auto& to, const auto& from) { return empback(to) = from; }

constexpr auto pos(const auto& src)
{
	//TODO: remove this function
	if constexpr ( requires{ position(src); } ) return position(src);
	else return src.ind;
}

template<class init_t, auto... inds, template<typename...>class src_t, typename... src_args_t, typename... args_t>
constexpr auto init_with_get(const src_t<src_args_t...>& src, args_t&&... args) {
	if constexpr (sizeof...(inds) == sizeof...(src_args_t)) return init_t{ get<inds>(src)..., static_cast<args_t&&>(args)... };
	else return init_with_get<init_t, inds..., sizeof...(inds)>(src, static_cast<args_t&&>(args)...);
}
template<class init_t, auto... inds, template<typename...>class src_t, typename... src_args_t, typename... args_t>
constexpr auto init_with_get_inv(const src_t<src_args_t...>& src, args_t&&... args) {
	if constexpr (sizeof...(inds) == sizeof...(src_args_t)) return init_t{ static_cast<args_t&&>(args)..., get<inds>(src)... };
	else return init_with_get_inv<init_t, inds..., sizeof...(inds)>(src, static_cast<args_t&&>(args)...);
}

template<auto cind> constexpr auto& variant_result(auto& result) {
	if constexpr (cind<0) return result;
	else if constexpr (requires{create<0>(result);}) return create<cind>(result);
	else if constexpr (requires{emplace<0>(result);}) return emplace<cind>(result);
	else if constexpr (requires{result.template emplace<0>();}) return result.template emplace<cind>();
	else return result;
}

#include "adl_methods.ipp"

} // namespace ascip_details


namespace ascip_reflection {
#include <utility.ipp>
#include <reflection/get_field_from_struct.ipp>
template<auto ind> constexpr auto& get(ascip_details::type_result_for_parser_concept& r){ return r; }
constexpr void test () {
	struct tf1{ char f1; };
	struct tf2{ char f1, f2; };
	static_assert( []{ tf1 t{ 'a' }; return get<0>(t); }() == 'a' );
	static_assert( []{ tf2 t{ 'a', 'b' }; return get<0>(t); }() == 'a' );
	static_assert( []{ tf2 t{ 'a', 'b' }; return get<1>(t); }() == 'b' );
}
} // namespace ascip_reflection
