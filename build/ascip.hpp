       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details {

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

constexpr void test_val_resetter() {
	static_assert( []{
		int val = 10;
		{
			val_resetter vr(&val, 20);
			val /= (val==20);
		}
		return val; }() == 10 );
}

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

constexpr void test_context() {
	struct v1_t{int v=1;}; struct v2_t{int v=2;}; struct v3_t{int v=4;}; struct v4_t{int v=8;};
	struct t1_t{}; struct t2_t{}; struct t3_t{}; struct t4_t{};
	static_assert( []{ constexpr v1_t v1;
		constexpr auto ctx = make_ctx<t1_t>(v1);
		return search_in_ctx<t1_t>(ctx).v;
	}()==1, "can find in single context" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t2_t>(v2, make_ctx<t1_t>(v1));
		return search_in_ctx<t1_t>(ctx2).v;
	}()==1, "can find in 2 deep context" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t2_t>(v2, make_ctx<t1_t>(v1));
		return search_in_ctx<t2_t>(ctx2).v;
	}()==2, "can find in 2 deep context" );
	static_assert( []{ v1_t v1; v3_t v3;
		auto ctx3 = make_ctx<t3_t>(v3, make_ctx<t1_t>(v1));
		return exists_in_ctx<t3_t>(ctx3) && exists_in_ctx<t1_t>(ctx3) && !exists_in_ctx<t2_t>(ctx3);
	}()==true, "exists_in_ctx by tag only" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t1_t>(v2, make_ctx<t1_t>(v1));
		return by_ind_from_ctx<0,t1_t>(ctx2).v;
	}()==2, "can find by ind" );
	static_assert( []{ v1_t v1; v2_t v2;
		auto ctx2 = make_ctx<t1_t>(v2, make_ctx<t1_t>(v1));
		return by_ind_from_ctx<1,t1_t>(ctx2).v;
	}()==1, "can find by ind" );
	static_assert(  exists_in_ctx<t1_t>(make_ctx<t1_t>(v1_t{})) );
	static_assert( !exists_in_ctx<t2_t>(make_ctx<t1_t>(v1_t{})) );
	(void)( static_cast<const decltype(ctx_not_found)&>(by_ind_from_ctx<0,t2_t>(make_ctx<t1_t>(v2_t{}, make_ctx<t1_t>(v1_t{})))) );
	static_assert( []{
		auto ctx1 = make_ctx<t1_t>(1, make_ctx<t1_t>(2, make_ctx<t2_t>(3)));
		auto ctx2 = remove_from_ctx<t1_t>(ctx1);
		return (search_in_ctx<t1_t>(ctx1)==1) + (2*(search_in_ctx<t1_t>(ctx2)==2)); }() == 3 );
}

       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<typename char_type, auto str_size>
struct string_literal {
	constexpr string_literal(const char_type (&str)[str_size]) {
		for(auto i=0;i<str_size;++i) value[i] = str[i];
	}

	char_type value[str_size];

	constexpr auto size() const { static_assert( 1 <= str_size ); return str_size-1; }
	constexpr auto operator[](auto i) const { return value[i]; }

	template<auto sz>
	constexpr bool operator==(const char_type(&r)[sz]) const {
		if constexpr (sz != str_size) return false;
		for(auto i=0;i<str_size;++i) if(value[i]!=r[i]) return false;
		return true;
	}
	template<typename char_t> requires (std::is_pointer_v<char_t>)
	constexpr bool operator==(char_t r) const {
		for(auto i=0;i<str_size;++i) {
			if(value[i]!=r[i]) return false;
		}
		return true;
	}
};
template<string_literal str> struct test_tmpl{ constexpr bool is_eq(const char* v) const {
	return str == v;
}
constexpr auto size() const { return str.size(); }
};

constexpr void test_static_string() {
	static_assert( string_literal("cstr")[0] == 'c' );
	static_assert( string_literal("cstr")[4] == 0x00 );
	static_assert( string_literal("cstr").value[4] == 0x00 );
	static_assert( string_literal("cstr") == "cstr" );
	static_assert( !(string_literal("cstr") == "cstr\0ups") );

	static_assert( string_literal("cstr").size() == 4 );

	static_assert( test_tmpl<"test">{}.is_eq("test") );
	static_assert( !test_tmpl<"test">{}.is_eq("test_ups") );
	static_assert( test_tmpl<"test">{}.is_eq("test\0ups") );
	static_assert( test_tmpl<"test">{}.size() == 4 );
	static_assert( test_tmpl<"is">{}.size() == 2 );
}

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


       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


struct adl_tag {};
struct seq_tag {};

template<typename value_t, ascip_details::parser parser_t> constexpr static auto as( parser_t&& p, value_t&& val ){
	using ptype = std::decay_t<decltype(p)>;
	using valtype = std::decay_t<decltype(val)>;
	return typename ptype::holder::template as_parser<valtype, ptype>( std::forward<decltype(val)>(val), std::forward<decltype(p)>(p) );
}
template<auto val, ascip_details::parser parser_t> constexpr static auto as( parser_t&& p) {
	using type = std::decay_t<decltype(p)>;
	return typename type::holder::template tmpl_as_parser<val, type>{ std::forward<decltype(p)>(p) }; }
constexpr static auto omit(auto&& p) {
	using type = std::decay_t<decltype(p)>;
	return typename type::holder::template omit_parser<type>{ std::forward<decltype(p)>(p) }; }
template<typename result, parser type> constexpr auto check(type&& p) {
	return typename std::decay_t<type>::holder::template result_checker_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) }; }
template<typename result, parser type> constexpr auto cast(type&& p){
	return typename std::decay_t<type>::holder::template cast_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) }; }

template<auto cnt, parser type> constexpr auto finc(type&& p) {
	using inc_type = typename std::decay_t<type>::holder::template _seq_inc_rfield_val<cnt,std::decay_t<type>>;
	return typename std::decay_t<type>::holder::template seq_inc_rfield_val<inc_type>{ inc_type{std::forward<decltype(p)>(p)} }; }
template<auto cnt, parser type> constexpr auto fnum(type&& p) {
	using num_type = typename std::decay_t<type>::holder::template _seq_num_rfield_val<cnt,std::decay_t<type>>;
	return typename std::decay_t<type>::holder::template seq_num_rfield_val<num_type>{ num_type{std::forward<decltype(p)>(p)} }; }
template<string_literal msg, parser type> constexpr auto must(type&& p) {
	return typename std::decay_t<type>::holder::template seq_error_parser<msg, std::decay_t<type>>{ std::forward<decltype(p)>(p) }; }
template<parser type> constexpr auto lexeme(type&& p) {
	return typename std::decay_t<type>::holder::template lexeme_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) }; }
template<parser type> constexpr auto skip(type&& p) {
	return typename std::decay_t<type>::holder::template skip_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) }; }

template<parser type> constexpr auto use_variant_result(const type& p) {
	return typename decltype(auto(p))::holder::template use_variant_result_parser<type>{ {}, p }; }
template<parser type, parser... types> constexpr auto rv(auto&& maker, type&& first, types&&... list) {
	return [&maker]<auto... inds>(std::index_sequence<inds...>, auto&&... parsers) {
		return typename std::decay_t<type>::holder::rvariant_parser(
				std::move(maker),
				std::decay_t<type>::holder::template transform<
					typename std::decay_t<type>::holder::template rvariant_mutator<inds>
					>(std::move(parsers))...
				);
	}(
		  std::make_index_sequence<sizeof...(list)+1>{}
		, std::forward<decltype(first)>(first)
		, std::forward<decltype(list)>(list)...
	);
}
template<parser type> constexpr auto rv_result(type&& p) { 
	using ptype = std::decay_t<decltype(p)>;
	return typename ptype::holder::template rvariant_top_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) }; }

// ===============================
//          parse part
// ===============================

constexpr auto parse(auto&& parser, auto src) {
	type_any_eq_allow r;
	return parse(std::forward<decltype(parser)>(parser), src, r);
}

constexpr auto parse(auto&& parser, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	return parser.parse(parser_type::holder::make_test_ctx(), src, result);
}

constexpr auto parse(auto&& parser, const auto& skip, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	auto ctx = parser_type::holder::make_test_ctx();
	return parser_type::holder::template
		inject_skipping(auto(parser), std::forward<decltype(skip)>(skip)).parse(ctx, src, result);
}

constexpr auto parse(auto&& parser, auto&& skip, auto src, auto& result, const auto& err) {
	using parser_type = std::decay_t<decltype(parser)>;
	auto ctx = parser_type::holder::make_test_ctx(&err);
	return parser_type::holder::template
		inject_skipping(auto(parser), std::move(skip)).parse(ctx, src, result);
}


// ===============================
//          operators part
// ===============================

template<parser left, typename right> constexpr auto operator>>(left&& l, right&& r)
	requires (!is_specialization_of<std::decay_t<left>, std::decay_t<left>::holder::template opt_seq_parser>)
{
	return typename std::decay_t<left>::holder::template opt_seq_parser<std::decay_t<left>, std::decay_t<right>>{ std::forward<left>(l), std::forward<right>(r) };
}
template<parser left> constexpr auto operator>>(left&& l, char r) {
	return std::forward<left>(l) >> typename std::decay_t<left>::holder::template value_parser<decltype(auto(r))>( r ); }
template<parser p> constexpr auto operator++(p&& l) {
	return typename std::decay_t<p>::holder::template seq_inc_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator++(p&& l,int) {
	return typename std::decay_t<p>::holder::template seq_inc_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l) {
	return typename std::decay_t<p>::holder::template seq_dec_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l,int) {
	return typename std::decay_t<p>::holder::template seq_dec_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }


} // namespace ascip_details


namespace ascip_reflection {
/* Copyright (C) 2024 Hudyaev Alexey
 * SPDX-License-Identifier:    MIT
 */

template<typename t>
struct _decay {
	template<typename type> static const type& lref() ;
	template<typename u> struct _t{ using type = u; };
	template<typename u> static _t<u> impl(const u&) ;
	template<typename u, unsigned int sz> static _t<u*> impl(const u(&)[sz]) ;
	template<typename r, typename... p> static _t<r(*)(p...)> impl(r(&)(p...)) ;
	using type = typename decltype(impl(lref<t>()))::type;
};

template<typename t> using decay_t = typename _decay<t>::type;

template<typename t, t v>
struct integral_constant {
	static constexpr t value = v;
	using value_type = t;
	using type = integral_constant;
	constexpr operator value_type() const noexcept { return value; }
	constexpr value_type operator()() const noexcept { return value; }
};
struct true_type : integral_constant<bool, true> {}; 
struct false_type : integral_constant<bool, false> {}; 
template<typename t> struct is_rvalue_reference : false_type {};
template<typename t> struct is_rvalue_reference<t&&> : true_type {};
template<typename t> struct is_lvalue_reference : false_type {};
template<typename t> struct is_lvalue_reference<t&> : true_type {};
template<typename t> struct is_const : false_type {};
template<typename t> struct is_const<const t> : true_type {};
template<typename t> inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<t>::value;
template<typename t> inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<t>::value;
template<typename t> inline constexpr bool is_const_v = is_const<t>::value;

template<class t> struct remove_reference { typedef t type; };
template<class t> struct remove_reference<t&> { typedef t type; };
template<class t> struct remove_reference<t&&> { typedef t type; };
template< class t > using remove_reference_t = typename remove_reference<t>::type;

template<class t, class u>
constexpr auto&& forward_like(u&& x) noexcept
{
	constexpr bool is_adding_const = is_const_v<remove_reference_t<t>>;
	if constexpr (is_rvalue_reference_v<t&&>) {
		if constexpr (is_adding_const) return static_cast<const decay_t<u>&&>(x);
		else return static_cast<decay_t<u>&&>(x);
	}
	else {
		if constexpr (is_adding_const) return static_cast<const u&>(x);
		else return static_cast<u&>(x);
	}
}

/* Copyright (C) 2024 Hudyaev Alexey
 * SPDX-License-Identifier:    MIT
 */

constexpr struct {
	template<typename t> operator t() const ;
} any ;

template<typename type, auto... args> constexpr auto size_impl() {
	if constexpr ( requires{ type{ args..., any }; })
		return size_impl<type, args..., any>();
	else return sizeof...(args);
}

template<typename type> constexpr auto size_impl_dispatcher() {
	if constexpr (requires{type::struct_fields_count();}) return type::struct_fields_count();
	else return size_impl<type>();
}

template<typename t> constexpr auto size = size_impl_dispatcher<t>();

template<auto ind,auto cur=0> constexpr auto&& nth(auto& first, auto&... args) { if constexpr (cur==ind) return first; else return nth<ind,cur+1>(args...); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 1) { auto&& [f1]=r; return forward_like<decltype(r)>(nth<ind>(f1)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 2) { auto&&[f1,f2]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 3) { auto&&[f1,f2,f3]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 4) { auto&&[f1,f2,f3,f4]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 5) { auto&&[f1,f2,f3,f4,f5]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 6) { auto&&[f1,f2,f3,f4,f5,f6]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 7) { auto&&[f1,f2,f3,f4,f5,f6,f7]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 8) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 9) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 10) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 11) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 12) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 13) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 14) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 15) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 16) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 17) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 18) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 19) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 20) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 21) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 22) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 23) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 24) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 25) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 26) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 27) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 28) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 29) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 30) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 31) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 32) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 33) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 34) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 35) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 36) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 37) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 38) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 39) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 40) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 41) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 42) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 43) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 44) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 45) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 46) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 47) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 48) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 49) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 50) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 51) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 52) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 53) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 54) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 55) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 56) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 57) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 58) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 59) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 60) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 61) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 62) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 63) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 64) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 65) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 66) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 67) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 68) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 69) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 70) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 71) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 72) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 73) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 74) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 75) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 76) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 77) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 78) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 79) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 80) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 81) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 82) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 83) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 84) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 85) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 86) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 87) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 88) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 89) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 90) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 91) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 92) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 93) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 94) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 95) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 96) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 97) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 98) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 99) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==100) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==101) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==102) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==103) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==104) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==105) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==106) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==107) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==108) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==109) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==110) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==111) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==112) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==113) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==114) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==115) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==116) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==117) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==118) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==119) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==120) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==121) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==122) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==123) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==124) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==125) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==126) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==127) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==128) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127,f128]=r; return forward_like<decltype(r)>(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127,f128)); }
template<auto ind> constexpr auto& get(ascip_details::type_result_for_parser_concept& r){ return r; }
constexpr void test () {
	struct tf1{ char f1; };
	struct tf2{ char f1, f2; };
	static_assert( []{ tf1 t{ 'a' }; return get<0>(t); }() == 'a' );
	static_assert( []{ tf2 t{ 'a', 'b' }; return get<0>(t); }() == 'a' );
	static_assert( []{ tf2 t{ 'a', 'b' }; return get<1>(t); }() == 'b' );
}
} // namespace ascip_reflection

/* desicions:
 *
 * each parser is a class drived from base_parser class with passing it self as template parameter for base_parser
 * wrapping parser contains wrapped parser with [[no_unique_address]] attribute (most parsers are empty)
 * seq parser stores parsed part even if parser fails: the object must to be deleted and we cannot clean everithing
 * use seq operators in adl instead on base parser - we cannot use it on all parsers (+(++p) for example)
 * seq with left seq operator - it was hard to implement in adl at the time and there was no reason to do so, so it's a part of seq structure
 * transfom method - use it for trnasform parser with mutator structure. ussage: inject skipping parser, add stop number in right reqursion
 * variant_pos_tag - used for get variant position from context. there is no easy way to get it from type or by this.
 *   (lambda works as unique type only from free function, inside a template<...> struct {...}; it doesn't)
 * error handling:
 *   must<"name">(parser) catches semantic errors, calls lambda passed in parse method (via ctx)
 *   check result method - semact parser checks result with user method and returns user result (user also can throw error)
 *   check result method alternative - same method as lambda in sequence
 *   error handler parameters: line number, result, message, current src
 *   result checker parameters: line number, result, src on seq start
 *   implement function enabled from adl for roll back src on line begin
 * left reqursion implementation: implmemented with reverse variant parser. it needs in extra make_result method for temporary
 *   store parse result.
 */

namespace {


template<template<typename...>class tuple, typename factory_t=void>
struct ascip {

using holder = ascip<tuple, factory_t>;
using parse_result = decltype(-1);

template<typename parser> struct base_parser : ascip_details::adl_tag {
	using type_in_base = parser;
	using holder = ascip<tuple, factory_t>;

	constexpr static int start_context_arg = 1;
	constexpr static const char* source_symbol = "ab";

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert( requires(const parser& p){ p.p; }, "child parser should define own parse method or have p field" );
		return static_cast<const parser&>(*this)
			.p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}

       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

constexpr auto operator()(auto act) const {
	return semact_parser<type_in_base, decltype(auto(act))>{ {},
		static_cast<decltype(act)&&>(act),
		static_cast<const type_in_base&>(*this)
	};
}

constexpr auto operator|(const ascip_details::parser auto& p2) const {
	return variant_parser( static_cast<const parser&>(*this), p2 );
}
constexpr auto operator|(const ascip_details::nonparser auto& p2) const {
	return variant_parser( static_cast<const parser&>(*this), value_parser(p2) );
}
constexpr auto operator!() const {
	return negate_parser<parser>{ static_cast<const parser&>(*this) };
}
constexpr auto operator-()const{ return opt_parser<parser>{ static_cast<const parser&>(*this) }; }
constexpr auto operator+()const{ return unary_list_parser<parser>{ static_cast<const parser&>(*this) }; }
constexpr auto operator*()const{ return -( +(static_cast<const parser&>(*this)) ); }

template<ascip_details::parser right> constexpr auto operator%(const right& r) const {
	return binary_list_parser( static_cast<const parser&>(*this), r ); }
constexpr auto operator%(char r)const{ return binary_list_parser( static_cast<const parser&>(*this), value_parser{r} ); }

template<ascip_details::parser right> constexpr auto operator-(const right& r)const{
	return different_parser( static_cast<const parser&>(*this), r ); }

};

constexpr static auto make_test_ctx() { return ascip_details::make_ctx<ascip_details::new_line_count_tag>(1); }
constexpr static auto make_test_ctx(auto err_handler){ return make_ctx<ascip_details::err_handler_tag>(err_handler, make_test_ctx()); }
//template<auto... i> friend constexpr auto make_test_ctx(const base_parser<auto>&) { return ascip_details::make_ctx<ascip_details::parser_concept_check_tag>(1); }
template<auto... i, typename parser_param> friend constexpr auto make_test_ctx(const base_parser<parser_param>&) { return ascip_details::make_ctx<ascip_details::parser_concept_check_tag>(1); }
// ^^ implemented for ascip_details::parser concept 

       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<typename type> constexpr static auto mk_vec() { return factory_t{}.template mk_vec<type>(); }
constexpr static auto mk_str() { return factory_t{}.template mk_str(); }
constexpr static auto mk_str(auto&& v) { return factory_t{}.template mk_str(static_cast<decltype(v)&&>(v)); }
constexpr static auto mk_sv(auto&& v) { return factory_t{}.template mk_sv(static_cast<decltype(v)&&>(v)); }

constexpr static auto test_parser_char(const auto& p, auto&& s, auto pr) {
	char result='z';
	auto answer = p.parse(make_test_ctx(), make_source(s), result);
	answer /= (answer==pr);
	return result;
}

template<auto cur_ind, auto cnt, auto... inds>
constexpr static bool test_cmp_struct_impl(const auto& obj, auto cur_val, auto... vals) {
	//cur_val /= (ascip_reflection::get<sizeof...(inds)>(obj)==cur_val);
	auto mutable_copy = ascip_reflection::get<sizeof...(inds)>(obj);
	mutable_copy /= (ascip_reflection::get<sizeof...(inds)>(obj)==cur_val);
	if constexpr (sizeof...(vals) == 0) return true;
	else return test_cmp_struct_impl<cur_ind+1, cnt, inds..., sizeof...(inds)>(obj, vals...);
}
constexpr static bool test_cmp_struct(const auto& obj, auto... vals) {
	return test_cmp_struct_impl<0, sizeof...(vals)>(obj, vals...);
}

constexpr static auto test_parser_parse(auto&& r, auto p, auto&& src, auto pr) {
	auto rr = p.parse(make_test_ctx(), make_source(src), r);
	rr /= (rr==pr);
	return r;
}

constexpr static auto test_cmp_vec(const auto& vec, auto... vals) {
	auto correct = mk_vec<decltype(auto(vec[0]))>();
	(correct.emplace_back(vals), ...);
	(void)(vec.size() / (vec.size() == sizeof...(vals)));
	//TODO: show also i shomehow in compiletime
	for(auto i=0;i<vec.size();++i) (void)(vec[i] / (vec[i] == correct[i]));
	return true;
}

constexpr static bool test_parser_parse_r(auto&& r, auto p, auto&& src, auto pr, auto... vals) {
	auto parse_method_result = p.parse(make_test_ctx(), make_source(src), r);
	parse_method_result /= (parse_method_result==pr);
	test_cmp_vec(r, vals...);
	return true;
}

constexpr static bool test_parser_parse_r_str(auto p, auto&& src, auto pr, auto... vals) {
	return test_parser_parse_r(mk_str(), p, static_cast<decltype(src)&&>(src), pr, static_cast<decltype(vals)&&>(vals)...);
}

       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<ascip_details::string_view type>
constexpr static auto make_source(type&& src) {
	struct strsrc {
		type src;
		unsigned ind = 0;
		constexpr auto operator()(){ return src[ind++]; }
		constexpr explicit operator bool() const { return ind < src.size(); }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
	} ret{ std::forward<decltype(src)>(src) };
	return ret;
}

constexpr static auto make_source(std::integral auto sym) {
	struct {
		decltype(sym) val; bool where_is_more=true;
		constexpr auto operator()(){ where_is_more=false; return val; }
		constexpr explicit operator bool() const { return where_is_more; }
		constexpr auto& operator += (int v) { (void)( v==1 || (throw 1,1) ); where_is_more=false; return *this; }
	} ret{ sym };
	return ret;
}

constexpr static auto make_source(const auto* vec) {
	struct {
		decltype(vec) val;
		unsigned sz;
		unsigned ind = 0;
		constexpr auto operator()(){ return val[ind++]; }
		constexpr explicit operator bool() const { return ind < sz-1; }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
	} ret{ vec, ascip_details::strlen(vec) };
	return ret;
}

// implemented for ascip_details::parser concept 
//friend constexpr auto make_source(const base_parser<auto>& p) { return make_source(p.source_symbol); }
template<typename param>
friend constexpr auto make_source(const base_parser<param>& p) { return make_source(p.source_symbol); }

constexpr static bool test_sources(auto&& s) {
	(void)( !!s        || (throw 0, 1) );
	(void)( s() == 'a' || (throw 1, 1) );
	(void)( !!s        || (throw 0, 1) );
	(void)( s() == 'b' || (throw 2, 1) );
	(void)( !s         || (throw 0, 1) );
	return true;
}
constexpr static void test_sources() {
	static_assert( make_source("ab").ind == 0, "source from qutoed string must to be with index" );
	static_assert( !!make_source("") == false, "source from array works, setp 0" );
	static_assert( test_sources( make_source("ab") ) );
	static_assert( test_sources( make_source(factory_t{}.mk_sv("ab")) ) );

	static_assert( test_sources( make_source(factory_t{}.mk_str("ab")) ) );

	static_assert( ({ constexpr const char* v="ab"; test_sources( make_source(v) );}) );

	static_assert( []{
		auto s = make_source('a');
		(void)( !!s       || (throw 0, 1) );
		(void)( s() == 'a'|| (throw 1, 1) );
		(void)( !s        || (throw 2, 1) );
		return true;
	}(), "source for debug with single symbol" );


	static_assert((char)make_source("я")() == (char)0xD1);
	static_assert(({auto s=make_source("я");s();(char)s();}) == (char)0x8F);

	static_assert( []{
		auto s = make_source("я");
		(void)( !!s         || (throw 0, 1) );
		(void)( s() != s()  || (throw 1, 1) );
		(void)( !s          || (throw 2, 1) );
		return true;
	}(), "source for debug with multibyte symbol" );
}
template<typename char_type, auto sz>
constexpr static auto& print_to(auto& os, const ascip_details::string_literal<char_type, sz>& what) { return os << what.value; }
constexpr static auto& print_to(auto& os, const auto& what) { return os << what; }
constexpr static void write_out_error_msg(
		auto& os,
		auto fn,
		auto msg,
		auto expt,
		auto src,
		auto ln
		) {
	print_to(os, msg) << '\n';
	print_to(os, fn) << ':' << ln << ' ';
	auto shift = 0;
	do{ ++shift; src += -2; } while(src()!='\n');
	os << '\n';
	while(src) {
		auto cur = src();
		if(cur == '\n') break;
		os << cur;
	}
	os << '\n';
	for(auto i=0;i<shift;++i) os << '-';
	os << "^\n";
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<auto... inds, template<typename...>class wrapper, typename... parsers>
constexpr static bool exists_in_get(const wrapper<parsers...>* seq, const auto& checker) {
	if constexpr (sizeof...(inds) == sizeof...(parsers)) return false;
	else return
		   checker((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr)
		|| exists_in((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr, checker)
		|| exists_in_get<inds..., sizeof...(inds)>(seq, checker)
		;
}
constexpr static bool exists_in(auto&& src, const auto& checker) { return exists_in(&src, checker); }
template<template<typename>class wrapper, typename parser>
constexpr static bool exists_in_drived(const wrapper<parser>* src, const auto& checker) {
	return exists_in(static_cast<const parser*>(src), checker); }
constexpr static bool exists_in(auto* src, const auto& checker) {
	if constexpr (std::is_same_v<typename std::decay_t<decltype(*src)>::type_in_base, std::decay_t<decltype(*src)>>)
		return checker(src);
	else return exists_in_drived(src, checker);
}
constexpr static bool exists_in(auto* src, const auto& checker) requires requires{ src->p; } {
	using type = std::decay_t<decltype(src->p)>;
	return checker(src) || exists_in((type*)nullptr, checker); }
constexpr static bool exists_in(auto* src, const auto& checker) requires requires{ src->lp; } {
	using ltype = std::decay_t<decltype(src->lp)>;
	using rtype = std::decay_t<decltype(src->rp)>;
	return checker(src)
		|| checker((ltype*)nullptr) || checker((rtype*)nullptr)
		|| exists_in((ltype*)nullptr, checker) || exists_in((rtype*)nullptr, checker)
		;
}
constexpr static bool exists_in(auto* src, const auto& checker) requires requires{ src->seq; } {
	using seq_t = decltype(src->seq);
	return checker(src) || exists_in_get((seq_t*)nullptr, checker); }
constexpr static bool exists_in(auto* src, const auto& checker) requires requires{ src->s; src->b; } {
	//NOTE: it's for injected_parser, but without forward declaration
	return exists_in((std::decay_t<decltype(src->b)>*)nullptr, checker) ; }

constexpr static bool test_exists_in() {
	auto checker = [](const auto* s){ return std::is_same_v<std::decay_t<decltype(*s)>, std::decay_t<decltype(char_<'a'>)>>; };
	static_assert(  exists_in(char_<'a'>, checker) );
	static_assert( !exists_in(char_<'b'>, checker) );
	static_assert(  exists_in(skip(char_<'a'>), checker) );
	static_assert( !exists_in(skip(char_<'b'>), checker) );
	static_assert(  exists_in(char_<'b'> | char_<'a'>, checker) );
	static_assert( !exists_in(char_<'b'> | char_<'x'>, checker) );
	static_assert(  exists_in(char_<'b'> - char_<'a'>, checker) );
	static_assert( !exists_in(char_<'b'> - char_<'x'>, checker) );
	static_assert(  exists_in(char_<'c'> >> (char_<'b'> - char_<'a'>), checker) );
	static_assert(  exists_in(char_<'c'> - (char_<'b'> >> char_<'a'>), checker) );
	static_assert(  exists_in(skip(char_<'c'> >> char_<'b'> >> char_<'a'>++), checker) );
	return true;
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<auto sym> struct char_parser : base_parser<char_parser<sym>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		const bool ok = src() == sym;
		if(ok) {
			ascip_details::eq(result, sym);
			ascip_details::count_new_line(ctx, sym, result);
		}
		return -1 + (2 * ok);
	}

	constexpr bool test() const {
		char r;
		parse(make_test_ctx(), make_source(sym), r) == 1           || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym+1), r) == -1        || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym-1), r) == -1        || (throw __LINE__, 1);

		r=sym+1;
		(parse(make_test_ctx(), make_source(sym), r),r) == sym     || (throw __LINE__, 1);

		r=sym+1;
		(parse(make_test_ctx(), make_source(sym-1), r),r) == sym+1 || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		if constexpr (sym=='\n') {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);
		} else {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		}

		return true;
	}
};
template<auto val> constexpr static const auto char_ = char_parser<val>{};

constexpr static void test_parser_char() {
	static_assert( char_<'a'>.test() ); static_assert( char_<'z'>.test() );
	static_assert( char_<'!'>.test() ); static_assert( char_<'Z'>.test() );
	static_assert( char_<' '>.test() ); static_assert( char_<'\n'>.test() );
	static_assert( []{char r;return char_<'a'>.parse(make_test_ctx(), make_source("abc"), r);}() == 1 );
	static_assert( []{char r;return char_<'b'>.parse(make_test_ctx(), make_source("abc"), r);}() == -1 );
	static_assert( []{char r;char_<'a'>.parse(make_test_ctx(), make_source("abc"), r);return r;}() == 'a' );
}

template<ascip_details::string_literal val> struct literal_parser : base_parser<literal_parser<val>> {
	constexpr parse_result parse(auto&&, auto src, auto&) const {
		//TODO: faster? add [] operator in src for direct access (operator[](auto i){return val[ind+i];})
		auto i=-1, r=0;
		{
			auto tsrc = src;
			tsrc += val.size()-1;
			if(!tsrc) return -1;
		}
		while(++i<val.size()) r += (src() == val[i]);
		return ((r+1)*(r==val.size())) - 1;
	}
};
template<ascip_details::string_literal v> constexpr static auto lit = literal_parser<v>{};
constexpr static bool test_literal_parser() {
	char r;
	static_assert( literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd"), r) == 3 );
	static_assert( literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd_tail"), r) == 3 );
	static_assert( literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("abcd"), r) == 4 );
	static_assert( literal_parser<"abcdef">{}.parse(make_test_ctx(), make_source("abcdef"), r) == 6 );
	static_assert( literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("bbcd"), r) == -1 );
	static_assert( literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("ab"), r) == -1 );
	return true;
}

template<typename t> struct value_parser : base_parser<value_parser<t>> {
	t val;
	constexpr value_parser(t v) : val(v) {}
	constexpr parse_result parse(auto&&, auto src, auto& result) const {
		const bool ok = src() == val;
		if(ok) ascip_details::eq(result, val);
		return -2 * !ok + 1;
	}

	constexpr bool test() const {
		char r;
		parse(make_test_ctx(), make_source(val), r) == 1           || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val+1), r) == -1        || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val-1), r) == -1        || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val), r),r) == val     || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val-1), r),r) == val+1 || (throw __LINE__, 1);

		return true;
	}
};






constexpr static void test_parser_value() {
	static_assert( value_parser{ 'a' }.test() ); static_assert( value_parser{ 'Z' }.test() );
	static_assert( value_parser{ L'!' }.test() ); static_assert( value_parser{ '\n' }.test() );

	static_assert( ({char r;value_parser{'a'}.parse(make_test_ctx(), make_source("abc"), r);}) == 1 );
	static_assert( ({char r;value_parser{'b'}.parse(make_test_ctx(), make_source("abc"), r);}) == -1 );

}

constexpr static struct space_parser : base_parser<space_parser> {
	constexpr parse_result parse(auto&& ctx,auto src, auto& r) const {
		auto sym = src();
		const bool is_space = 0x07 < sym && sym < '!'; // 0x08 is a backspace
		ascip_details::count_new_line(ctx, sym, r);
		return -1 + (2 * is_space);
	}
	constexpr bool test() const {
		constexpr char r=0x00;
		parse(make_test_ctx(), make_source(' '), r) == 1    || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\n'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\t'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('!'), r) == -1   || (throw __LINE__, 1);
		r == 0x00                                           || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);

		return true;
	}
} space {};

constexpr static struct any_parser : base_parser<any_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = 0;
		decltype(src()) cur;
		do { 
			cur = src();
			ascip_details::eq( result, cur );
			count_new_line(ctx, cur, result);
			++ret;
		}
		while(src && (cur & 0x80)) ;
		return ret;
	}
	constexpr bool test() const {
		char r=0x00; ascip_details::type_any_eq_allow rr;
		parse(make_test_ctx(), make_source(' '), r) == 1       || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('Z'), r),r) == 'Z' || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('~'), r),r) == '~' || (throw __LINE__, 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("я"), rr)  == 2);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"я"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"яz"), rr) == 1);

		auto ctx = make_test_ctx();
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);

		return true;
	}
} any {};

constexpr static struct int_parser : base_parser<int_parser> {
	constexpr static bool is_int(auto s) { return '0' <= s && s <= '9'; }
	constexpr bool next(auto cur, auto& result) const {
		const bool isint = is_int(cur);
		result += ((int)(cur - '0')) * isint;
		result /= (!isint * 9) + 1;
		return isint;
	}
	constexpr parse_result parse(auto&&, auto src, auto& _result)  const {
		auto sign = src();
		if(sign != '-' && sign != '+' && !is_int(sign)) return -1;
		int signer = sign == '-' ? -1 : is_int(sign) || sign=='+';
		auto& result = ascip_details::eq(_result, is_int(sign) ? sign-'0' : 0);
		auto ret = 1;
		while(src && next(src(), result *= 10)) ++ret;
		result *= signer;
		return ret;
	}
	constexpr auto parse_without_preparation(auto src, auto& result) const {
		auto ret = 0;
		while(src && next(src(), result *= 10)) ++ret;
		return ret;
	}

	constexpr bool test() const {
		int_parser p; auto r=0;

		auto t = [](auto&& src, auto sym_cnt, auto answer) {
			auto r = answer + 100;
			int_parser p{};
			p.parse(make_test_ctx(), make_source(src), r) == sym_cnt || (throw __LINE__, 1);
			r == answer                                              || (throw __LINE__, 1);
			return true;
		};

		static_assert( t("-1", 2, -1) );
		static_assert( t("1", 1, 1) );
		static_assert( t("+2", 2, 2) );
		static_assert( t("+0", 2, 0) );
		static_assert( t("0", 1, 0) );
		static_assert( t("+103", 4, 103) );
		static_assert( t("103", 3, 103) );
		

		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("!"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("a"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("A"), r);}) == -1 );


		return true;
	}

} int_ {};

constexpr static struct float_point_parser : base_parser<float_point_parser> {
	constexpr static auto pow(auto what, auto to) { const auto m = what; for(auto i=1;i<to;++i) what*=m; return what; }
	constexpr parse_result parse(auto&& ctx, auto src, auto& result)  const {
		result = 0;
		auto int_pos = src;
		auto dec_pos = src;
		auto left_result = int_.parse_without_preparation(int_pos, result);
		if(left_result <= 0 && int_pos() != '.') return -1;
		dec_pos+=left_result;
		if(!dec_pos || dec_pos()!='.') return -1;
		auto right_result = int_.parse_without_preparation(dec_pos, result);
		if(right_result <= 0) return -1;
		result /= pow(10, right_result);
		return left_result + right_result + 1;
	}
	constexpr bool test() const {
		float_point_parser p; 
		auto t = [](auto&& src, auto sym_cnt, auto answer) {
			float_point_parser p; 
			auto r = answer + 100;
			auto pr = p.parse(make_test_ctx(), make_source(src), r);
		       	pr /= (pr == sym_cnt);
			r /= (r == answer);
			return true;
		};

		static_assert( p.pow(10, 1) == 10 );
		static_assert( p.pow(10, 2) == 100 );
		static_assert( p.pow(10, 3) == 1000 );
		static_assert( t("0.5", 3, 0.5) );
		static_assert( t("1.5", 3, 1.5) );
		static_assert( t("3.075", 5, 3.075) );


		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("0"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("a"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("1."), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("5+3"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("5-3"), r); }) == -1);


		return true;
	}
} fp {};

template<auto from, auto to> struct range_parser : base_parser<range_parser<from,to>> { 
	constexpr parse_result parse(auto&&, auto src, auto& result) const {
		auto sym = src();
		const bool ok = from <= sym && sym <= to;
		if(ok) ascip_details::eq( result, sym );
		return -2 * !ok + 1;
	}

};
constexpr static const auto lower = range_parser<'a','z'>{};
constexpr static const auto upper = range_parser<'A','Z'>{};
constexpr static const auto digit = range_parser<'0','9'>{};
constexpr static const auto d10 = range_parser<'0', '9'>{};
constexpr static const auto ascii = range_parser<(char)0x01,(char)0x7F>{};

constexpr static bool test_range_parser() {

	static_assert( ({char r;lower.parse(make_test_ctx(), make_source("a"), r);r;}) == 'a' );
	static_assert( ({char r;lower.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source("A"), r);}) == 1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source('~'+1), r);}) == 1 );
	static_assert( ({char r;ascii.parse(make_test_ctx(), make_source('~'+2), r);}) == -1 );

	return true;
}

       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<typename parser, typename act_t> struct semact_parser : base_parser<semact_parser<parser,act_t>> {
	act_t act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr bool is_any_arg_pattern = !requires{ act(); };
		if constexpr(ascip_details::is_in_concept_check(decltype(auto(ctx)){})) return 0;
		else if constexpr(std::is_same_v<ascip_details::type_any_eq_allow&, decltype(result)>)
			return p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
		else if constexpr(requires{ act(result); requires std::is_pointer_v<decltype(act(result))>;} ) {
			auto* nr = act(result);
			if constexpr (requires{ p.parse_with_user_result(ctx,src,*nr); })
				return p.parse_with_user_result(static_cast<decltype(ctx)&&>(ctx),src,*nr);
			else return p.parse(ctx, src, *nr);
		}
		else if constexpr (requires{ act(result); requires std::is_lvalue_reference_v<decltype(act(result))>; } ) {
			auto& nr = act(result);
			if constexpr (requires{ p.parse_with_user_result(ctx,src,nr); })
				return p.parse_with_user_result(static_cast<decltype(ctx)&&>(ctx),src,nr);
			else return p.parse(ctx, src, nr);
		}
		else if constexpr(requires{ act(result); } && !requires{act(0, ctx, src, result);} ) {
			auto nr = act(result);
			if constexpr (requires{ p.parse_with_user_result(ctx,src,nr); })
				return p.parse_with_user_result(static_cast<decltype(ctx)&&>(ctx),src,nr);
			else return p.parse(ctx, src, nr);
		}
		else {
			auto ret = p.parse(ctx, src, result);
			if(ret >= 0) {
				if constexpr (requires{ act(ret, ctx, src, result); }) act(ret, ctx, src, result);
				else if constexpr (requires{ act(ret, result); }) act(ret, result);
				else act();
			}
			return ret;
		}
	}
};

constexpr static bool test_semact() {

	static_assert( ({char r='z',a='y';(char_<'a'> >> char_<'b'>([&a](){a='c';})).parse(make_test_ctx(),make_source("ab"),r);a;}) == 'c', "semact: calls if parsed");
	static_assert( ({char r='z',a='y';(char_<'a'> >> char_<'b'>([&a](){a='c';})).parse(make_test_ctx(),make_source("au"),r);a;}) == 'y', "semact: silent if fails");

	return true;
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<ascip_details::parser parser> struct negate_parser : base_parser<negate_parser<parser>> {
	parser p;
	constexpr negate_parser() =default ;
	constexpr negate_parser(negate_parser&&) =default ;
	constexpr negate_parser(const negate_parser&) =default ;
	constexpr negate_parser(parser p) : p(std::move(p)) {}
	constexpr auto operator!() const { return p; }
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), result);
		return ret * (-1);
	}
};

constexpr static bool test_negate() {

	static_assert( ({char r;(!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);}) == -1 );
	static_assert( ({char r;(!char_<'a'>).parse(make_test_ctx(), make_source('b'), r);}) ==  1 );
	static_assert( ({char r='z';(!char_<'a'>).parse(make_test_ctx(), make_source('b'), r);r;}) ==  'z' );

	static_assert( ({char r;(!!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);}) == 1 );
	static_assert( ({char r;(!!!!char_<'a'>).parse(make_test_ctx(), make_source('a'), r);r;}) == 'a' );


	static_cast<const decltype(auto(char_<'a'>))&>(!!char_<'a'>);
	static_cast<const decltype(auto(char_<'a'>))&>(!!!!char_<'a'>);
	static_cast<const opt_parser<negate_parser<char_parser<'a'>>>&>(-(!char_<'a'>));
	static_cast<const opt_parser<char_parser<'a'>>&>(-(!!char_<'a'>));

	return true;
}

template<ascip_details::parser parser> struct opt_parser : base_parser<opt_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr opt_parser(opt_parser&&) =default ;
	constexpr opt_parser(const opt_parser&) =default ;
	constexpr opt_parser() =default ;
	constexpr opt_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		using result_type = std::decay_t<decltype(result)>;

		if(!src) return 0;
		if constexpr(ascip_details::optional<result_type>) {
			auto ret = p.parse(ctx, src, result.emplace());
			if(ret<0) result.reset();
			return ret * (ret >= 0);
		}
		else {
			auto ret = p.parse(ctx, src, result);
			return ret * (ret >= 0);
		}
	}
};

constexpr static bool test_optional() {
	static_assert( test_parser_char(-char_<'a'>, "b", 0) == 'z' );
	static_assert( test_parser_char(-char_<'a'>, "a", 1) == 'a' );
	return true;
}

template<ascip_details::parser parser> struct omit_parser : base_parser<omit_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr omit_parser() =default ;
	constexpr omit_parser(omit_parser&&) =default ;
	constexpr omit_parser(const omit_parser&) =default ;
	constexpr omit_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		return p.parse(ctx, src, r);
	}
};





template<auto val> constexpr static const auto _char = omit(char_<val>);
constexpr static const auto nl = _char<'\n'>;

constexpr static bool test_omit() {
	static_assert( test_parser_char(omit(char_<'a'>), "a", 1) == 'z' );
	static_assert( test_parser_char(omit(char_<'a'>), "b", -1) == 'z' );
	static_assert( test_parser_char(_char<'a'>, "a", 1) == 'z' );
	static_assert( test_parser_char(_char<'a'>, "b", -1) == 'z' );
	return true;
}

template<auto val, ascip_details::parser parser> struct tmpl_as_parser : base_parser<tmpl_as_parser<val,parser>> {
	[[no_unique_address]] parser p;
	constexpr tmpl_as_parser() =default ;
	constexpr tmpl_as_parser(tmpl_as_parser&&) =default ;
	constexpr tmpl_as_parser(const tmpl_as_parser&) =default ;
	constexpr tmpl_as_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};
template<typename value_t, ascip_details::parser parser> struct as_parser : base_parser<as_parser<value_t, parser>> {
	value_t val;
	[[no_unique_address]] parser p;
	constexpr as_parser() =default ;
	constexpr as_parser(as_parser&&) =default ;
	constexpr as_parser(const as_parser&) =default ;
	constexpr as_parser(value_t val, parser p) : val(std::move(val)), p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};
constexpr static bool test_as() {
	static_assert( test_parser_char(as(char_<'a'>, 'b'), "a", 1) == 'b' );
	static_assert( test_parser_char(as(char_<'a'>, 'b'), "b", -1) == 'z' );
	static_assert( test_parser_char(as(int_, 'b'), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(int_), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(int_), "a", -1) == 'z' );
	static_assert( []{
		auto p = char_<'a'>;
		return test_parser_char(as(p, 'b'), "a", 1);
	}() == 'b' );
	return true;
}

template<typename good_result, ascip_details::parser parser>
struct result_checker_parser : base_parser<result_checker_parser<good_result, parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert(
			   ascip_details::is_in_concept_check(decltype(auto(ctx)){})
			|| std::is_same_v<ascip_details::type_any_eq_allow, decltype(auto(result))>
			|| std::is_same_v<good_result, decltype(auto(result))>
			, "can only parser to required type" );
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(auto(src))&&>(src), result);
	}
};
template<typename needed, ascip_details::parser type> struct cast_parser : base_parser<cast_parser<needed,type>> {
	type p;
	constexpr static auto& check_result(auto& result) {
		if constexpr( std::is_same_v<std::decay_t<decltype(result)>, ascip_details::type_any_eq_allow> ) return result;
		else {
			static_assert(requires{ static_cast<needed&>(result); }, "the result must to be castable to needed type" );
			return static_cast<needed&>(result);
		}
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr ( ascip_details::is_in_concept_check(decltype(auto(ctx)){}) ) return 0;
		else return p.parse(static_cast<decltype(ctx)&&>(ctx), src, check_result(result));
	}
};
constexpr static bool test_checkers() {
	//NOTE: for c++23 we can only test if it works and cannot test if it fails
	static_assert( requires(char& r) {
		check<char>(char_<'a'>).parse(make_test_ctx(),make_source('a'),r);
		});
	static_assert( requires(char& r) {
		cast<char>(char_<'a'>).parse(make_test_ctx(),make_source('a'),r);
		});
	static_assert( requires(ascip_details::type_any_eq_allow& r) {
		check<char>(char_<'a'>).parse(make_test_ctx(),make_source('a'),r);
		});
	static_assert( requires(ascip_details::type_any_eq_allow& r) {
		cast<char>(char_<'a'>).parse(make_test_ctx(),make_source('a'),r);
		});
	return true;
}

template<ascip_details::parser left, ascip_details::parser right> struct different_parser : base_parser<different_parser<left, right>> {
	left lp;
	right rp;
	constexpr different_parser( left l, right r ) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow fake_result;
		if(rp.parse(ctx, src, fake_result) >= 0) return -1;
		return lp.parse(ctx, src, result);
	}
};
template<ascip_details::parser left, ascip_details::parser right>
different_parser(left, right) -> different_parser<left, right>;
template<typename v, typename p> as_parser(v,p) -> as_parser<v,p>;

constexpr static bool test_different() {


	return true;
}

       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

struct variant_pos_tag{};
struct variant_stack_tag{};
struct variant_stack_result_tag{};
template<ascip_details::parser parser> struct use_variant_result_parser : base_parser<use_variant_result_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}
};
template<auto val> struct variant_pos_value{ constexpr static auto pos = val; };
template<ascip_details::parser... parsers> struct variant_parser : base_parser<variant_parser<parsers...>> {
	using self_type = variant_parser<parsers...>;
	tuple<parsers...> seq;
	constexpr variant_parser(const variant_parser& other) : seq(other.seq) {}
	constexpr variant_parser(variant_parser&& other) : seq(std::move(other.seq)) {}
	constexpr explicit variant_parser( parsers... l ) : seq( std::forward<parsers>(l)... ) {}

	template<auto ind, auto cnt, auto cur, typename cur_parser, typename... tail>
	constexpr static auto _cur_ind() {
		constexpr const bool skip = ascip_details::is_specialization_of<cur_parser, use_variant_result_parser>;
		if constexpr (ind == cnt) {
			if constexpr (skip) return -1;
			else return cur;
		}
		else return _cur_ind<ind,cnt+1,cur+(!skip),tail...>();
	}
	template<auto ind> consteval static auto cur_ind() { return _cur_ind<ind,0,0,parsers...>(); }
	//TODO: make current result with if constexpr with the this sequence: create, adl emplace, inner emplace
	template<auto ind> constexpr auto& current_result(auto& result) const
	requires requires{ create<1>(result); } {
		if constexpr (cur_ind<ind>()<0) return result;
		else return create<cur_ind<ind>()>(result);
	}
	template<auto ind> constexpr auto& current_result(auto& result) const
	requires (requires{ result.template emplace<1>(); } && !requires{ create<1>(result); }) {
		if constexpr (cur_ind<ind>()<0) return result;
		else return result.template emplace<cur_ind<ind>()>();
	}
	template<auto ind> constexpr auto& current_result(auto& result) const {
		return result;
	}
	template<auto ind> constexpr auto parse_ind(auto&& ctx, auto& src, auto& result) const {
		auto parse_ctx = make_ctx<variant_pos_tag>(variant_pos_value<ind>{}, ctx);
		auto prs = [&](auto&& r){ return get<ind>(seq).parse(parse_ctx, src, current_result<ind>(r)); };
		if constexpr (ind+1 == sizeof...(parsers)) return prs(result);
		else {
			auto parse_result = prs(ascip_details::type_any_eq_allow{});
			if(parse_result > 0) return prs(result);
			return parse_ind<ind+1>(ctx, src, result);
		}
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (exists_in_ctx<self_type>(decltype(auto(ctx)){})) 
			return parse_ind<0>(ctx, src, result);
		else {
			auto variant_ctx =
				make_ctx<variant_stack_result_tag>(&result,
					make_ctx<variant_stack_tag>(this, ctx)
				)
			;
			auto nctx = make_ctx<self_type>(&variant_ctx, variant_ctx);
			return parse_ind<0>(nctx, src, result);
		}
	}

	template<ascip_details::parser right>
	constexpr auto operator|(const right& p2) const {
		return ascip_details::init_with_get<variant_parser<parsers..., right>>(seq, p2); }
	constexpr auto operator|(char p2) { return *this | value_parser( p2 ); }
};

template<ascip_details::parser... parsers>
variant_parser(parsers...) -> variant_parser<parsers...>;

constexpr static const auto alpha = lower | upper;

constexpr static bool test_variant() {
	constexpr const auto run_parse = [](const auto& p, auto&& src, auto& r) ->decltype(auto(r)) {
		return p.parse(make_test_ctx(), make_source(src), r);
	};

	static_assert( ({ char r;run_parse(char_<'a'>|char_<'b'>, "a", r);r;}) == 'a' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b', "b", r);r;}) == 'b' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "c", r);}) == 1 );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "c", r);r;}) == 'c' );
	static_assert( ({ char r;run_parse(char_<'a'>|'b'|'c', "d", r);}) == -1 );

	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "b", r);r;}) == 'c' );
	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "a", r);r;}) == 'a' );
	static_assert( ({ char r;run_parse(as(char_<'b'>,'c')|char_<'a'>, "a", r);  }) ==  1 );


	return true;
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


struct rvariant_stack_tag {};
struct rvariant_crop_ctx_tag {};
struct rvariant_copied_result_tag {};
template<auto ind> struct rvariant_stop_val { constexpr static auto val = ind; };
constexpr static struct rvariant_lreq_parser : base_parser<rvariant_lreq_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr const bool need_in_result =
			   !is_in_concept_check(decltype(auto(ctx)){})
			&& !std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>
			;
		if constexpr (need_in_result) {
			result = std::move(*search_in_ctx<rvariant_copied_result_tag>(ctx));
			search_in_ctx<rvariant_copied_result_tag>(ctx) = nullptr;
		}
		return 0;
	}
} rv_lreq{};
template<auto stop_ind>
struct rvariant_rreq_parser : base_parser<rvariant_rreq_parser<stop_ind>> {
	constexpr parse_result parse(auto&& ctx, auto, auto&) const requires (is_in_concept_check(decltype(auto(ctx)){})) { return 0; }
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const requires (!is_in_concept_check(decltype(auto(ctx)){})) {
		if(!src) return 0;
		auto* var = search_in_ctx<rvariant_stack_tag>(ctx);
		auto* croped_ctx = search_in_ctx<rvariant_crop_ctx_tag>(ctx);
		auto nctx = make_ctx<rvariant_crop_ctx_tag>(croped_ctx, *croped_ctx);
		return var->template parse_without_prep<stop_ind+1>(nctx, src, result);
	}
};
template<auto stop_ind> constexpr static rvariant_rreq_parser<stop_ind> _rv_rreq{};
constexpr static struct rvariant_rreq_pl_parser : base_parser<rvariant_rreq_pl_parser> { constexpr parse_result parse(auto&& ctx, auto src, auto& result) const { return 0; } } rv_rreq{};
constexpr static struct rvariant_req_parser : base_parser<rvariant_req_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (is_in_concept_check(decltype(auto(ctx)){})) return 0;
		else {
			auto* var = search_in_ctx<rvariant_stack_tag>(ctx);
			auto* croped_ctx = search_in_ctx<rvariant_crop_ctx_tag>(ctx);
			auto nctx = make_ctx<rvariant_crop_ctx_tag>(croped_ctx, *croped_ctx);
			return var->parse(nctx, src, result);
		}
	}
} rv_req{};
template<ascip_details::parser parser>
struct rvariant_top_result_parser : base_parser<rvariant_top_result_parser<parser>> { parser p; };




template<template<class>class wrapper, ascip_details::parser parser> constexpr static auto is_top_result_parser_helper(const wrapper<parser>& p) -> decltype(p.p);
template<typename parser>
constexpr static bool is_top_result_parser() {
	if constexpr (requires{ is_top_result_parser_helper(std::declval<parser>()); })
		return
			   is_top_result_parser<decltype(is_top_result_parser_helper(std::declval<parser>()))>()
			|| ascip_details::is_specialization_of<parser, rvariant_top_result_parser>;
	else return ascip_details::is_specialization_of<parser, rvariant_top_result_parser>;
}

template<typename maker_type, ascip_details::parser... parsers>
struct rvariant_parser : base_parser<rvariant_parser<maker_type, parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;
	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	template<auto ind> constexpr static bool is_term() {
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		auto checker = [](const auto* p){return std::is_same_v<std::decay_t<decltype(*p)>, std::decay_t<decltype(rv_lreq)>>;};
		return !exists_in((cur_parser_t*)nullptr, checker);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const requires ( ascip_details::is_in_concept_check(decltype(ctx){}) ) {
		return 0;
	}
	template<auto ind, auto cnt, auto cur, typename cur_parser, typename... tail>
	constexpr static auto _cur_ind() {
		constexpr const bool skip = is_top_result_parser<cur_parser>();
		if constexpr (ind == cnt) {
			if constexpr (skip) return -1;
			else return cur;
		}
		else return _cur_ind<ind,cnt+1,cur+(!skip),tail...>();
	}
	template<auto ind> consteval static auto cur_ind() {
		/*
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		if constexpr (is_top_result_parser<cur_parser_t>()) return -1;
		else {
			auto cur = 0;
			auto cnt = 0;
			(void)( ((ind==cnt)||(++cnt,cur+=!is_top_result_parser<parsers>(),false)) || ... );
			return cur;
		}
		*/
		return _cur_ind<ind,0,0,parsers...>();
	}
	template<auto ind> constexpr static auto& cur_result(auto& result) {
		if constexpr (cur_ind<ind>() == -1) return result;
		else if constexpr (requires{ create<1>(result); } ) return create<cur_ind<ind>()>(result);
		else if constexpr (requires{ result.template emplace<1>(); } ) return result.template emplace<cur_ind<ind>()>();
		else return result;
	}
	constexpr auto copy_result(auto& result) const {
		if constexpr (std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>) return result;
		else return maker(result);
	}
	template<auto ind> constexpr parse_result parse_term(auto&& ctx, auto src, auto& result) const {
		if constexpr (ind == 0) {
			if constexpr (is_term<ind>()) return get<ind>(seq).parse(ctx, src, result);
			else return -1;
		}
		else if constexpr (!is_term<ind>()) return parse_term<ind-1>(ctx, src, result);
		else {
			auto cur = get<ind>(seq).parse(ctx, src, cur_result<ind>(result));
			if(0 <= cur) return cur;
			if constexpr (ind==0) return cur;
			else return parse_term<ind-1>(ctx, src, result);
		}
	}
	template<auto ind, auto stop_pos>
	constexpr parse_result parse_nonterm(auto&& ctx, auto src, auto& result, auto shift) const {
		if(!src) return shift;
		if constexpr (ind < stop_pos) return shift;
		else if constexpr (is_term<ind>()) {
			if constexpr (ind == 0) return 0;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, shift);
		}
		else {
			ascip_details::type_any_eq_allow result_for_check;
			auto pr = get<ind>(seq).parse(ctx, src, result_for_check);
			decltype(pr) prev_pr = 0;
			while(0 < pr) {
				prev_pr += pr;
				auto cur = copy_result(result);
				if constexpr (!std::is_same_v<decltype(result), ascip_details::type_any_eq_allow&>)
					search_in_ctx<rvariant_copied_result_tag>(ctx) = &cur;
				src += get<ind>(seq).parse(ctx, src, cur_result<ind>(result));
				pr = get<ind>(seq).parse(ctx, src, result_for_check);
			}
			auto total_shift = shift + (prev_pr*(prev_pr>0));
			if constexpr (ind==0) return total_shift;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, total_shift);
		}
	}
	template<auto stop_pos>
	constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
		auto term_r = parse_term<sizeof...(parsers)-1>(ctx, src, result);
		if(term_r < 0) return term_r;
		auto nonterm_r = parse_nonterm<sizeof...(parsers)-1, stop_pos>(ctx, src += term_r, result, 0);
		return term_r + (nonterm_r*(nonterm_r>0));
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (exists_in_ctx<rvariant_stack_tag>(decltype(auto(ctx)){}))
			return parse_without_prep<0>(ctx, src, result);
		else {
			using copied_result_type = decltype(copy_result(result));
			auto nctx =
				make_ctx<rvariant_copied_result_tag>((copied_result_type*)nullptr,
				make_ctx<rvariant_stack_tag>(this, ctx));
			return parse_without_prep<0>(make_ctx<rvariant_crop_ctx_tag>(&nctx, nctx), src, result);
		}
	}
};




template<auto ind>
struct rvariant_mutator {
	struct context{};
	constexpr static auto create_ctx() { return context{}; }
	constexpr static auto create_ctx(auto&&,auto&&) { return context{}; }
	template<typename type>
	constexpr static auto apply(rvariant_rreq_pl_parser&&,auto&&) {
		return rvariant_rreq_parser<ind>{};
	}
};

constexpr static auto test_rvariant_simple(auto r, auto&& src, auto&&... parsers) {
	auto var = rv([](auto& r){return &r;}, parsers...);
	var.parse(make_test_ctx(), make_source(src), r);
	static_assert( var.template is_term<0>() );
	static_assert( var.template is_term<1>() );
	auto var_with_skip = inject_skipping(var, +space);
	static_assert( var_with_skip.template is_term<0>() );
	static_assert( var_with_skip.template is_term<1>() );
	return r;
}
template<typename dbl_expr>
constexpr static auto test_rvariant_val(auto r, auto&& maker, auto pr, auto&& src) {
	constexpr auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
	auto var = rv(std::forward<decltype(maker)>(maker)
		, cast<dbl_expr>(rv_lreq++ >> _char<'+'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> _char<'-'> >> rv_rreq(rmaker))
		, int_
		, cast<dbl_expr>(rv_lreq++ >> _char<'*'> >> rv_rreq(rmaker)) | cast<dbl_expr>(rv_lreq++ >> _char<'/'> >> rv_rreq(rmaker))
		, cast<dbl_expr>(rv_lreq++ >> lit<"**"> >> rv_rreq(rmaker))
		, fp
		, quoted_string
		, rv_result(_char<'('> >> rv_req >> _char<')'>)
		) ;
	auto cr = var.parse(make_test_ctx(), make_source(src), r);
	cr /= (cr == pr);

	auto var_with_skip = inject_skipping(var, +space);
	static_assert( !var_with_skip.template is_term<0>() );
	static_assert( !var_with_skip.template is_term<1>() );
	static_assert(  var_with_skip.template is_term<2>() );
	static_assert( !var_with_skip.template is_term<3>() );
	static_assert( !var_with_skip.template is_term<4>() );
	static_assert(  var_with_skip.template is_term<5>() );
	static_assert(  var_with_skip.template is_term<6>() );
	static_assert(  var_with_skip.template is_term<7>() );

	return r;
}

constexpr static bool test_rvariant_dexpr() {
	struct expr_rt;
	using term_rt = typename factory_t::template variant<int,decltype(mk_str())>;
	struct dbl_expr{
		typename factory_t::template unique_ptr<expr_rt> left;
		typename factory_t::template unique_ptr<expr_rt> right;
	};
	struct mul_expr : dbl_expr {} ;
	struct min_expr : dbl_expr {} ;
	struct pls_expr : dbl_expr {} ;
	struct div_expr : dbl_expr {} ;
	struct pow_expr : dbl_expr {} ;
	struct expr_rt : factory_t::template variant<
		 pls_expr, min_expr,
		 int,
		 typename factory_t::template variant<mul_expr, div_expr>,
		 pow_expr,
		 double, decltype(mk_str())
	       > {};
	constexpr auto pls_ind = 0;
	constexpr auto mul_ind = 3;
	constexpr auto pow_ind = 4;
	constexpr auto int_ind = 2;
	constexpr auto fp_ind = 5;

	constexpr auto maker = [](auto& r){ return typename factory_t::template unique_ptr<expr_rt>( new expr_rt{std::move(r)} ); };

	static_assert( test_rvariant_val<dbl_expr>(expr_rt{}, maker, -1, "").index() == int_ind, "if parser fails the variant have the last terminal index" );
	static_assert( get<int_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "123")) == 123 );
	static_assert( get<fp_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "0.5")) == 0.5 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1*5"))).left) == 1 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1*5"))).right) == 5 );
	static_assert( get<int_ind>(*get<1>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 3, "1/5"))).right) == 5 );
	static_assert( get<int_ind>(*get<pow_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 6, "1*5**2"))).right).right) == 2 );
	static_assert( get<int_ind>(*get<pow_ind>(*get<1>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 6, "1/5**2"))).right).right) == 2 );
	static_assert( get<int_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).right) == 7 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).left)).left) == 1 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(*get<pls_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 5, "1*5+7")).left)).right) == 5 );
	static_assert( get<int_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)"))).left) == 1 );
	static_assert( get<int_ind>(*get<pls_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)"))).right).left) == 5 );
	static_assert( get<int_ind>(*get<pls_ind>(*get<0>(get<mul_ind>(test_rvariant_val<dbl_expr>(expr_rt{}, maker, 7, "1*(5+7)"))).right).right) == 7 );


	return true;
}

constexpr static bool test_rvariant() {
	static_assert( test_rvariant_simple(int{}, "123", int_, fp) == 123 );
	static_assert( test_rvariant_simple(double{}, "1.2", int_, fp) == 1.2 );
	return test_rvariant_dexpr();
	return true;
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<typename type> struct forwarder{ type& o; constexpr forwarder(type& o) : o(o) {} };
template<typename type> forwarder(type&) -> forwarder<type>;
constexpr static auto fwd(auto& o) { return forwarder( o ); }

template<ascip_details::parser parser> struct unary_list_parser : base_parser<unary_list_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr unary_list_parser(unary_list_parser&&) =default ;
	constexpr unary_list_parser(const unary_list_parser&) =default ;
	constexpr unary_list_parser() =default ;
	constexpr unary_list_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return -1;
		auto ret = call_parse(ctx, src, result);
		src += ret * (0<=ret);
		auto cur_r = ret;
		while(src && 0<cur_r) {
			cur_r = call_parse(ctx, src, result);
			ret += cur_r * (0<=cur_r);
			src += cur_r * (0<=cur_r);
		}
		if(src) ascip_details::pop(result);
		return ret;
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = ascip_details::is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return p.parse(ctx, src, result.o);
		else return p.parse(ctx, src, ascip_details::empback(result));
	}
};

template<ascip_details::parser left, ascip_details::parser right>
struct binary_list_parser : base_parser<binary_list_parser<left, right>> {
	left lp;
	right rp;
	constexpr binary_list_parser(left l, right r) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow fake_result;
		auto ret = call_parse(ctx, src, result);
		if(ret<0) ascip_details::pop(result);
		auto cur = ret;
		while(cur > 0) {
			src += cur;
			auto r_res = rp.parse(ctx, src, fake_result);
			if( r_res <= 0 ) break;
			src += r_res;
			cur = call_parse(ctx, src, result);
			if( cur <= 0 ) {
				ascip_details::pop(result);
				break;
			}
			ret += cur + r_res;
		}
		return ret;
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = ascip_details::is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return lp.parse(ctx, src, result.o);
		else return lp.parse(ctx, src, ascip_details::empback(result));
	}
};





constexpr static bool test_unary_list() {
	test_parser_parse(mk_vec<char>(), *char_<'a'>, "", 0);
	static_assert(test_parser_parse(mk_vec<char>(), *char_<'a'>, "", 0).size() == 0);
	static_assert(test_parser_parse(mk_vec<char>(), *char_<'a'>, "aa", 2).size() == 2);
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), *char_<'a'>, "aa", 2), 'a', 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), +char_<'a'>, "aa", 2), 'a', 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), (+(char_<'a'> >> char_<'b'>))([](auto& r){return fwd(r);}), "abab", 4), 'a','b','a','b' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), (+(char_<'a'> >> char_<'b'>))([](auto& r){return fwd(r);}), "", -1) ));


	static_assert( ({char r='z';char_<'a'>.parse(make_test_ctx(),make_source('b'),r);r;}) == 'z' );
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), +(!char_<'a'>), "bb", 2), 0x00, 0x00 ),
			"!char_<'a'> parses but don't sore it's value, we have list with zeros (instead of infinit loop)");

	static_assert(test_cmp_vec( test_parser_parse(mk_str(),+(char_<'a'>|char_<'b'>), "aab", 3), 'a', 'a', 'b' ));


	return true;
}

constexpr static bool test_binary_list() {
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % ',', "a,b,c", 5), 'a', 'b', 'c' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % d10, "a1b2c", 5), 'a', 'b', 'c' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % d10, "a", 1), 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % d10, "a1", 1), 'a' ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), lower % d10, "A1", -1) ));
	static_assert(test_cmp_vec( test_parser_parse(mk_vec<char>(), ((lower >> lower) % ',')([](auto&r){return fwd(r);}), "ab,cd", 5), 'a','b','c','d' ));
	return true;
}

constexpr static bool test_lists() {
	return test_unary_list() && test_binary_list();
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

constexpr static auto call_err_method(auto& method, auto& ctx, auto src, auto& result, auto message) {
	if constexpr (requires{ { method(result,src,0,message) } -> std::same_as<void>; })
		return (method(
			result,
			src, //*search_in_ctx<seq_src_stack_tag>(ctx),
			search_in_ctx<ascip_details::new_line_count_tag>(ctx),
			message
			), -1);
	else return method(
			result,
			src, //*search_in_ctx<seq_src_stack_tag>(ctx),
			search_in_ctx<ascip_details::new_line_count_tag>(ctx),
			message
			);
}

struct seq_stack_tag{};
struct seq_src_stack_tag{};
struct seq_shift_stack_tag{};
struct seq_result_stack_tag{};
//TODO: dose we realy need the pos parser?
constexpr static struct cur_pos_parser : base_parser<cur_pos_parser> {
	constexpr parse_result parse(auto&&, auto src, auto& result) const {
		//TODO: extract the info from context or from parent's object
		//      sequence may sotre it in context
		//      sequence may have mutable field and
		//        pass it to parse method here or
		//        store it to result on it's own
		//      if use context - make it with tags
		ascip_details::eq(result, ascip_details::pos(src));
		return 0;
	}
} cur_pos{};
constexpr static struct cur_shift_parser : base_parser<cur_shift_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr (ascip_details::is_in_concept_check(decltype(auto(ctx)){})) return 0;
		else {
			ascip_details::eq(result, *search_in_ctx<seq_shift_stack_tag>(ctx));
			return 0;
		}
	}
} cur_shift{};
template<auto ind, auto ctx_chunk_size, auto ctx_result_pos>
struct seq_reqursion_parser : base_parser<seq_reqursion_parser<ind, ctx_chunk_size, ctx_result_pos>> {
	static_assert( ctx_chunk_size > ctx_result_pos, "we need to extract result from ctx"  );
	constexpr auto& extract_result(auto& ctx) const {
		return *by_ind_from_ctx<ind, seq_result_stack_tag>(ctx);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr( ascip_details::is_in_concept_check(decltype(auto(ctx)){})  ) return 0;
		else if constexpr (ascip_details::is_in_reqursion_check(decltype(auto(ctx)){})) {
			return !!src ? by_ind_from_ctx<ind, seq_stack_tag>(ctx)->parse(ctx, static_cast<decltype(src)&&>(src), extract_result(ctx)) : -1;
		} else {
			auto new_ctx = make_ctx<ascip_details::in_req_flag>(true, ctx);
			return !!src ? by_ind_from_ctx<ind, seq_stack_tag>(ctx)->parse(new_ctx, static_cast<decltype(src)&&>(src), extract_result(new_ctx)) : -1;
		}
	}
	constexpr auto parse_with_user_result(auto&& ctx, auto src, auto& result) const {
		if constexpr (ascip_details::is_in_reqursion_check(decltype(auto(ctx)){})) {
			return !!src ? by_ind_from_ctx<ind,seq_stack_tag>(ctx)->parse(ctx, static_cast<decltype(src)&&>(src), result) : -1;
		} else {
			auto new_ctx = make_ctx<ascip_details::in_req_flag>(true, ctx);
			return !!src ? by_ind_from_ctx<ind,seq_stack_tag>(new_ctx)->parse(new_ctx, static_cast<decltype(src)&&>(src), result) : -1;
		}
	}
};
template<auto ind> constexpr static auto req = seq_reqursion_parser<ind, 4, 1>{};

template<auto cnt, ascip_details::parser p> struct _seq_inc_rfield_val : p { constexpr static auto inc_val = cnt; };
template<auto cnt, ascip_details::parser p> struct _seq_num_rfield_val : p { constexpr static auto num_val = cnt; };
template<ascip_details::parser p> struct seq_inc_rfield_val : p { };
template<ascip_details::parser p> struct seq_num_rfield_val : p { };
template<typename type> constexpr static auto inc_field_val() {
	if constexpr (ascip_details::is_specialization_of<type, seq_inc_rfield_val>) return type::inc_val;
	else return 0;
}
template<typename type> constexpr static auto num_field_val() {
	if constexpr (ascip_details::is_specialization_of<type, seq_num_rfield_val>) return type::num_val;
	else return 0;
}

static struct seq_inc_rfield : base_parser<seq_inc_rfield> {constexpr parse_result parse(auto&&,auto,auto&)const {return 0;} } sfs ;
template<ascip_details::parser parser> struct seq_inc_rfield_after : base_parser<seq_inc_rfield_after<parser>> {
	constexpr seq_inc_rfield_after() =default ;
	constexpr seq_inc_rfield_after(seq_inc_rfield_after&&) noexcept =default ;
	constexpr seq_inc_rfield_after(const seq_inc_rfield_after&) noexcept =default ;
	constexpr explicit seq_inc_rfield_after(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_inc_rfield_before : base_parser<seq_inc_rfield_before<parser>> {
	constexpr seq_inc_rfield_before() =default ;
	constexpr seq_inc_rfield_before(seq_inc_rfield_before&&) noexcept =default ;
	constexpr seq_inc_rfield_before(const seq_inc_rfield_before&) noexcept =default ;
	constexpr explicit seq_inc_rfield_before(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_dec_rfield_after : base_parser<seq_dec_rfield_after<parser>> {
	constexpr seq_dec_rfield_after() =default ;
	constexpr seq_dec_rfield_after(seq_dec_rfield_after&&) noexcept =default ;
	constexpr seq_dec_rfield_after(const seq_dec_rfield_after&) noexcept =default ;
	constexpr explicit seq_dec_rfield_after(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_dec_rfield_before : base_parser<seq_dec_rfield_before<parser>> {
	constexpr seq_dec_rfield_before() =default ;
	constexpr seq_dec_rfield_before(seq_dec_rfield_before&&) noexcept =default ;
	constexpr seq_dec_rfield_before(const seq_dec_rfield_before&) noexcept =default ;
	constexpr explicit seq_dec_rfield_before(parser p) : p(std::move(p)) {}
	parser p;
};
template<typename p> seq_inc_rfield_after(p) -> seq_inc_rfield_after<p>;
template<typename p> seq_inc_rfield_before(p) ->  seq_inc_rfield_before<p>;
template<typename p> seq_dec_rfield_after(p) ->  seq_dec_rfield_after<p>;
template<typename p> seq_dec_rfield_before(p) -> seq_dec_rfield_before<p>;
template<typename concrete, typename... parsers> struct com_seq_parser : base_parser<concrete>, ascip_details::seq_tag {
	tuple<parsers...> seq;

	constexpr com_seq_parser() =default ;
	constexpr com_seq_parser(tuple<parsers...> t) : seq(std::move(t)) {}
	constexpr com_seq_parser(auto&&... args) requires (sizeof...(parsers) == sizeof...(args)) : seq( static_cast<decltype(args)&&>(args)... ) {}
	constexpr com_seq_parser(const com_seq_parser&) =default ;

	//TODO: make construction like --parser++ works as expected (decriment result field now and increment after this parser)
	template<typename type> constexpr static bool is_field_separator = requires(type&p){ static_cast<const seq_inc_rfield&>(p); };
	template<typename type> constexpr static bool is_inc_field_val = ascip_details::is_specialization_of<type, seq_inc_rfield_val>;
	template<typename type> constexpr static bool is_num_field_val = ascip_details::is_specialization_of<type, seq_num_rfield_val>;
	template<typename type> constexpr static bool is_inc_field_after = ascip_details::is_specialization_of<type, seq_inc_rfield_after>;
	template<typename type> constexpr static bool is_inc_field_before = ascip_details::is_specialization_of<type, seq_inc_rfield_before>;
	template<typename type> constexpr static bool is_dec_field_after = ascip_details::is_specialization_of<type, seq_dec_rfield_after>;
	template<typename type> constexpr static bool is_dec_field_before = ascip_details::is_specialization_of<type, seq_dec_rfield_before>;
	template<typename... types> constexpr static bool is_struct_requires = 
		((is_field_separator<types> + ...) + (is_inc_field_val<types> + ...) +
		 (is_inc_field_after<types> + ...) + (is_inc_field_before<types> + ...) +
		 (is_dec_field_after<types> + ...) + (is_dec_field_before<types> + ...) +
		 (is_num_field_val<types> + ...)
		) > 0;

	constexpr auto on_error(auto val) const { return static_cast<const concrete*>(this)->on_error(val); }

	template<auto find> constexpr auto call_parse(ascip_details::parser auto& p, auto&& ctx, auto src, auto& result) const requires is_struct_requires<parsers...> {
		if constexpr (is_field_separator<decltype(auto(p))>) return p.parse(ctx, src, result);
		else if constexpr ( std::is_same_v<std::decay_t<decltype(result)>, ascip_details::type_any_eq_allow> )
			return p.parse(ctx, src, result);
		else {
			return p.parse(ctx, src, ascip_reflection::get<find>(result));
		}
	}
	template<auto find> constexpr auto call_parse(ascip_details::parser auto& p, auto&& ctx, auto src, auto& result) const requires (!is_struct_requires<parsers...>) {
		return p.parse(ctx, src, result);
	}
	template<auto find> constexpr auto call_parse(auto& p, auto&& ctx, auto src, auto& result) const requires (!ascip_details::parser<decltype(auto(p))>) {
		auto& prev_src = *search_in_ctx<concrete>(ctx);
		return call_err_method(p, ctx, src, result, "unknown");
	}
	template<auto find, auto pind, typename cur_t, typename... tail> constexpr auto parse_seq(auto&& ctx, auto src, auto& result) const {
		//TODO: use -1 as last struct field, -2 as the field before last one and so on...
		constexpr const auto cur_field =
			( (find + is_inc_field_before<cur_t> + (-1*is_dec_field_before<cur_t>) + inc_field_val<cur_t>()) * !is_num_field_val<cur_t>)
			+ num_field_val<cur_t>();
		constexpr const auto nxt_field = cur_field + is_inc_field_after<cur_t> + (-1*is_dec_field_after<cur_t>) + is_field_separator<cur_t>;

		auto& cur = get<pind>(seq);
		auto ret = call_parse<cur_field>(cur, ctx, src, result);
		src += ret * (0 <= ret);
		//NOTE: check src and return  ret if no more data exists?
		*search_in_ctx<seq_shift_stack_tag>(ctx) += ret * (0 <= ret);
		if constexpr (pind+1 == sizeof...(parsers)) return ret; 
		else {
			if( ret < 0 ) return on_error(ret);
			auto req = parse_seq<nxt_field, pind+1, tail...>(ctx, src, result);
			if(req < 0) return on_error(req);
			return ret + req;
		}
	}

	template<auto find, auto pind>
	constexpr auto parse_and_store_shift(auto&& ctx, auto src, auto& result) const -> decltype(0) {
		//static_assert - exists concrete in ctx
		auto* old_shift = search_in_ctx<seq_shift_stack_tag>(ctx);
		auto cur_shift = 0;
		search_in_ctx<seq_shift_stack_tag>(ctx) = &cur_shift;
		auto ret = parse_seq<find, pind, parsers...>(static_cast<decltype(ctx)&&>(ctx), src, result);
		search_in_ctx<seq_shift_stack_tag>(ctx) = old_shift;
		return ret;
	}
	constexpr auto parse_with_modified_ctx(auto&& ctx, auto src, auto& result) const {
		const concrete* _self = static_cast<const concrete*>(this);
		ascip_details::type_any_eq_allow fake_r;
		auto shift_store = 0;
		auto cur_ctx = make_ctx<seq_shift_stack_tag>(&shift_store,
		      make_ctx<seq_src_stack_tag>(&src, 
			make_ctx<seq_result_stack_tag>(&result,
			  make_ctx<seq_stack_tag>(this,
			    ascip_details::make_ctx<concrete>(&src, ctx)
			  )
			)
		      )
		);
		return parse_and_store_shift<0,0>(cur_ctx, src, result);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return -1;
		if constexpr (ascip_details::is_in_concept_check(decltype(auto(ctx)){})) return 0;
		else {
			if constexpr (exists_in_ctx<concrete>(decltype(auto(ctx)){}))
				return parse_and_store_shift<0,0>(static_cast<decltype(ctx)&&>(ctx), src, result);
			else return parse_with_modified_ctx(static_cast<decltype(ctx)&&>(ctx), src, result);
		}
	}

	template<typename right> constexpr auto operator>>(const right& r)const{
		return ascip_details::init_with_get<opt_seq_parser<parsers..., right>>(seq, r); }
	template<ascip_details::parser right> constexpr auto operator>(const right& r)const{
		return operator>>(seq_error_parser<"unknown", right>{{}, r}); }
};
template<typename... parsers> struct opt_seq_parser : com_seq_parser<opt_seq_parser<parsers...>, parsers...> {
	using base_t = com_seq_parser<opt_seq_parser<parsers...>, parsers...>;
	constexpr opt_seq_parser() =default ;
	constexpr opt_seq_parser(opt_seq_parser&&) =default ;
	constexpr opt_seq_parser(const opt_seq_parser&) =default ;
	constexpr opt_seq_parser(auto&&... args) : base_t(static_cast<decltype(args)&&>(args)...) {}
	constexpr auto on_error(auto val) const { return val; }
};
template<typename... p> opt_seq_parser(p...) -> opt_seq_parser<std::decay_t<p>...>;

template<ascip_details::string_literal message, ascip_details::parser type>
struct seq_error_parser : base_parser<seq_error_parser<message,type>> {
	type p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(static_cast<decltype(ctx)&&>(ctx), src, result);
		auto err = search_in_ctx<ascip_details::err_handler_tag>(ctx);
		if constexpr (!requires{ (*err)(result, src, 0, message); }) return ret;
		else return call_err_method(*err, ctx, src, result, message);
	}
};

/***********
 * tests
 ***********/

constexpr static bool test_seq_simple_case() {
	constexpr auto p_ab = char_<'a'> >> char_<'b'>;
	static_assert( test_parser_parse_r_str(p_ab, "ab", 2, 'a', 'b') );
	static_assert( test_parser_parse_r_str(p_ab, "ac", -1, 'a'), "we store parsed part even on fail" );
	static_assert( test_parser_parse_r_str(p_ab, "abc", 2, 'a', 'b'), "don't catch a tail" );
	static_assert( test_parser_parse_r_str(p_ab, "cab", -1) );
	static_assert( test_parser_parse_r_str(any >> omit(space) >> any, "1 2", 3, '1', '2') );
	static_assert( test_parser_parse_r_str(char_<'a'> >> 'b', "ab", 2, 'a', 'b') );

	static_assert( ({ char r='z', l='a';
		(char_<'a'> >> char_<'b'> >> [&](auto& result, auto src, auto line, auto msg) {
		 src.ind /= (src.ind==2);
		 result /= (result=='b');
		 line /= (line==1);
		 l = 'u' * (result == 'b');
		 }).parse(make_test_ctx(), make_source("ab"), r);
	l;}) == 'u');
	static_assert(
		test_parser_parse_r_str(char_<'a'> >> char_<'b'> >> [](auto&&...){return 1;}, "ab", 3, 'a', 'b'),
		"lambda value is added to position" );

	return true;
}

constexpr static bool test_seq_result_fields() {
	struct with_2_chars { char a, b; };
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>, "ab", 2), 'a', 'b' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, ++char_<'a'> >> --char_<'b'>, "ab", 2), 'b', 'a' ) );
	static_cast<const opt_seq_parser<char_parser<'a'>, char_parser<'b'>, char_parser<'c'>>&>(char_<'a'> >> char_<'b'> >> char_<'c'>);
	static_cast<const opt_seq_parser<char_parser<'a'>, int_parser, char_parser<'c'>, int_parser>&>(char_<'a'> >> int_ >> char_<'c'> >> int_);
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>-- >> char_<'c'>, "abc", 3), 'c', 'b' ) );
	return true;
}
constexpr static bool test_seq_finc() {
	struct with_3_chars { char a, b, c; };
	static_assert( test_cmp_struct( test_parser_parse(with_3_chars{}, finc<2>(char_<'a'>) >> --char_<'b'> >> --char_<'c'>, "abc", 3), 'c', 'b', 'a' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_3_chars{}, ++char_<'a'> >> ++char_<'b'> >> finc<-2>(char_<'c'>), "abc", 3), 'c', 'a', 'b' ) );
	return true;
}
constexpr static bool test_seq_single_field() {
	struct with_1_field { decltype(mk_str()) val; };

	static_assert( test_parser_parse(with_1_field{}, fnum<0>(char_<'a'>) >> char_<'b'>, "ab", 2).val[0] == 'a' );
	static_assert( test_parser_parse(with_1_field{}, finc<0>(char_<'a'>) >> char_<'b'>, "ab", 2).val[1] == 'b' );
	static_assert( test_parser_parse(with_1_field{}, sfs >> --char_<'a'> >> char_<'b'>, "ab", 2).val[1] == 'b' );

	return true;
}
constexpr static bool test_seq_fnum() {
	struct with_2_chars { char a, b; };
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>++ >> fnum<0>(char_<'c'>), "abc", 3), 'c', 'b' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>++ >> fnum<1>(char_<'c'>), "abc", 3), 'a', 'c' ) );
	return true;
}
constexpr static bool test_seq_req() {
	constexpr auto ab_req = (char_<'a'>|'b') >> -(_char<'('> >> req<1> >> _char<')'>);
	static_assert( test_parser_parse(char{}, ab_req, "a(b)", 4) == 'b' );
	static_assert( test_parser_parse_r_str(ab_req, "a(b(a))", 7, 'a', 'b', 'a') );
	static_assert( test_parser_parse_r_str((char_<'a'>|'b'|'c') >> _char<'('> >> -req<0> >> _char<')'>, "a(b(c()))", 9, 'a', 'b', 'c') );

	struct semact_req_tester { char n='z'; semact_req_tester* ptr=nullptr; };
	static_assert( []{
		semact_req_tester r, r2; char ok='u';
		auto p=((char_<'a'>|'b')++ >> -(_char<'('> >> req<1>([&r2,&ok](auto& r){ok='o';return &r2;}) >> _char<')'>));
		p.parse(make_test_ctx(), make_source("a(b)"), r);
		return r.n * (r2.n == 'b') * (ok=='o');
	}() == 'a', "check semact for create value");
	static_assert( []{
		semact_req_tester r;
		auto p=(char_<'a'>++ >> -(omit(char_<'('>) >> req<1>([](auto& r){r=new semact_req_tester('b');return r;}) >> omit(char_<')'>)));
		p.parse(make_test_ctx(), make_source("a(a)"), r);
		char ret = r.ptr->n * (r.ptr->ptr == nullptr);
		delete r.ptr;
		return ret;
	}() == 'a', "check semact for create value");

	return true;
}
constexpr static bool test_seq_must() {

	static_assert(
		({char r='z';(char_<'a'> >> char_<'b'> >> must<"t">(char_<'c'>)).parse(make_test_ctx(), make_source("abd"), r);}) == -1,
		"if error and no lambda - nothing changed");
	static_assert( ({
		char r='z';
		const auto err_method = [&](...){return -10;};
		(char_<'a'> >> char_<'b'> >> must<"t">(char_<'c'>)).parse(make_test_ctx(&err_method), make_source("abe"), r);
	}) == -10, "error lambda are called");
	const auto err_method = [](
			auto& result,
			auto src_on_start,
			auto line_number,
			auto message){
		line_number /= (line_number==2);
		return (-3 * (message=="unknown")) + (-4 * (message=="test"));
	};
	static_assert( ({ char r=0x00;
		(any >> char_<'a'> >> char_<'b'> > char_<'c'>).parse(make_test_ctx(&err_method), make_source("\nabe"), r);
	}) == -3, "on error: sources are on start sequence and on rule where the error");
	static_assert( ({ char r=0x00;
		(any >> char_<'a'> >> char_<'b'> >> must<"test">(char_<'c'>)).parse(make_test_ctx(&err_method), make_source("\nabe"), r);
	}) == -4, "on error: sources are on start sequence and on rule where the error");

	return true;
}
constexpr static bool test_seq_shift_pos() {
	static_assert( []{
		struct { char a='z', b='z'; int pos=-1; } r;
		(char_<'a'> >> ++char_<'b'> >> ++cur_pos).parse(make_test_ctx(), make_source("ab"), r);
		return r.pos;
	}() == 2, "can parse current position");
	static_assert( std::is_same_v<decltype(auto(char_<'a'> >> char_<'b'>)), decltype(auto(char_<'a'> >> char_<'b'>))>,
			"even if it different declarations it's the same parser and we can use one instead of other instance" );
	constexpr auto abcd = char_<'a'>++ >> char_<'b'>++ >> (char_<'c'> >> ++char_<'d'> >> ++cur_shift) >> ++cur_shift;
	static_assert( [&]{
		struct { char a='z', b='z'; struct { char c,d; int shift1;} i; int shift2; } r;
		abcd.parse(make_test_ctx(), make_source("abcd"), r);
		return r.i.shift1 + r.shift2;
	}() == 6, "can parse current shift");
	constexpr auto ab_req = (char_<'a'>|'b')++ >> -(_char<'('> >> req<1> >> _char<')'> >> ++cur_shift) >> ++cur_shift;
	static_assert( [&]{
		struct { char a='z'; struct { char b; int shift1;} i; int shift2; } r;
		ab_req.parse(make_test_ctx(), make_source("a(b)"), r);
		return r.i.shift1;
	}() == 3, "can parse current shift in reqursion");
	static_assert( [&]{
		struct { char a='z'; struct { char b; int shift1;} i; int shift2; } r;
		ab_req.parse(make_test_ctx(), make_source("a(b)"), r);
		return r.shift2;
	}() == 4, "can parse current shift in reqursion");
	return true;
}


constexpr static bool test_seq() {
	return
	     test_seq_simple_case()
	  && test_seq_result_fields()
	  && test_seq_single_field()
	  && test_seq_finc()
	  && test_seq_fnum()
	  && test_seq_req()
	  && test_seq_must()
	  && test_seq_shift_pos()
	  ;
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


/***********
 * transformation
 ***********/

template<typename mutator>
constexpr static auto transform_apply(auto&& src, auto& ctx) {
	if constexpr (requires{ mutator::template apply<std::decay_t<decltype(src)>>( std::forward<decltype(src)>(src), ctx ); }) {
		return mutator::template apply<std::decay_t<decltype(src)>>( std::forward<decltype(src)>(src), ctx );
	}
	else return src;
}

template<typename mutator, template<typename...>class result_t, auto... ind, template<typename...>class i_tuple, typename... tail>
constexpr static auto transform_apply_to_each(i_tuple<tail...>&& src, auto& ctx, auto&&... args) {
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
template<typename mutator, auto val, typename type>
constexpr static auto transform_special(_seq_inc_rfield_val<val,type>&& src, auto& ctx) {
	return transform_apply<mutator>(finc<val>(transform<mutator>(static_cast<type&&>(src), ctx)), ctx);
}
template<typename mutator, auto val, typename type>
constexpr static auto transform_special(_seq_num_rfield_val<val,type>&& src, auto& ctx) {
	return transform_apply<mutator>(fnum<val>(transform<mutator>(static_cast<type&&>(src), ctx)), ctx);
}
template<typename mutator, typename type, typename parser>
constexpr static auto transform_special(cast_parser<type,parser>&& src, auto& ctx) {
	return transform_apply<mutator>(cast<type>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
template<typename mutator, typename type, typename parser>
constexpr static auto transform_special(result_checker_parser<type,parser>&& src, auto& ctx) {
	return transform_apply<mutator>(check<type>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
template<typename mutator, typename parser, typename value_type>
constexpr static auto transform_special(as_parser<value_type, parser>&& src, auto&& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( as_parser{ src.val, std::move(np) }, nctx );
}
template<typename mutator, typename parser, auto value>
constexpr static auto transform_special(tmpl_as_parser<value, parser>&& src, auto&& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( tmpl_as_parser<value, std::decay_t<decltype(np)>>{ std::move(np) }, nctx );
}

template<typename mutator>
constexpr static auto transform(auto&& src) {
	auto ctx = mutator::create_ctx();
	return transform<mutator>(std::forward<decltype(src)>(src), ctx);
}

template<typename mutator>
constexpr static auto transform(auto&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	if constexpr(requires{ transform_special<mutator>(std::move(src), ctx); })
		return transform_special<mutator>(std::move(src), nctx);
	else return transform_apply<mutator>(std::forward<decltype(src)>(src), nctx);
}
template<typename mutator, template<ascip_details::parser>class wrapper, ascip_details::parser inner>
constexpr static auto transform(wrapper<inner>&& src, auto& ctx) requires (requires{ src.p; } && !requires{transform_special<mutator>(std::move(src), ctx);}){
	auto nctx = mutator::create_ctx(src, ctx);
	auto mp = transform<mutator>(std::move(src.p), nctx);
	if constexpr(requires{ wrapper{{}, std::move(mp)}; })
		return transform_apply<mutator>(wrapper{{}, std::move(mp)}, nctx);
	else 
		return transform_apply<mutator>(wrapper{std::move(mp)}, nctx);
}
template<typename mutator, typename left_type, typename right_type>
constexpr static auto transform(different_parser<left_type, right_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto lp = transform<mutator>(std::move(src.lp), nctx);
	auto rp = transform<mutator>(std::move(src.rp), nctx);
	return transform_apply<mutator>(different_parser( std::move(lp), std::move(rp) ), nctx);
}
template<typename mutator, typename left_type, typename right_type>
constexpr static auto transform(binary_list_parser<left_type, right_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto lp = transform<mutator>(std::move(src.lp), nctx);
	auto rp = transform<mutator>(std::move(src.rp), nctx);
	return transform_apply<mutator>(binary_list_parser( std::move(lp), std::move(rp) ), nctx);
}
template<typename mutator, template<typename...>class seq_parser, typename... list>
constexpr static auto transform(seq_parser<list...>&& src, auto& ctx) requires requires{ src.seq; } {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>(transform_apply_to_each<mutator, seq_parser, 0>(std::move(src.seq), nctx), nctx);
}
template<typename mutator, typename type, typename... parsers>
constexpr static auto transform(rvariant_parser<type, parsers...>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>( transform_apply_to_each<mutator,rvariant_parser,0>(std::move(src.seq),nctx,std::move(src.maker)), nctx );
}
template<typename mutator, typename parser, typename act_t>
constexpr static auto transform(semact_parser<parser, act_t>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return semact_parser<std::decay_t<decltype(np)>, std::decay_t<decltype(src.act)>>{ {}, std::move(src.act), std::move(np) };
}


/********
 * transform tests
 ********/

struct transform_noncopyable{
	constexpr transform_noncopyable(){}
	constexpr transform_noncopyable(const transform_noncopyable&) =delete ;
	constexpr transform_noncopyable& operator=(const transform_noncopyable&) =delete ;
};
struct test_parser : base_parser<test_parser>{constexpr auto parse(auto&& ctx,auto src, auto&)const{return 0;}};
struct test_parser2 : base_parser<test_parser>{constexpr auto parse(auto&& ctx,auto src, auto&)const{return 0;}};
template<typename pt> struct test_wrapper : base_parser<test_wrapper<pt>>{ pt p; };
template<auto ind,typename pt> struct transform_val_p : base_parser<transform_val_p<ind,pt>>{ pt p; };
struct t1_to_t2_mutator {
	struct context{ };
	struct in_lexeme_context{ };
	constexpr static auto create_ctx(){ return context{}; }
	constexpr static auto create_ctx(auto&& src, auto&& ctx) {
		if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, lexeme_parser>)
			return in_lexeme_context{ };
		else if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, skip_parser>)
			return context{};
		else return ctx;
	}
	template<typename type>
	constexpr static auto apply(test_parser&& p, auto& ctx) {
		if constexpr (std::is_same_v<in_lexeme_context,std::decay_t<decltype(ctx)>>)
			return p;
		else return test_parser2{};
	}
	template<typename type, typename inner>
	constexpr static auto apply(test_wrapper<inner>&& p, auto&) {
			return p.p;
	}
};

constexpr static auto test_transform_t_to_p(auto&& parser) {
	return transform<t1_to_t2_mutator>(std::forward<decltype(parser)>(parser));
}
constexpr static bool test_transform_modify_leaf() {

	static_assert( !requires(const transform_noncopyable& nc, transform_noncopyable& n){ n = nc; } );
	static_assert( std::is_same_v<test_parser2, decltype(test_transform_t_to_p(test_parser{}))> );
	static_assert( std::is_same_v<test_parser2, decltype(test_transform_t_to_p(test_parser2{}))> );
	static_assert( std::is_same_v<skip_parser<test_parser2>, decltype(test_transform_t_to_p(skip(test_parser{})))> );
	static_assert( std::is_same_v<skip_parser<test_parser2>, decltype(test_transform_t_to_p(skip(skip(test_parser{}))))> );
	static_assert( std::is_same_v<skip_parser<char_parser<'a'>>, decltype(test_transform_t_to_p(skip(char_<'a'>)))> );
	static_assert( std::is_same_v<lexeme_parser<test_parser>, decltype(test_transform_t_to_p(lexeme(test_parser{})))> );
	static_assert( std::is_same_v<lexeme_parser<skip_parser<test_parser2>>, decltype(test_transform_t_to_p(lexeme(skip(test_parser{}))))> );
	static_assert( std::is_same_v<opt_seq_parser<char_parser<'a'>, test_parser2>, decltype(test_transform_t_to_p(char_<'a'> >> test_parser{}))> );
	static_assert( std::is_same_v<skip_parser<test_parser2>, decltype(test_transform_t_to_p(skip(test_wrapper{ {}, test_parser{} })))> );
	static_assert( ({ char r; transform_noncopyable nc;
		auto p = char_<'a'> >> [&nc](...){ return 0; };
		test_transform_t_to_p(p).parse(make_test_ctx(), make_source("a"), r);
	r;}) == 'a' );
	static_assert( std::is_same_v<seq_inc_rfield_val<_seq_inc_rfield_val<1,test_parser2>>, decltype(test_transform_t_to_p(finc<1>(test_parser{})))> );
	static_assert( std::is_same_v<seq_num_rfield_val<_seq_num_rfield_val<1,test_parser2>>, decltype(test_transform_t_to_p(fnum<1>(test_parser{})))> );
	static_assert( std::is_same_v<cast_parser<int,test_parser2>, decltype(test_transform_t_to_p(cast<int>(test_parser{})))> );
	static_assert( std::is_same_v<result_checker_parser<int,test_parser2>, decltype(test_transform_t_to_p(check<int>(test_parser{})))> );

	auto rv_maker = []{};
	static_assert( std::is_same_v<
			rvariant_parser<decltype(rv_maker),char_parser<'a'>,test_parser2>,
			decltype(test_transform_t_to_p(rv(rv_maker,char_<'a'>,test_parser{})))> );
	static_assert( std::is_same_v<
			result_checker_parser<int,cast_parser<char,opt_seq_parser<char_parser<'a'>,test_parser2>>>,
			decltype(test_transform_t_to_p(check<int>(cast<char>(char_<'a'> >> test_parser{})))) >);
	static_assert( std::is_same_v<
			semact_parser<test_parser2, decltype(rv_maker)>,
			decltype( test_transform_t_to_p(test_parser{}(rv_maker)) )
			> );
	static_assert( std::is_same_v<
			seq_inc_rfield_before<semact_parser<test_parser2, decltype(rv_maker)>>,
			decltype( test_transform_t_to_p(++test_parser{}(rv_maker)) )
			> );
	static_assert( std::is_same_v<
			seq_inc_rfield_after<semact_parser<test_parser2, decltype(rv_maker)>>,
			decltype( test_transform_t_to_p(test_parser{}(rv_maker)++) )
			> );

	return true;
}
constexpr static bool test_transform_modify_leaf_with_cond() {
	return true;
}
constexpr static bool test_transform() {
	return 
		   test_transform_modify_leaf()
		&& test_transform_modify_leaf_with_cond()
		&& test_exists_in()
		;
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

template<ascip_details::parser pt> struct lexeme_parser : base_parser<lexeme_parser<pt>> { [[no_unique_address]] pt p; 
	constexpr lexeme_parser() =default ;
	constexpr lexeme_parser(lexeme_parser&&) =default ;
	constexpr lexeme_parser(const lexeme_parser&) =default ;
	constexpr lexeme_parser(pt p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const { return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result); }
};
template<ascip_details::parser pt> struct skip_parser : base_parser<lexeme_parser<pt>> { [[no_unique_address]] pt p; 
	constexpr skip_parser() =default ;
	constexpr skip_parser(skip_parser&&) =default ;
	constexpr skip_parser(const skip_parser&) =default ;
	constexpr skip_parser(pt p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const { return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result); }
};
template<ascip_details::parser skip, ascip_details::parser base> struct injected_parser : base_parser<injected_parser<skip,base>> {
	[[no_unique_address]] skip s;
	[[no_unique_address]] base b;
	constexpr injected_parser() noexcept =default ;
	constexpr injected_parser(injected_parser&&) noexcept =default ;
	constexpr injected_parser(const injected_parser&) noexcept =default ;
	constexpr injected_parser(skip s, base b) : s(std::forward<decltype(s)>(s)), b(std::forward<decltype(b)>(b)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		ascip_details::type_any_eq_allow skip_result;
		auto sr = s.parse(ctx, src, skip_result);
		sr *= (0<=sr);
		src += sr;
		auto mr = src ? b.parse(ctx, src, result) : -1;
		return (sr * (0<=mr)) + mr; // 0<=mr ? mr+sr : mr;
	}

	constexpr auto parse_with_user_result(auto&& ctx, auto src, auto& result) const
	requires requires(const base& p){ p.parse_with_user_result(ctx, src, result); } {
		return b.parse_with_user_result(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}
};
template<typename t> lexeme_parser(t) -> lexeme_parser<t>;
template<typename t> skip_parser(t) -> skip_parser<t>;
template<typename s, typename b> injected_parser(s,b) -> injected_parser<s,b>;

template<typename skip_type>
struct injection_mutator {
	struct context{ };
	struct in_lexeme_context{ };
	constexpr static auto create_ctx(){ return context{}; }
	constexpr static auto create_ctx(auto&& src, auto&& ctx) {
		if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, lexeme_parser>)
			return in_lexeme_context{ };
		else if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, skip_parser>)
			return context{};
		else return ctx;
	}

	template<typename type>
	constexpr static auto apply(auto&& p, auto& ctx) {
		constexpr const bool is_inside_lexeme = std::is_same_v<in_lexeme_context,std::decay_t<decltype(ctx)>>;
		constexpr const bool is_parser_lexeme = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, lexeme_parser>;
		constexpr const bool is_parser_skip = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, skip_parser>;

		constexpr const bool is_parser_variant = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, variant_parser>;
		constexpr const bool is_parser_rvariant = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, rvariant_parser>;
		constexpr const bool is_parser_blist = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, binary_list_parser>;
		constexpr const bool is_parser_diff = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, different_parser>;
		constexpr const bool is_opt_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, opt_seq_parser>;
		constexpr const bool is_num_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, seq_num_rfield_val>;
		constexpr const bool is_inc_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, seq_inc_rfield_val>;

		constexpr const bool is_parser_for_skip =
			   is_opt_seq_parser
			|| is_parser_variant
			|| is_parser_rvariant
			|| is_parser_blist
			|| is_parser_diff
			|| is_num_seq_parser
			|| is_inc_seq_parser
			;

		if constexpr (is_parser_lexeme)
			return injected_parser<skip_type, std::decay_t<decltype(p.p)>>( skip_type{}, std::move(p.p) );
		else if constexpr (is_inside_lexeme || is_parser_for_skip) return p;
		else if constexpr (is_parser_skip) return p.p;
		else if constexpr ( requires{ p.p; }) return p;
		else return injected_parser<skip_type, std::decay_t<decltype(p)>>( skip_type{}, std::move(p) );
	}
};

template<ascip_details::parser parser, ascip_details::parser skipper>
constexpr static auto inject_skipping(parser&& to, skipper&& what) {
	using mutator = injection_mutator<std::decay_t<skipper>>;
	return transform<mutator>(std::move(to));
}
template<ascip_details::parser p1, ascip_details::parser p2> constexpr static auto make_injected(const p1& l, const p2& r) {
	return typename p1::holder::template injected_parser<p1, p2>( l, r ); }

constexpr static bool test_injection_parser() {

	static_assert( ({ char r='z'; make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source(" a"), r);
	}) == 2, "inejction parser can parse");
	static_assert( ({ char r='z'; make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source("  b"), r);
	}) == -1, "inejction parser cannot parse invalid data");
	static_assert( ({ char r='z';
		make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source(" a"), r);
	r;}) == 'a', "the first parameter of inejction parser skipped");
	static_assert( ({ char r='z';
		make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source("ba"), r);
	}) == -1, "inejction parser fails if cannot parse first parameter");
	static_assert( []{ char r='z';
		auto pr=make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source("a"), r);
		return (pr==1) + (2*(r=='a'));}() == 3, "inejction parser parse if only first parameter fails");
	static_assert( []{ char r='z';
		auto pr=make_injected(char_<' '>, char_<'a'>).parse(make_test_ctx(), make_source("aa"), r);
		return (pr==1) + (2*(r=='a'));}() == 3, "inejction parser parse if only first parameter fails");

	return true;
}
template<auto p1, auto p2>
constexpr static bool test_seq_injection() {
	using p1_t = decltype(auto(p1));
	using p2_t = decltype(auto(p2));
	using inj_t = injected_parser<p2_t,p1_t>;

	(void)static_cast<const inj_t&>(inject_skipping(p1, p2));
	(void)static_cast<const opt_seq_parser<inj_t, inj_t>&>(inject_skipping(p1 >> p1, p2));

	(void)static_cast<const injected_parser<p2_t,opt_seq_parser<p1_t, p1_t, p1_t>>&>(inject_skipping(lexeme(p1 >> p1 >> p1), p2));
	(void)static_cast<const injected_parser<p2_t,opt_seq_parser<p1_t, opt_seq_parser<inj_t, inj_t>>>&>(
			inject_skipping(lexeme(p1 >> skip(p1 >> p1)), p2));

	(void)static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping( p1|p1, p2 ));
	(void)static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping( p1|p1|p1, p2 ));
	(void)static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		cast_parser<char,inj_t>
		>&>(inject_skipping( check<char>(p1)|cast<char>(p1), p2 ));

	(void)static_cast<const unary_list_parser<inj_t>&>(inject_skipping( +p1, p2 ));
	(void)static_cast<const unary_list_parser<opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( +(p1>>p1), p2 ));

	(void)static_cast<const opt_parser<inj_t>&>(inject_skipping( -p1, p2 ));
	(void)static_cast<const opt_parser<opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( -(p1>>p1), p2 ));

	(void)static_cast<const different_parser<inj_t, inj_t>&>(inject_skipping( p1 - p1, p2 ));
	(void)static_cast<const opt_seq_parser<
		inj_t,
		different_parser<
		  opt_seq_parser<inj_t,inj_t>,
		  inj_t
		>
		>&>(inject_skipping( p1 >> ((p1>>p1) - p1), p2 ));


	static_assert( ({ char r='z'; int t=0;
		auto p = char_<'a'> >> char_<'b'> >> [&t](...){++t;return 0;};
		p.parse(make_test_ctx(), make_source("ab"), r);
	t;}) == 1, "injection works win lambda in seq");
	static_assert( ({ char r='z'; int t=0;
		auto p = char_<'a'> >> char_<'b'>([&t](...){++t;});
		p.parse(make_test_ctx(), make_source(mk_str("ab")), r);
	t; }) == 1, "injection works with semact" );
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping(char_<'a'> >> char_<'b'>([&t](...){++t;}), +space);
		p.parse(make_test_ctx(), make_source(mk_str("ab")), r);
	t; }) == 1, "injection works with semact" );
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping(char_<'a'> >> cast<char>(char_<'b'>([&t](...){++t;})), +space);
		p.parse(make_test_ctx(), make_source(mk_str("abc")), r);
	t; }) == 1, "injection works with semact" );


	(void)static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping( p1|p1, p2 ));
	(void)static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping( p1|p1|p1, p2 ));
	(void)static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		result_checker_parser<char,inj_t>
		>&>(inject_skipping( check<char>(p1)|check<char>(p1), p2 ));

	(void)static_cast<const result_checker_parser<int, inj_t>&>(inject_skipping( check<int>(p1), p2 ));
	(void)static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( check<int>(p1 >> p1), p2 ));
	(void)static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,seq_num_rfield_val<_seq_num_rfield_val<2,inj_t>>>>&>(inject_skipping( check<int>(p1 >> fnum<2>(p1)), p2 ));
	{
		auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
		auto var = rv(rmaker, p1, p1);
		(void)static_cast<rvariant_parser<decltype(rmaker), p1_t, p1_t>&>(var);
		(void)static_cast<const rvariant_parser<decltype(rmaker), inj_t, inj_t>&>(inject_skipping(var, p2));
	}


	static_assert( ({ char r; const auto parser = +alpha; const auto skipper = +space;
		inject_skipping(parser, skipper).parse(make_test_ctx(), make_source(" a b c "), r); }) == -1 );

	(void)static_cast<const binary_list_parser<inj_t, inj_t>&>(inject_skipping( p1 % p1, p2 ));

	return true;
}
constexpr static bool test_injection() {
	return test_injection_parser() && test_seq_injection<char_<'a'>,space>();
}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


constexpr static auto dquoted_string = lexeme(_char<'"'> >> *(as<'"'>(char_<'\\'> >> char_<'"'>)| (ascip::any - char_<'"'>)) >> _char<'"'>);
constexpr static auto squoted_string = lexeme(_char<'\''> >> *(as<'\''>(char_<'\\'> >> char_<'\''>)| (ascip::any - char_<'\''>)) >> _char<'\''>);
constexpr static auto quoted_string = lexeme(squoted_string | dquoted_string);

constexpr static bool integrated_tests() {
	static_assert( test_parser_parse_r_str(quoted_string, "'ok'", 4, 'o', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "'o\\'k'", 6, 'o', '\'', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "' o\\'k'", 7, ' ', 'o', '\'', 'k') );

	static_assert( test_parser_parse_r_str(quoted_string, "\"ok\"", 4, 'o', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "\"o'k\"", 5, 'o', '\'', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "\" o'k\"", 6, ' ', 'o', '\'', 'k') );
	static_assert( test_parser_parse_r_str(quoted_string, "\" o\\\"k\"", 7, ' ', 'o', '"', 'k') );


	static_assert( ({ auto r = mk_str(); parse(quoted_string, +space, make_source("'ok'"), r); }) == 4);
	static_assert( ({ auto r = mk_str(); parse(+alpha, +space, make_source("a b c "), r); }) == 5);
	static_assert( ({ auto r = mk_str(); parse(+alpha, +space, make_source(" a b c "), r); }) == 6);
	static_assert( ({ auto r = mk_str(); parse((as(char_<'a'>, 'b')|char_<'c'>) % int_, +space, make_source("c1  a2 a  3 a 3a"), r); }) == 16);
	static_assert( ({ auto r = mk_str(); parse((as<'b'>(char_<'a'>)|char_<'c'>) % int_, +space, make_source("c1  a2 a  3 a 3a"), r); }) == 16);

	static_assert( ({ auto r = mk_str(); auto ctx = make_test_ctx();
		quoted_string.parse(ctx, make_source("'1\n2\n'"), r);
		search_in_ctx<ascip_details::new_line_count_tag>(ctx);}) == 3);

	return true;
}

constexpr static void test() {



	static_assert( ascip_details::parser<decltype(char_<'b'>)> );

	test_sources();
	test_parser_char();
	test_parser_value();
	static_assert( space.test() );
	static_assert( any.test() );
	static_assert( int_.test() );
	static_assert( fp.test() );
	static_assert( fp.test() );
	static_assert( test_literal_parser() );
	static_assert( test_variant() );
	static_assert( test_rvariant() );
	static_assert( test_range_parser() );
	static_assert( test_negate() );
	static_assert( test_optional() );
	static_assert( test_omit() );
	static_assert( test_as() );
	static_assert( test_lists() );
	static_assert( test_checkers() );
	static_assert( test_different() );
	static_assert( test_semact() );
	static_assert( test_seq() );
	static_assert( test_injection() );
	static_assert( integrated_tests() );
	static_assert( test_transform() );
}

template<auto sym> struct tmpl {
	constexpr static auto& fp = holder::fp;
	constexpr static auto& char_ = holder::char_<sym>;
	constexpr static auto& _char = holder::_char<sym>;
	constexpr static auto& space = holder::space;
	constexpr static auto& any = holder::any;
	constexpr static auto& int_ = holder::int_;
	constexpr static auto& lower = holder::lower;
	constexpr static auto& upper = holder::upper;
	constexpr static auto& alpha = holder::alpha;
	constexpr static auto& digit = holder::digit;
	constexpr static auto& d10 = holder::d10;
	constexpr static auto& ascii = holder::ascii;
	constexpr static auto& cur_pos = holder::cur_pos;
	constexpr static auto& cur_shift = holder::cur_shift;
	constexpr static auto& req = holder::req<sym>;
	constexpr static auto& rv_lreq = holder::rv_lreq;
	constexpr static auto& rv_rreq = holder::rv_rreq;
	constexpr static auto& nl = holder::nl;
	constexpr static auto& quoted_string = holder::quoted_string;
	constexpr static auto& dquoted_string = holder::dquoted_string;
	constexpr static auto& squoted_string = holder::squoted_string;
};
template<auto sym> struct term : tmpl<sym> {};
// c++ cannot use auto as string_literal
template<ascip_details::string_literal v> struct sterm {
	constexpr static auto& lit = holder::lit<v>;
};

}; // struct ascip (context)

} // namespace <anonymus>






template<typename factory_t, template<typename...>class tuple>
struct ascip_literals {
template<typename char_t, char_t... chars>
friend constexpr auto operator""_lex() {
	return lexeme( omit( (... >> ascip<tuple, factory_t>::template char_<chars>) ) );
}
}; // namespace ascip_literals

