namespace ascip_details {

template<typename type, template<typename...>class tmpl> constexpr bool is_specialization_of = false;
template<template<typename...>class type, typename... args> constexpr bool is_specialization_of<type<args...>, type> = true;

template<typename...> struct type_list {};
template<typename,auto...> struct seq_type {};
template<typename t> struct type_holder { using type = t; t operator+() const ; };
template<typename t> constexpr auto type_c = type_holder<t>{};
template<typename t> constexpr auto type_dc = type_holder<std::decay_t<t>>{};
template<typename l, typename r> constexpr bool operator==(type_holder<l>, type_holder<r>) { return false; }
template<typename t> constexpr bool operator==(type_holder<t>, type_holder<t>) { return true; }
template<typename type, typename... list> constexpr bool contains(type_list<list...>, type_holder<type> s) {
	return ((s == type_c<list>) + ... );
}
template<typename... in_list, typename... what> constexpr bool contains_all(type_list<in_list...> in, type_list<what...>) {
	return (contains(in, type_c<what>) * ...);
}
template<typename... in_list, typename... what> constexpr bool contains_any(type_list<in_list...> in, type_list<what...>) {
	return (contains(in, type_c<what>) + ...);
}

static_assert( contains(type_list<int,char>{}, type_c<int>) );
static_assert( !contains(type_list<int,char>{}, type_c<double>) );
static_assert( contains_all(type_list<int,char,double>{}, type_list<int>{}) );
static_assert( contains_all(type_list<int,char,double>{}, type_list<int,double>{}) );
static_assert( !contains_all(type_list<int,char,double>{}, type_list<int,unsigned>{}) );
static_assert( contains_any(type_list<int,char,double>{}, type_list<int,unsigned>{}) );
static_assert( !contains_any(type_list<int,char,double>{}, type_list<long,unsigned>{}) );

template<typename type, auto ind> struct tuple_value {
	type value; //NOTE: we cannot downcast for some reason in get method later, so we need in the fucky g methods
	template<auto i> constexpr type& g() requires (i==ind) { return value; }
	template<auto i> constexpr const type& g() const requires (i==ind) { return value; }
};
template<typename... types> struct tuple {
	consteval static auto mk_storage_type() {
		return []<typename fucky_clang,auto... inds>(seq_type<fucky_clang,inds...>){
			struct storage : tuple_value<types, inds>... { using tuple_value<types, inds>::g...; };
			return type_holder<storage>{};
		}(

			seq_type<unsigned, __integer_pack(sizeof...(types))...>{}



		);
	}

	using storage_type = decltype(+mk_storage_type());
	storage_type storage;

	constexpr tuple() =default ;
	constexpr tuple(tuple&& other) : storage(std::move(other.storage)) {}
	constexpr tuple(const tuple& other) : storage(other.storage) {}
	constexpr explicit tuple(auto&&... args) : storage{std::forward<decltype(args)>(args)...} {}

	template<auto ind> constexpr friend auto& get(tuple& t) { return t.storage.template g<ind>(); }
	template<auto ind> constexpr friend const auto& get(const tuple& t) { return t.storage.template g<ind>(); }
	friend constexpr auto size(const tuple&) { return sizeof...(types); }
};

template<typename... types> tuple(types&&...) -> tuple<std::decay_t<types>...>;

} // namespace ascip_details
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details {

template<typename type> concept vector = requires(const type& v){ v.size(); v[0]; v.back(); } && requires(type& v){ v.pop_back(); };
template<typename type> concept string = requires(type& s){ s += typename type::value_type{}; } && vector<type>;
template<typename type> concept string_view = requires(const type& s){ s.size(); s[0]; s.back(); };
template<typename type> concept empbackable = requires(type& r){ emplace_back(r); } || requires(type& r){ r.emplace_back(); };

} // namespace ascip_details

namespace ascip_details {

struct type_any_eq_allow { };

struct type_check_parser : type_any_eq_allow {
	template<typename type> constexpr auto& operator =(const type&){ return *this; }
	template<typename type> constexpr auto& operator+=(const type&){ return *this; }
	template<typename type> constexpr auto& operator-=(const type&){ return *this; }
	template<typename type> constexpr auto& operator/=(const type&){ return *this; }
	template<typename type> constexpr auto& operator*=(const type&){ return *this; }
};
struct type_parse_without_result : type_any_eq_allow {
	template<typename type> constexpr auto& operator =(const type&){ return *this; }
	template<typename type> constexpr auto& operator+=(const type&){ return *this; }
	template<typename type> constexpr auto& operator-=(const type&){ return *this; }
	template<typename type> constexpr auto& operator/=(const type&){ return *this; }
	template<typename type> constexpr auto& operator*=(const type&){ return *this; }
};
struct result_check_ok_for_clang { constexpr static bool ok = true; };

constexpr auto is_checking(const auto&) { return false; }
constexpr auto is_checking(const type_check_parser&) { return result_check_ok_for_clang{}; }
constexpr auto is_parsing_without_result(const auto&) { return false; }
constexpr auto is_parsing_without_result(const type_parse_without_result&) { return result_check_ok_for_clang{}; }

constexpr auto is_parse_non_result(const auto&) { return false; }
constexpr auto is_parse_non_result(const type_check_parser&) { return result_check_ok_for_clang{}; }
constexpr auto is_parse_non_result(const type_parse_without_result&) { return result_check_ok_for_clang{}; }


constexpr auto& pop_back(type_check_parser& v){ return v; };
constexpr auto& pop_back(type_parse_without_result& v){ return v; };
constexpr auto& emplace_back(type_check_parser& v){ return v; };
constexpr auto& emplace_back(type_parse_without_result& v){ return v; };
constexpr void pop(auto& r) requires requires{ pop_back(r); } { pop_back(r); }
constexpr void pop(auto& r) requires requires{ r.pop_back(); } { r.pop_back(); }
constexpr void pop(auto& r) { }
template<typename type> constexpr auto& empback(type& r) {
	if constexpr(requires{ emplace_back(r); }) return emplace_back(r);
	else if constexpr(requires{ r.emplace_back(); }) return r.emplace_back();
	else if constexpr(string<type>) { r += typename decltype(auto(r))::value_type{}; return r.back(); }
	else return r;
}
constexpr void eq( auto& to, const auto& from) { empback(to) = from; }
constexpr void eq(bool ok, auto& to, const auto& from) {
  if constexpr (std::is_integral_v<std::decay_t<decltype(to)>>) to = from*ok + to*!ok;
	else (void)(ok && (eq(to, from),1));
}

template<auto cind> constexpr auto& variant_result(auto& result) {
	if constexpr (cind<0) return result;
	else if constexpr (requires{create<0>(result);}) return create<cind>(result);
	else if constexpr (requires{emplace<0>(result);}) return emplace<cind>(result);
	else if constexpr (requires{result.template emplace<0>();}) return result.template emplace<cind>();
	else return result;
}

}

namespace ascip_details {

struct skip_parser_tag{};
struct err_handler_tag{};
struct new_line_count_tag {};
struct inverted_result_tag {};

template<typename value_t, typename... t> struct context_frame {
  constexpr static auto tags = type_list<t...>{};
  value_t value;
};
constexpr auto make_default_context() {
  return tuple{context_frame<int, new_line_count_tag>{1}};
}
constexpr auto make_default_context(auto* nls) {
  using st = std::decay_t<decltype(*nls)>;
  return tuple{context_frame<st*, new_line_count_tag>{nls}};
}
constexpr static auto make_default_context(auto* nls, auto err_handler) {
  return make_ctx<err_handler_tag>(err_handler, make_default_context(nls));
}

constexpr auto make_test_ctx() { return make_default_context(); }
constexpr static auto make_test_ctx(auto* nls, auto err_handler){ return make_default_context(nls, std::move(err_handler)); }

template<template<typename...>class tuple, typename... types> constexpr decltype(auto) repack(tuple<types...>&& tup, auto&& fnc) {
  return [&]<auto... inds>(std::index_sequence<inds...>){
    return fnc(std::move(get<inds>(tup))...);
  }(std::make_index_sequence<sizeof...(types)>{});
}
template<typename... tag, typename value, typename... frames> constexpr auto make_ctx(value&& val, tuple<frames...> prev_ctx) {
  using adding_t = context_frame<std::decay_t<value>, tag...>;
  return repack(std::move(prev_ctx), [&](auto&&... prev) {
    return tuple<adding_t, frames...>{adding_t{std::forward<decltype(val)>(val)}, std::move(prev)...};
  });
}
template<typename... tag, typename value> constexpr auto make_ctx(value&& val) {
  return make_ctx<tag...>(std::forward<decltype(val)>(val), make_default_context());
}

constexpr struct ctx_not_found_type {} ctx_not_found ;
template<auto cur, auto cur_ind, auto ind, auto sz, typename... tag> constexpr auto& _search_in_ctx(type_list<tag...> tags, auto& ctx) {
  if constexpr (sz<=cur) return ctx_not_found;
  else {
    if constexpr (!contains_any(decltype(auto(get<cur>(ctx)))::tags, type_list<tag...>{})) return _search_in_ctx<cur+1, cur_ind, ind, sz>(tags, ctx);
    else {
      if constexpr (cur_ind==ind) return get<cur>(ctx).value;
      else return _search_in_ctx<cur+1, cur_ind+1, ind, sz>(tags, ctx);
    }
  }
}
template<auto ind, typename... tag, typename... frames> constexpr auto& search_in_ctx(tuple<frames...>& ctx) {
  return _search_in_ctx<0, 0, ind, sizeof...(frames)>(type_list<tag...>{}, ctx);
}
template<auto ind, typename... tag, typename... frames> constexpr const auto& search_in_ctx(const tuple<frames...>& ctx) {
  return _search_in_ctx<0, 0, ind, sizeof...(frames)>(type_list<tag...>{}, const_cast<tuple<frames...>&>(ctx));
}
template<typename... tag, typename... frames> constexpr auto& search_in_ctx(tuple<frames...>& ctx) { return search_in_ctx<0, tag...>(ctx); }
template<typename... tag, typename... frames> constexpr const auto& search_in_ctx(const tuple<frames...>& ctx) { return search_in_ctx<0, tag...>(ctx); }

template<typename... _tags, typename... frames> constexpr bool exists_in_ctx(const tuple<frames...>&) {
  constexpr type_list<_tags...> tags{};
  return (contains_any(frames::tags, tags) + ... );
}


constexpr auto& new_line_counter(auto& ctx) {
  if constexpr (requires{*search_in_ctx<new_line_count_tag>(ctx);}) return *search_in_ctx<new_line_count_tag>(ctx);
  else return search_in_ctx<new_line_count_tag>(ctx);
}
struct fucky_clang { constexpr static bool ok=true; };
consteval auto need_count_new_lines(auto& ctx) {
  constexpr bool exists_val = requires{ search_in_ctx<new_line_count_tag>(ctx) = 1; };
  constexpr bool exists_ptr = requires{ *search_in_ctx<new_line_count_tag>(ctx) = 1; };
  if constexpr (exists_ptr || exists_val) return fucky_clang{};
  else return false;
}
consteval auto need_count_new_lines(auto& ctx, auto& r) {
  if constexpr (!requires{ is_checking(r).ok; } && requires{need_count_new_lines(ctx).ok;}) return fucky_clang{};
  else return false;
}
constexpr void count_new_line(auto p_result, auto& ctx, auto sym, auto& r) {
	if constexpr (requires{need_count_new_lines(ctx, r).ok;}) {
	  constexpr bool is_result_inverted = exists_in_ctx<inverted_result_tag>(decltype(auto(ctx)){});
	  if constexpr (is_result_inverted) new_line_counter(ctx) += (sym == '\n') * (p_result < 0);
	  else new_line_counter(ctx) += (sym == '\n') * (p_result > 0);
	}
}

constexpr auto new_line_count(auto& ctx) {
  if constexpr (!requires{need_count_new_lines(ctx).ok;}) return 0;
  else return new_line_counter(ctx);
}

template<typename type> constexpr auto update_shift(auto& ctx, auto ret) {
  if constexpr(requires{ *search_in_ctx<type>(ctx) = 1; }) *search_in_ctx<type>(ctx) = ret * (0<=ret);
}

struct new_line_counter_resetter_fake {
  constexpr static void update(auto&&...) {}
};

template<typename storage>
struct new_line_counter_resetter {
  storage* val;
  storage prev;
  constexpr explicit new_line_counter_resetter(auto& ctx) : val(&new_line_counter(ctx)), prev(*val) {}
  constexpr ~new_line_counter_resetter() { *val = prev; }
  constexpr void update(auto pr) { prev = prev*(pr<=0) + (*val)*(0<pr); }
};

constexpr auto make_new_line_count_resetter(auto& ctx, auto& r) {
  using storage_type = std::decay_t<decltype(new_line_counter(ctx))>;
  if constexpr (!requires{need_count_new_lines(ctx, r).ok;}) return new_line_counter_resetter_fake{};
  else return new_line_counter_resetter<storage_type>{ctx};
}

} // namespace ascip_details

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

constexpr auto back_to_nl(auto& src, auto& ind) {
	auto prev = ind;
	while (ind>0 &&  src[--ind]!='\n');
	return prev - ++ind;
}

template<ascip_details::string_view type>
constexpr static auto make_source(type&& src) {
	struct strsrc {
		type src;
		unsigned ind = 0;
		constexpr auto operator()(){ return src[ind++]; }
		constexpr explicit operator bool() const { return ind < src.size(); }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
		constexpr auto back_to_nl() { return ascip_details::back_to_nl(src, ind); }
		constexpr static bool can_read_after_end() { return false; }
	} ret{ std::forward<decltype(src)>(src) };
	return ret;
}

constexpr static auto make_source(std::integral auto sym) {
	struct gcc_requires_name_for_tu_bug { // https://stackoverflow.com/a/66966679/24870413
		decltype(sym) val; bool where_is_more=true;
		constexpr auto operator()(){ where_is_more=false; return val; }
		constexpr explicit operator bool() const { return where_is_more; }
		constexpr auto& operator += (int v) { (void)( v==1 || (throw 1,1) ); where_is_more=false; return *this; }
	} ret{ sym };
	return ret;
}

constexpr auto strlen(const auto* vec){ unsigned ret = 0; while(vec[ret])++ret; return ++ret; }
constexpr static auto make_source(const auto* vec) {
	struct gcc_requires_name_for_tu_bug { // https://stackoverflow.com/a/66966679/24870413
		decltype(vec) val;
		unsigned sz;
		unsigned ind = 0;
		constexpr auto operator()(){ return val[ind++]; }
		constexpr explicit operator bool() const { return ind < sz-1; }
		constexpr auto& operator += (const int v) { ind+=v; return *this; }
		constexpr auto back_to_nl() { return ascip_details::back_to_nl(val, ind); }
		constexpr static bool can_read_after_end() { return true; }
	} ret{ vec, strlen(vec) };
	return ret;
}

template<typename src_type> constexpr bool can_read_after_end() {
	if constexpr (requires{src_type::can_read_after_end();})
		return src_type::can_read_after_end();
	else return false;
}

constexpr auto pos(const auto& src) {
	//TODO: remove this function and use information from context
	if constexpr ( requires{ position(src); } ) return position(src);
	else return src.ind;
}

} // namespace ascip_details

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

template<typename char_type, auto str_size>
struct string_literal {
	constexpr string_literal(const char_type (&str)[str_size]) {
		for(auto i=0;i<str_size;++i) value[i] = str[i];
	}

	char_type value[str_size];

	constexpr auto size() const { static_assert( 1 <= str_size ); return str_size-1; }
	constexpr auto operator[](auto i) const { return value[i]; }

	consteval bool contains(char_type s) const {
		for (auto i=0;i<str_size;++i) if (value[i]==s) return true;
		return false;
	}

	constexpr void for_each(auto&& fnc) const {
		[&]<auto... i>(std::index_sequence<i...>){
			(fnc(value[i]), ...);
		}(std::make_index_sequence<str_size>{});
	}

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

template<string_literal str> struct test_tmpl {
	constexpr bool is_eq(const char* v) const { return str == v; }
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

	static_assert(  string_literal("abc").contains('b') );
	static_assert( !string_literal("abc").contains('d') );
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


namespace ascip_details {
template<auto... inds, template<typename...>class wrapper, typename... parsers>
constexpr static bool exists_in_get(const wrapper<parsers...>* seq, const auto& checker, const auto& stop) {
	if constexpr (sizeof...(inds) == sizeof...(parsers)) return false;
	else return
		(!stop((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr)
		&& (
		      checker((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr)
		   || exists_in((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr, checker, stop)
		   )
		)
		|| exists_in_get<inds..., sizeof...(inds)>(seq, checker, stop)
		;
}
template<template<typename>class wrapper, typename parser>
constexpr static bool exists_in_derived(const wrapper<parser>* src, const auto& checker, const auto& stop) {
	if(stop(src)) return false;
	if(checker(src)) return true;
	return exists_in(static_cast<const parser*>(src), checker, stop);
}

constexpr static bool exists_in(auto&& src, const auto& checker, const auto& stop) {
	return exists_in(&src, checker, stop);
}

constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires (!requires{typename std::decay_t<decltype(*src)>::type_in_base;}){
	return false;
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) {
	if(stop(src)) return false;
	if(checker(src)) return true;
	if constexpr (std::is_same_v<typename std::decay_t<decltype(*src)>::type_in_base, std::decay_t<decltype(*src)>>)
		return checker(src);
	else return exists_in_derived(src, checker, stop);
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->p; } {
	using type = std::decay_t<decltype(src->p)>;
	return stop(src) ? false : checker(src) || exists_in((type*)nullptr, checker, stop);
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->lp; } {
	using ltype = std::decay_t<decltype(src->lp)>;
	using rtype = std::decay_t<decltype(src->rp)>;
	if(stop(src)) return false;
	const bool lstop = stop((ltype*)nullptr);
	const bool rstop = stop((rtype*)nullptr);
	return checker(src)
		|| (!lstop&&checker((ltype*)nullptr))
		|| (!rstop&&checker((rtype*)nullptr))
		|| (!lstop&&exists_in((ltype*)nullptr, checker, stop))
		|| (!rstop&&exists_in((rtype*)nullptr, checker, stop))
		;
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->seq; } {
	using seq_t = decltype(src->seq);
	if(stop(src)) return false;
	return checker(src) || exists_in_get((seq_t*)nullptr, checker, stop); }
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->s; src->b; } {
	//NOTE: it's for injected_parser, but without forward declaration
	if(stop(src)) return false;
	const auto* ptr = static_cast<const std::decay_t<decltype(src->b)>*>(nullptr);
	if(checker(ptr)) return true;
	return exists_in(ptr, checker, stop) ; }

template<typename p, template<auto>class t=p::template tmpl>
constexpr static bool test_exists_in() {
	auto checker = [](const auto* s){ return std::is_same_v<std::decay_t<decltype(*s)>, std::decay_t<decltype(t<'a'>::char_)>>; };
	auto pass = [](const auto* s){ return false; };
	auto stop = [](const auto* s){ return true; };
	static_assert(  exists_in(t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_, checker, stop) );
	static_assert(  exists_in(skip(t<'a'>::char_), checker, pass) );
	static_assert( !exists_in(skip(t<'a'>::char_), checker, stop) );
	static_assert( !exists_in(skip(t<'b'>::char_), checker, pass) );
	static_assert(  exists_in(t<'b'>::char_ | t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_ | t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_ | t<'x'>::char_, checker, pass) );
	static_assert(  exists_in(t<'b'>::char_ - t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_ - t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_ - t<'x'>::char_, checker, pass) );
	static_assert(  exists_in(t<'c'>::char_ >> (t<'b'>::char_ - t<'a'>::char_), checker, pass) );
	static_assert(  exists_in(t<'c'>::char_ - (t<'b'>::char_ >> t<'a'>::char_), checker, pass) );
	static_assert( !exists_in(t<'c'>::char_ - (t<'b'>::char_ >> t<'a'>::char_), checker, stop) );
	static_assert(  exists_in(skip(t<'c'>::char_ >> t<'b'>::char_ >> t<'a'>::char_++), checker, pass) );
	static_assert( !exists_in(skip(t<'c'>::char_ >> t<'b'>::char_ >> t<'a'>::char_++), checker, stop) );
	return true;
}

} // namespace ascip_details
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 1) { auto&& [f1]=r; return (forward_like<decltype(r)>)(nth<ind>(f1)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 2) { auto&&[f1,f2]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 3) { auto&&[f1,f2,f3]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 4) { auto&&[f1,f2,f3,f4]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 5) { auto&&[f1,f2,f3,f4,f5]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 6) { auto&&[f1,f2,f3,f4,f5,f6]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 7) { auto&&[f1,f2,f3,f4,f5,f6,f7]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 8) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 9) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 10) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 11) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 12) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 13) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 14) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 15) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 16) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 17) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 18) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 19) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 20) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 21) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 22) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 23) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 24) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 25) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 26) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 27) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 28) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 29) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 30) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 31) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 32) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 33) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 34) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 35) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 36) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 37) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 38) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 39) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 40) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 41) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 42) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 43) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 44) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 45) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 46) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 47) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 48) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 49) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 50) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 51) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 52) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 53) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 54) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 55) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 56) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 57) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 58) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 59) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 60) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 61) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 62) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 63) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 64) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 65) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 66) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 67) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 68) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 69) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 70) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 71) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 72) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 73) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 74) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 75) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 76) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 77) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 78) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 79) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 80) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 81) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 82) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 83) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 84) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 85) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 86) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 87) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 88) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 89) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 90) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 91) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 92) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 93) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 94) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 95) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 96) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 97) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 98) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 99) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==100) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==101) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==102) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==103) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==104) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==105) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==106) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==107) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==108) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==109) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==110) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==111) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==112) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==113) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==114) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==115) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==116) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==117) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==118) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==119) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==120) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==121) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==122) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==123) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==124) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==125) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==126) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==127) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==128) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127,f128]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127,f128)); }
constexpr void test () {
	struct tf1{ char f1; };
	struct tf2{ char f1, f2; };
	static_assert( []{ tf1 t{ 'a' }; return get<0>(t); }() == 'a' );
	static_assert( []{ tf2 t{ 'a', 'b' }; return get<0>(t); }() == 'a' );
	static_assert( []{ tf2 t{ 'a', 'b' }; return get<1>(t); }() == 'b' );
}
} // namespace ascip_reflection
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details {

template<typename type> struct forwarder{ type& o; constexpr forwarder(type& o) : o(o) {} };
template<typename type> forwarder(type&) -> forwarder<type>;
constexpr auto fwd(auto& o) { return forwarder( o ); }

}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details {

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
template<typename mutator, template<typename>class wrapper, typename inner>
constexpr static auto transform(wrapper<inner>&& src, auto& ctx) requires (requires{ src.p; } && !requires{transform_special<mutator>(std::move(src), ctx);}){
	auto nctx = mutator::create_ctx(src, ctx);
	auto mp = transform<mutator>(std::move(src.p), nctx);
	if constexpr(requires{ wrapper{{}, std::move(mp)}; })
		return transform_apply<mutator>(wrapper{{}, std::move(mp)}, nctx);
	else
		return transform_apply<mutator>(wrapper{std::move(mp)}, nctx);
}
template<typename mutator, template<typename...>class semact_wrapper, typename parser, typename act_t, typename... tags>
constexpr static auto transform(semact_wrapper<parser, act_t, tags...>&& src, auto& ctx) requires requires{ src.act; src.p; } {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( semact_wrapper<std::decay_t<decltype(np)>, std::decay_t<decltype(src.act)>, tags...>{ {}, std::move(src.act), std::move(np) }, nctx );
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details {

constexpr auto inject_skipping(auto&& to, auto&& what) ;

constexpr auto parse(auto&& parser, auto src) {
	type_parse_without_result r;
	return parse(std::forward<decltype(parser)>(parser), src, r);
}

constexpr auto parse(auto&& parser, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	long int nls = 1;
	return parser.parse(make_default_context(&nls), src, result);
}

constexpr auto parse(auto&& parser, const auto& skip, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	long int nls = 1;
	auto ctx = make_ctx<skip_parser_tag>( skip, make_default_context(&nls) );
	return inject_skipping(auto(parser), std::forward<decltype(skip)>(skip)).parse(ctx, src, result);
}

constexpr auto parse(auto&& parser, auto&& skip, auto src, auto& result, const auto& err) {
	using parser_type = std::decay_t<decltype(parser)>;
	long int nls = 1;
	auto ctx = make_ctx<skip_parser_tag>( skip, make_default_context(&nls, &err) );
	return inject_skipping(auto(parser), std::move(skip)).parse(ctx, src, result);
}

constexpr auto continue_parse(const auto& ctx, auto&& parser, auto src, auto& result) {
	auto err = search_in_ctx<err_handler_tag>(ctx);
	auto skip = search_in_ctx<skip_parser_tag>(ctx);
	constexpr bool skip_found = !std::is_same_v<ctx_not_found_type, decltype(skip)>;
	constexpr bool err_found = !std::is_same_v<ctx_not_found_type, decltype(err)>;
	if constexpr (!skip_found && !err_found) return parse(std::forward<decltype(parser)>(parser), src, result);
	else if constexpr (skip_found && !err_found) return parse(std::forward<decltype(parser)>(parser), skip, src, result);
	else return parse(std::forward<decltype(parser)>(parser), skip, src, result, err);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details {

template<typename char_type, auto sz>
constexpr auto& print_to(auto& os, const ascip_details::string_literal<char_type, sz>& what) { return os << what.value; }
constexpr auto& print_to(auto& os, const auto& what) { return os << what; }
constexpr void write_out_error_msg(
		auto& os,
		auto fn,
		auto msg,
		auto expt,
		auto src,
		auto ln
		) {
	os << "Error in ";
	print_to(os, fn) << ':' << ln << ' ';
	print_to(os, msg) << "\n";
	auto shift = src.back_to_nl();
	for(auto cur = src(); src; cur = src()) os << cur;
	os << "\n";
	for(auto i=0;i<shift;++i) os << '-';
	os << "^\n";
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {
struct adl_tag {};
struct any_shift_tag {};
struct list_shift_tag {};
using parse_result = decltype(-1);

namespace prs { template<typename parser> struct base_parser; }

template<typename type> concept parser = std::is_base_of_v<prs::base_parser<std::decay_t<type>>, std::decay_t<type>> ;
template<typename type> concept nonparser = !parser<type>;

} // namespace ascip_details

namespace ascip_details::prs {

template<typename parser> struct base_parser : adl_tag {
	using type_in_base = parser;

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert( requires(const parser& p){ p.p; }, "child parser should define own parse method or have p field" );
		return static_cast<const parser&>(*this).p.parse(static_cast<decltype(ctx)&&>(ctx), std::move(src), result);
	}

	//NOTE: to have access to child classes we must to implement body after the classes will be defined
	constexpr auto operator()(auto act) const ;
};

} // namespace ascip_details::prs


namespace ascip_details::prs {

struct nop_parser : base_parser<nop_parser> {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&&, const auto&, auto&) {
		return 0;
	}
};

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {
template <auto sym> struct char_parser : base_parser<char_parser<sym>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto cur = src();
		const bool ok = cur == sym;
		eq(ok, result, cur);
		parse_result ret = -1 + 2 * ok;
		count_new_line(ret, ctx, cur, result);
		return ret;
	}

	constexpr bool test() const {
		char r{};
		parse(make_test_ctx(), make_source(sym), r) == 1 || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym + 1), r) == -1 || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym - 1), r) == -1 || (throw __LINE__, 1);

		r = sym + 1;
		(parse(make_test_ctx(), make_source(sym), r), r) == sym || (throw __LINE__, 1);

		r = sym + 1;
		(parse(make_test_ctx(), make_source(sym - 1), r), r) == sym + 1 || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		if constexpr (sym == '\n') {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);
		}
		else {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		}

		return true;
	}
};

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {
template <ascip_details::string_literal val>
struct literal_parser : base_parser<literal_parser<val>> {
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    // TODO: what if the literal contains \n - it will be not counted if literal matches
    // TODO: faster? add [] operator in src for direct access (operator[](auto i){return val[ind+i];})
    auto i = -1, r = 0;
    {
      auto tsrc = src;
      tsrc += val.size() - 1;
      if (!tsrc)
        return -1;
    }
    while (++i < val.size()) r += (src() == val[i]);
    parse_result ret = ((r + 1) * (r == val.size())) - 1;
    if constexpr (val.contains('\n')) {
      val.for_each([&](auto s) { count_new_line(ret, ctx, s, result); });
    }
    return ret;
  }
};

template <ascip_details::string_literal v>
constexpr static auto lit = literal_parser<v>{};
constexpr bool test_literal_parser() {
  char r;
  static_assert(literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd"), r) == 3);
  static_assert(literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd_tail"), r) == 3);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("abcd"), r) == 4);
  static_assert(literal_parser<"abcdef">{}.parse(make_test_ctx(), make_source("abcdef"), r) == 6);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("bbcd"), r) == -1);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("ab"), r) == -1);
  static_assert( [&] {
      auto ctx = make_test_ctx();
      literal_parser<"ab\ncd\nef">{}.parse(ctx, make_source("ab\ncd\nef"), r);
      return new_line_count(ctx);
    }() == 3, "if the literal contains new lines it must to be counted");
  return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<typename t> struct value_parser : base_parser<value_parser<t>> {
	t val;
	constexpr value_parser(t v) : val(v) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto cur = src();
		const bool ok = cur == val;
		eq(ok, result, cur);
		count_new_line(1, ctx, cur, result);
		return -2 * !ok + 1;
	}

	constexpr bool test() const {
		char r{};
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





template<typename t> constexpr auto parse_value(t&& val) {
	return value_parser<std::decay_t<t>>{ std::forward<decltype(val)>(val) };
}

constexpr static bool test_parser_value() {
	static_assert( value_parser{ 'a' }.test() ); static_assert( value_parser{ 'Z' }.test() );
	static_assert( value_parser{ L'!' }.test() ); static_assert( value_parser{ '\n' }.test() );
	static_assert( []{char r{}; return value_parser{'a'}.parse(make_test_ctx(), make_source("abc"), r);}() == 1 );
	static_assert( []{char r{}; return value_parser{'b'}.parse(make_test_ctx(), make_source("abc"), r);}() == -1 );
	static_assert( [] {
		auto ctx1 = make_test_ctx();
		auto ctx2 = make_test_ctx();
		char r{};
		value_parser{'\n'}.parse(ctx1, make_source("a"), r);
		value_parser{'\n'}.parse(ctx2, make_source("\n"), r);
		return (new_line_count(ctx1) == 1) + 2*(new_line_count(ctx2) == 2);
	}() == 3 );
	return true;
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

struct space_parser : base_parser<space_parser> {
	constexpr parse_result parse(auto&& ctx,auto src, auto& r) const {
		auto sym = src();
		const bool is_space = 0x07 < sym && sym < '!'; // 0x08 is a backspace
		parse_result ret = -1 + 2*is_space;
		count_new_line(ret, ctx, sym, r);
		return ret;
	}
	constexpr bool test() const {
		constexpr char r=0x00;
		parse(make_test_ctx(), make_source(' '), r) == 1    || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\n'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\t'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('!'), r) == -1   || (throw __LINE__, 1);
		r == 0x00                                           || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		new_line_count(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		new_line_count(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		new_line_count(ctx) == 2 || (throw __LINE__, 1);

		return true;
	}
} ;

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

struct any_parser : base_parser<any_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = 0;
		decltype(src()) cur;
		do {
			cur = src();
			ascip_details::eq( result, cur );
			count_new_line(++ret, ctx, cur, result);
		}
		while(src && (cur & 0x80)) ;
		return ret;
	}
	constexpr bool test() const {
		char r=0x00; type_parse_without_result rr;
		parse(make_test_ctx(), make_source(' '), r) == 1       || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('Z'), r),r) == 'Z' || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('~'), r),r) == '~' || (throw __LINE__, 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("я"), rr)  == 2);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"я"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"яz"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("❤"), rr) == 3);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("θ"), rr) == 2);

		auto ctx = make_test_ctx();
		search_in_ctx<new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		search_in_ctx<new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		search_in_ctx<new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);

		return true;
	}
};

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<auto base=10> struct int_base_parser {
	constexpr static bool is_int(auto s) {
		constexpr bool is_0 = base <= 10;
		return
			   ('0' <= s && s <= '9')
			|| (!is_0 && ('a' <= s && s <= ('a'+base-10)))
			|| (!is_0 && ('A' <= s && s <= ('A'+base-10)))
			;
	}
	constexpr static int to_int(auto s) {
		const bool is_a = 'a' <= s && s <= 'z';
		const bool is_A = 'A' <= s && s <= 'Z';
		const bool is_0 = '0' <= s && s <= '9';
		return
			  is_0 * (s - '0')
			+ is_a * (s - 'a' + 10)
			+ is_A * (s - 'A' + 10)
			;
	}
	constexpr bool next(auto cur, auto& result) const {
		const bool isint = is_int(cur);
		result *= (base*isint) + !isint;
		result += to_int(cur) * isint;
		return isint;
	}
};

struct int_parser : base_parser<int_parser>, int_base_parser<> {
	constexpr parse_result parse(auto&&, auto src, auto& _result)  const {
		auto sign = src();
		if(sign != '-' && sign != '+' && !this->is_int(sign)) return -1;
		int signer = -1*(sign=='-') + this->is_int(sign) + (sign=='+');
		ascip_details::eq(_result, this->is_int(sign) * this->to_int(sign));
		auto& result = _result;
		auto ret = 1;
		while(src && this->next(src(), result)) ++ret;
		result *= signer;
		bool bad_result = ((sign=='-')+(sign=='+')+(ret==1))==2;
		return -1*bad_result + ret*!bad_result;
	}
	constexpr auto parse_without_preparation(auto src, auto& result) const {
		auto ret = 0;
		while(src && this->next(src(), result)) ++ret;
		return ret;
	}

	constexpr bool test() const {
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


		auto r=0;
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("!"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("a"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("-"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("-["), r);}) == -1 );


		return true;
	}

};

template<auto base> struct uint_parser : base_parser<uint_parser<base>>, int_base_parser<base> {
	constexpr auto parse(auto&&, auto src, auto& result) const {
		auto ret = 0;
		while(src && this->next(src(), result)) ++ret;
		return ret - (ret==0);
	}

	constexpr bool test() const {
		uint_parser p; auto r=0;

		auto t = [](auto&& src, auto sym_cnt) {
			int r=0;
			uint_parser<10> p{};
			p.parse(make_test_ctx(), make_source(src), r) == sym_cnt || (throw __LINE__, 1);
			return r;
		};

		static_assert( []{ int r=0; return uint_parser<10>{}.parse(make_test_ctx(), make_source("+"), r); }() == -1 );
		static_assert( t("1", 1) == 1 );
		static_assert( t("2", 1) == 2 );
		static_assert( t("0", 1) == 0 );
		static_assert( t("103", 3) == 103 );

		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("A"), r); return r; }() == 10 );
		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("A3"), r); return r; }() == 163 );
		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("FD"), r); return r; }() == 253 );
		static_assert( []{ int r=0; return uint_parser<16>{}.parse(make_test_ctx(), make_source("FD"), r); }() == 2 );

		return true;
	}
} ;

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs {

struct float_point_parser : base_parser<float_point_parser> {
	constexpr static auto pow(auto what, auto to) { const auto m = what; for(auto i=1;i<to;++i) what*=m; return what; }
	constexpr parse_result parse(auto&&, auto src, auto& result)  const {
		constexpr int_parser int_{};
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
		constexpr float_point_parser p{};
		auto t = [](auto&& src, auto sym_cnt, auto answer) {
			const float_point_parser p{};
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
};
} // namespace ascip_details::prs

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs {

template<auto from, auto to> struct range_parser : base_parser<range_parser<from,to>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto sym = src();
		const bool ok = from <= sym && sym <= to;
		eq( ok, result, sym );
		parse_result ret = -2 * !ok + 1;
		if constexpr (from <= '\n' && '\n' <= to)
			count_new_line(ret, ctx, sym, result);
		return ret;
	}
};

constexpr static bool test_range_parser() {
	constexpr static auto ascii = range_parser<(char)0x01,(char)0x7F>{};

	constexpr static auto lower = range_parser<'a','z'>{};
	static_assert( ({char r{};lower.parse(make_test_ctx(), make_source("a"), r);r;}) == 'a' );
	static_assert( ({char r{};lower.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source("A"), r);}) == 1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source('~'+1), r);}) == 1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source('~'+2), r);}) == -1 );

	static_assert( [&] {
		char r{};
		auto ctx = make_test_ctx();
		ascii.parse(ctx, make_source("\n"), r);
		return new_line_count(ctx);
	}() == 2 );
	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<typename parser, typename act_t> struct semact_parser : base_parser<semact_parser<parser,act_t>> {
	act_t act;
	[[no_unique_address]] parser p;

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr(requires{is_parse_non_result(result).ok;})
			return p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
		else if constexpr(requires{ act(result); requires std::is_pointer_v<decltype(act(result))>;} ) {
			auto* nr = act(result);
			return p.parse(ctx, src, *nr);
		}
		else if constexpr (requires{ act(result); requires std::is_lvalue_reference_v<decltype(act(result))>; } ) {
			auto& nr = act(result);
			return p.parse(ctx, src, nr);
		}
		else if constexpr(requires{ act(result); } && !requires{act(0, ctx, src, result);} ) {
			auto nr = act(result);
			return p.parse(ctx, src, nr);
		}
		else {
			auto ret = p.parse(ctx, src, result);
			if(ret >= 0) {
				if constexpr (requires{ act(ret, &ctx, src, result); }) act(ret, &ctx, src, result);
				else if constexpr (requires{ act(ret, result); }) act(ret, result);
				else act();
			}
			return ret;
		}
	}
};

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_semact() {

	static_assert( ({char r='z',a='y';(t<'a'>::char_ >> t<'b'>::char_([&a](){a='c';})).parse(make_test_ctx(),make_source("ab"),r);a;}) == 'c', "semact: calls if parsed");
	static_assert( ({char r='z',a='y';(t<'a'>::char_ >> t<'b'>::char_([&a](){a='c';})).parse(make_test_ctx(),make_source("au"),r);a;}) == 'y', "semact: silent if fails");

	return true;
}

template<typename parser, typename act_type, typename tag> struct exec_before_parser : base_parser<exec_before_parser<parser, act_type, tag>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr(requires{is_parse_non_result(result).ok;}) return p.parse(ctx, src, result);
		else if constexpr(requires{*act(search_in_ctx<tag>(ctx), result);}) {
			auto* new_result = act(search_in_ctx<tag>(ctx), result);
			return p.parse(ctx, src, *new_result);
		}
		else {
			act(search_in_ctx<tag>(ctx), result);
			return p.parse(ctx, src, result);
		}
	}
};

template<typename parser, typename act_type, typename tag> struct exec_after_parser : base_parser<exec_after_parser<parser, act_type, tag>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		if constexpr (!requires{is_parse_non_result(result).ok;}) act(search_in_ctx<tag>(ctx), result);
		else if constexpr(requires{act(search_in_ctx<tag>(ctx));}) act(search_in_ctx<tag>(ctx));
		return ret;
	}
};

template<typename tag, parser type> constexpr auto exec_before(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return exec_before_parser<type, act_type, tag>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename tag, parser type> constexpr auto exec_after(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return exec_after_parser<type, act_type, tag>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool context_parsers() {
	struct tag {};
	static_assert( []{
		char r{};
		int r_inner{};
		char val=3;
		parse(add_to_ctx<tag>(&val,
			t<'a'>::char_ >>
			exec_before<tag>([&](auto* val, auto& _p){r_inner=_p*(*val);return &_p;}, t<'b'>::char_)
		), p::make_source("ab"), r);
		return r_inner;
	}() == 'a'*3 );
	static_assert( []{
		char r{};
		int r_inner{};
		char val=3;
		parse(add_to_ctx<tag>(&val,
			t<'a'>::char_ >>
			exec_after<tag>([&](auto* val, auto& _p){r_inner=_p*(*val);}, t<'b'>::char_)
		), p::make_source("ab"), r);
		return r_inner;
	}() == 'b'*3 );

	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<parser type> struct def_parser : base_parser<def_parser<type>> { type p; };

template<parser type> constexpr auto def(type&& p) {
  return def_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) };
}

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_def_parser() {
  static_assert( [] {
    char r{};
    auto pr = parse(t<'a'>::char_ >> def(t<'b'>::char_) >> t<'c'>::char_, p::make_source("abc"), r);
    return (pr==3) + 2*(r=='c');
  }() == 3 );
  static_assert( [] {
    type_parse_without_result r{};
    return parse(t<'a'>::char_ >> def(t<'b'>::char_) >> t<'c'>::char_, p::make_source("abc"), r);
  }() == 3 );
  static_assert( [] {
    type_check_parser r{};
    return parse(t<'a'>::char_ >> def(t<'b'>::char_) >> t<'c'>::char_, p::make_source("abc"), r);
  }() == 2 );
  static_assert( [] {
    type_parse_without_result r{};
    return parse(t<'a'>::char_ >> t<'b'>::char_ > t<'c'>::char_, +p::space, p::make_source("abc"), r, [](int,auto){return 0;});
  }() == 3 );
  static_assert( [] {
    type_check_parser r{};
    return parse(t<'a'>::char_ >> t<'b'>::char_ > t<'c'>::char_, +p::space, p::make_source("abc"), r, [](int,auto){return 0;});
  }() == 2 );
  return true;
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {
template<string_literal msg, parser type> struct must_parser : base_parser<must_parser<msg, type>> {
	type p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		return call_if_error(ctx, result, ret, src);
	}

	constexpr static auto call_if_error(auto& ctx, auto& result, auto orig_ret, auto& src) {
		if (0 <= orig_ret) return orig_ret;
		auto err = search_in_ctx<err_handler_tag>(ctx);
		static_assert( !std::is_same_v<std::decay_t<decltype(err)>, ctx_not_found_type>, "for using the must parser a error handler is required" );
		if constexpr(requires{(*err)(0, msg);}) return (*err)(new_line_count(ctx), msg);
		else if constexpr(requires{(*err)(0, src, msg);}) return (*err)(new_line_count(ctx), src, msg);
		else if constexpr(requires{(*err)(msg);}) return (*err)(msg);
		else if constexpr(requires{(*err)(result, 0, msg);}) return (*err)(result, new_line_count(ctx), msg);
		else if constexpr(requires{(*err)(result, src, 0, msg);}) return (*err)(result, src, new_line_count(ctx), msg);
		else {
			static_assert( requires{(*err)(result, msg);}, "the error handler have to request no result as parameter or it have to be a template parameter" );
			return (*err)(result, msg);
		}
	}
};

constexpr static auto is_must_parser = [](const auto* p) {
	constexpr auto checker = []<string_literal msg, parser type>(const must_parser<msg, type>*){ return true; };
	return requires{ checker(p); };
};

template<parser left, typename right> constexpr auto operator>(left&& l, right&& r) {
	return std::move(l) >> must<"unknown">(std::forward<decltype(r)>(r));
}

template<string_literal msg> constexpr auto must(parser auto&& p) {
	return must_parser<msg, std::decay_t<decltype(p)>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_must_parser() {
	static_assert( []{
		char r='z'; int nls=0;
		const auto err_method = [&](...){return -10;};
		return (t<'a'>::char_ >> t<'b'>::char_ >> must<"t">(t<'c'>::char_)).parse(make_test_ctx(&nls, &err_method), make_source("abe"), r);
	}() == -10, "error lambda are called");

	const auto err_method = [](
			auto&,
			auto line_number,
			auto message){
		line_number /= (line_number==2);
		return (-3 * (message=="unknown")) + (-4 * (message=="test"));
	};
	static_assert( [&]{ char r=0x00; int nls=1; auto ctx = make_test_ctx(&nls, &err_method);
		return (p::any >> t<'a'>::char_ >> t<'b'>::char_ > t<'c'>::char_).parse(ctx, make_source("\nabe"), r);
	}() == -3, "on error: sources are on start sequence and on rule where the error");
	static_assert( [&]{ char r=0x00; int nls=1; auto ctx = make_test_ctx(&nls, &err_method);
		return (p::any >> t<'a'>::char_ >> t<'b'>::char_ >> must<"test">(t<'c'>::char_)).parse(ctx, make_source("\nabe"), r);
	}() == -4, "on error: sources are on start sequence and on rule where the error");

	return true;
}

} // namespace ascip_details::prs

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details::prs {
struct seq_stack_tag{};
struct seq_shift_stack_tag{};
struct seq_result_stack_tag{};
}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs {
template<parser parser>
struct use_seq_result_parser : base_parser<use_seq_result_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		auto& result = *search_in_ctx<seq_result_stack_tag>(ctx);
		return p.parse(ctx, src, result);
	}
};
}

namespace ascip_details {
template<parser type> constexpr auto use_seq_result(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return prs::use_seq_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) }; }
}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<auto ind> struct seq_shift_parser : base_parser<seq_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
	constexpr parse_result parse(auto&& ctx, auto, auto& result) const {
		eq(result, *search_in_ctx<ind, seq_shift_stack_tag>(ctx));
		return 0;
	}
};

//TODO: dose we realy need the pos parser?
struct cur_pos_parser : base_parser<cur_pos_parser> {
	constexpr parse_result parse(auto&&, auto src, auto& result) const {
		//TODO: extract the info from context or from parent's object
		//      sequence may sotre it in context
		//      sequence may have mutable field and
		//        pass it to parse method here or
		//        store it to result on it's own
		//      if use context - make it with tags
		eq(result, pos(src));
		return 0;
	}
};

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)







namespace ascip_details::prs {

struct seq_enable_recursion_parser : base_parser<seq_enable_recursion_parser> {
  constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&&, auto&, auto&) { return 0; }
};

template<auto ind>
struct seq_recursion_parser : base_parser<seq_recursion_parser<ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		const auto* req = *search_in_ctx<ind, seq_stack_tag>(ctx);
		return src ? req->call_parse(src, result) : -1 ;
	}
};

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<auto cnt> struct _seq_rfield_val { constexpr static auto num_val = cnt; };
template<ascip_details::parser parser, typename val> struct seq_inc_rfield_val : base_parser<seq_inc_rfield_val<parser, val>> {
	constexpr seq_inc_rfield_val() =default ;
	constexpr seq_inc_rfield_val(seq_inc_rfield_val&&) noexcept =default ;
	constexpr seq_inc_rfield_val(const seq_inc_rfield_val&) noexcept =default ;
	constexpr explicit seq_inc_rfield_val(parser p) : p(std::move(p)) {}
	parser p;

	constexpr static auto value = val::num_val;
};
template<ascip_details::parser parser, typename val> struct seq_num_rfield_val : base_parser<seq_num_rfield_val<parser, val>> {
	constexpr seq_num_rfield_val() =default ;
	constexpr seq_num_rfield_val(seq_num_rfield_val&&) noexcept =default ;
	constexpr seq_num_rfield_val(const seq_num_rfield_val&) noexcept =default ;
	constexpr explicit seq_num_rfield_val(parser p) : p(std::move(p)) {}
	parser p;

	constexpr static auto value = val::num_val;
};

template<typename type, typename stop_on_parser, template<typename...>class tmpl> constexpr int grab_num_val() {
	int val = 0;
	exists_in((type*)nullptr, [&val](const auto* p){
		constexpr bool is_num = ascip_details::is_specialization_of<std::decay_t<decltype(*p)>, tmpl>;
		if constexpr (is_num) val = std::decay_t<decltype(*p)>::value;
		return is_num;
	}, [](const auto* p) {
		return requires{ p->seq; } && !requires{ static_cast<const stop_on_parser*>(p); };
	});
	return val;
}

struct seq_inc_rfield : base_parser<seq_inc_rfield> {constexpr parse_result parse(auto&&,auto,auto&)const {return 0;} };
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

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename value, typename type>
constexpr static auto transform_special(prs::seq_inc_rfield_val<type, value>&& src, auto& ctx) {
	constexpr auto num = prs::seq_inc_rfield_val<type, value>::value;
	return transform_apply<mutator>(finc<num>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
template<typename mutator, parser ptype, typename value>
constexpr static auto transform_special(prs::seq_num_rfield_val<ptype, value>&& src, auto& ctx) {
	constexpr auto num = prs::seq_num_rfield_val<ptype, value>::value;
	return transform_apply<mutator>(fnum<num>(transform<mutator>(std::move(src.p), ctx)), ctx);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs::seq_details {

template<typename source, typename result> struct monomorphic {
  virtual ~monomorphic() =default ;
  virtual parse_result parse_mono_omit(source src) const =0 ;
  virtual parse_result parse_mono_check(source src) const =0 ;
  virtual parse_result parse_mono(source src, result& r) const =0 ;

  constexpr parse_result call_parse(source src, auto& r) const {
    if constexpr(requires{ is_parsing_without_result(r).ok; })
      return parse_mono_omit(std::move(src));
    else if constexpr(requires{ is_checking(r).ok; })
      return parse_mono_check(std::move(src));
    else
      return parse_mono(std::move(src), r);
  }
};

template<typename parser, typename context, typename source, typename result>
struct mono_for_seq final : monomorphic<source, result> {
	using base_type = monomorphic<source, result>;
	const parser* self;
	mutable context ctx;
	constexpr mono_for_seq(const parser* self, context ctx) : self(self), ctx(std::move(ctx)) {}
	constexpr parse_result parse_mono_omit(source src) const override {
		type_parse_without_result r;
		return self->parse_without_prep(ctx, src, r);
	}
	constexpr parse_result parse_mono_check(source src) const override {
		type_check_parser r;
		return self->parse_without_prep(ctx, src, r);
	}
	constexpr parse_result parse_mono(source src, result& r) const override {
		return self->parse_without_prep(ctx, src, r);
	}
};

constexpr auto mk_mono(const auto* parser, auto ctx, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
	return mono_for_seq<std::decay_t<decltype(*parser)>, decltype(ctx), decltype(src), std::decay_t<decltype(result)>>( parser, ctx );
}

}


namespace ascip_details::prs {

template<string_literal message, parser type> struct seq_error_parser ;
template<typename... parsers> struct opt_seq_parser : base_parser<opt_seq_parser<parsers...>> {
	tuple<parsers...> seq;

	constexpr opt_seq_parser() =default ;
	constexpr opt_seq_parser(tuple<parsers...> t) : seq(std::move(t)) {}
	constexpr opt_seq_parser(auto&&... args) requires (sizeof...(parsers) == sizeof...(args)) : seq( static_cast<decltype(args)&&>(args)... ) {}
	constexpr opt_seq_parser(const opt_seq_parser&) =default ;

	template<typename type> constexpr static auto is_type_checker = [](const auto* p){ return type_dc<decltype(*p)> == type_dc<type>; };
	template<template<typename...>class tmpl> constexpr static auto is_spec_checker = [](const auto* p) { return is_specialization_of<std::decay_t<decltype(*p)>, tmpl>; };
	template<typename type> constexpr static bool _exists_in(auto&& ch) {
		return exists_in((type*)nullptr, ch, [](const auto* p){
			return requires{ p->seq; } && !requires{ static_cast<const opt_seq_parser*>(p); };
		});
	}
	template<typename type> constexpr static bool is_field_separator = _exists_in<type>([](const auto* p){return requires{ static_cast<const seq_inc_rfield*>(p); };});
	template<typename type> constexpr static bool is_inc_field_val = _exists_in<type>(is_spec_checker<seq_inc_rfield_val>);
	template<typename type> constexpr static bool is_num_field_val = _exists_in<type>(is_spec_checker<seq_num_rfield_val>);
	template<typename type> constexpr static bool is_inc_field_after = _exists_in<type>(is_spec_checker<seq_inc_rfield_after>);
	template<typename type> constexpr static bool is_inc_field_before = _exists_in<type>(is_spec_checker<seq_inc_rfield_before>);
	template<typename type> constexpr static bool is_dec_field_after = _exists_in<type>(is_spec_checker<seq_dec_rfield_after>);
	template<typename type> constexpr static bool is_dec_field_before = _exists_in<type>(is_spec_checker<seq_dec_rfield_before>);
	template<typename... types> constexpr static bool is_struct_requires =
		((is_field_separator<types> + ...) + (is_inc_field_val<types> + ...) +
		 (is_inc_field_after<types> + ...) + (is_inc_field_before<types> + ...) +
		 (is_dec_field_after<types> + ...) + (is_dec_field_before<types> + ...) +
		 (is_num_field_val<types> + ...)
		) > 0;
	constexpr static bool is_struct_requires_pd = is_struct_requires<parsers...>;
	constexpr static bool is_recursion_enabled = (_exists_in<parsers>(is_type_checker<seq_enable_recursion_parser>) + ...) ;

	template<typename type> constexpr static bool is_def_parser = _exists_in<type>(is_spec_checker<def_parser>);
	template<typename type> constexpr static bool is_must_parser = _exists_in<type>(prs::is_must_parser);

	template<typename type> constexpr static int num_field_val() { return grab_num_val<type, opt_seq_parser, seq_num_rfield_val>(); }
	template<typename type> constexpr static auto inc_field_val() { return grab_num_val<type, opt_seq_parser, seq_inc_rfield_val>(); }

	template<auto find> constexpr auto call_parse(ascip_details::parser auto& p, auto&& ctx, auto src, auto& result) const {
		if constexpr (!is_struct_requires_pd) return p.parse(ctx, src, result);
		else if constexpr (is_field_separator<decltype(auto(p))>) return p.parse(ctx, src, result);
		else if constexpr (requires{ is_parse_non_result(result).ok; })
			return p.parse(ctx, src, result);
		else {
			return p.parse(ctx, src, ascip_reflection::get<find>(result));
		}
	}
	template<auto find, auto pind, typename cur_t, typename... tail> constexpr parse_result parse_seq(auto&& ctx, auto src, auto& result) const {
		//TODO: use -1 as last struct field, -2 as the field before last one and so on...
		constexpr auto cur_field = num_field_val<cur_t>() +
			( (find + is_inc_field_before<cur_t> + (-1*is_dec_field_before<cur_t>) + inc_field_val<cur_t>()) * !is_num_field_val<cur_t> );
		constexpr auto nxt_field = cur_field + is_inc_field_after<cur_t> + (-1*is_dec_field_after<cur_t>) + is_field_separator<cur_t>;

		if constexpr(requires{is_checking(result).ok;} && is_must_parser<cur_t>) return 0;
		auto& cur = get<pind>(seq);
		auto ret = call_parse<cur_field>(cur, ctx, src, result);
		src += ret * (0 <= ret);
		update_shift<seq_shift_stack_tag>(ctx, ret);
		if constexpr (pind+1 == sizeof...(parsers)) return ret;
		else {
			if( ret < 0 ) return ret;
			if constexpr (is_def_parser<cur_t> && requires{is_checking(result).ok;}) return ret;
			auto req = parse_seq<nxt_field, pind+1, tail...>(ctx, src, result);
			return req*(req<0) + (ret+req)*(0<=req);
		}
	}

	template<auto find, auto pind> constexpr parse_result parse_and_store_shift(auto&& ctx, auto src, auto& result) const {
		//static_assert - exists concrete in ctx
		auto* old_shift = search_in_ctx<seq_shift_stack_tag>(ctx);
		auto cur_shift = 0;
		search_in_ctx<seq_shift_stack_tag>(ctx) = &cur_shift;
		auto ret = parse_seq<find, pind, parsers...>(ctx, src, result);
		search_in_ctx<seq_shift_stack_tag>(ctx) = old_shift;
		return ret;
	}
	constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
		return parse_and_store_shift<0,0>(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return -1;
		auto nl_controller = make_new_line_count_resetter(ctx, result);
		auto shift_store = 0;
		auto cur_ctx = make_ctx<seq_shift_stack_tag, any_shift_tag>(&shift_store,
		  make_ctx<seq_result_stack_tag>(&result, ctx ) ) ;
		auto ret = parse_rswitch(cur_ctx, std::move(src), result);
		nl_controller.update(ret);
		return ret;
	}
	constexpr parse_result parse_rswitch(auto&& ctx, auto src, auto& result) const requires (!is_recursion_enabled) {
		return parse_without_prep(ctx, std::move(src), result);
	}
	constexpr parse_result parse_rswitch(auto&& ctx, auto src, auto& result) const requires is_recursion_enabled {
		using mono_type = seq_details::monomorphic<decltype(src), std::decay_t<decltype(result)>>;
		const mono_type* mono_ptr;
		auto cur_ctx = make_ctx<seq_stack_tag>(&mono_ptr, ctx) ;
		auto mono = seq_details::mk_mono(this, cur_ctx, src, result);
		mono_ptr = &mono;
		return mono.parse_mono(std::move(src), result);
	}
};
template<typename... p> opt_seq_parser(p...) -> opt_seq_parser<std::decay_t<p>...>;

} // namespace ascip_details::prs

namespace ascip_details {

template<parser... left, typename right> constexpr auto operator>>(prs::opt_seq_parser<left...>&& l, right&& r) {
    return [&]<auto... inds>(std::index_sequence<inds...>){
        return prs::opt_seq_parser<left..., std::decay_t<right>>{ std::move(get<inds>(l.seq))..., std::move(r) };
    }(std::make_index_sequence<sizeof...(left)>{});
}
template<parser left, typename right> constexpr auto operator>>(left&& l, right&& r)
	requires (!is_specialization_of<std::decay_t<left>, prs::opt_seq_parser>) {
	return prs::opt_seq_parser<std::decay_t<left>, std::decay_t<right>>{ std::forward<left>(l), std::forward<right>(r) };
}
template<parser left> constexpr auto operator>>(left&& l, char r) {
	return std::forward<decltype(l)>(l) >> prs::value_parser{ r };
}
template<parser p> constexpr auto operator++(p&& l) {
	return prs::seq_inc_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator++(p&& l,int) {
	return prs::seq_inc_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l) {
	return prs::seq_dec_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l,int) {
	return prs::seq_dec_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }

template<auto cnt, parser type> constexpr auto finc(type&& p) {
	using p_type = std::decay_t<type>;
	using inc_type = prs::_seq_rfield_val<cnt>;
	return prs::seq_inc_rfield_val<p_type, inc_type>{ std::forward<decltype(p)>(p) }; }
template<auto cnt, parser type> constexpr auto fnum(type&& p) {
	using p_type = std::decay_t<type>;
	using num_type = prs::_seq_rfield_val<cnt>;
	return prs::seq_num_rfield_val<p_type, num_type>{ std::forward<decltype(p)>(p) }; }

template<typename mutator, template<typename...>class seq_parser, typename... list>
constexpr static auto transform(seq_parser<list...>&& src, auto& ctx) requires requires{ src.seq; } {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>(transform_apply_to_each<mutator, seq_parser, 0>(std::move(src.seq), nctx), nctx);
}

}


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

constexpr static auto test_parser_char(const auto& p, auto&& s, auto pr) {
	char result='z';
	auto answer = p.parse(make_test_ctx(), make_source(s), result);
	answer /= (answer==pr);
	return result;
}

}

namespace ascip_details::prs {

template<ascip_details::parser parser> struct omit_parser : base_parser<omit_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr omit_parser() =default ;
	constexpr omit_parser(omit_parser&&) =default ;
	constexpr omit_parser(const omit_parser&) =default ;
	constexpr omit_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		type_parse_without_result r;
		return p.parse(ctx, src, r);
	}
};

constexpr auto omit(auto&& p) {
	using type = std::decay_t<decltype(p)>;
	return omit_parser<type>{ std::forward<decltype(p)>(p) };
}

template<typename p>
constexpr static bool test_omit() {
	static_assert( test_parser_char(omit(p::template char_<'a'>), "a", 1) == 'z' );
	static_assert( test_parser_char(omit(p::template char_<'a'>), "b", -1) == 'z' );
	static_assert( test_parser_char(p::template _char<'a'>, "a", 1) == 'z' );
	static_assert( test_parser_char(p::template _char<'a'>, "b", -1) == 'z' );
	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<auto val, parser parser> struct tmpl_as_parser : base_parser<tmpl_as_parser<val,parser>> {
	[[no_unique_address]] parser p;
	constexpr tmpl_as_parser() =default ;
	constexpr tmpl_as_parser(tmpl_as_parser&&) =default ;
	constexpr tmpl_as_parser(const tmpl_as_parser&) =default ;
	constexpr tmpl_as_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_parse_without_result r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		eq(shift >= 0, result, val);
		return shift;
	}
};

template<auto val, parser parser_t> constexpr auto as( parser_t&& p) {
	using type = std::decay_t<decltype(p)>;
	return tmpl_as_parser<val, type>{ std::forward<decltype(p)>(p) };
}

template<typename value_t, parser parser> struct as_parser : base_parser<as_parser<value_t, parser>> {
	value_t val;
	[[no_unique_address]] parser p;
	constexpr as_parser() =default ;
	constexpr as_parser(as_parser&&) =default ;
	constexpr as_parser(const as_parser&) =default ;
	constexpr as_parser(value_t val, parser p) : val(std::move(val)), p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_parse_without_result r;
		auto shift = p.parse(ctx, std::move(src), r);
		eq(shift >= 0, result, val);
		return shift;
	}
};

template<typename value_t, parser parser_t> constexpr auto as( parser_t&& p, value_t&& val ){
	using ptype = std::decay_t<decltype(p)>;
	using val_type = std::decay_t<decltype(val)>;
	return as_parser<val_type, ptype>( std::forward<decltype(val)>(val), std::forward<decltype(p)>(p) );
}

template<typename prs, template<auto>class t=prs::template tmpl>
constexpr static bool test_as() {
	static_assert( test_parser_char(as(t<'a'>::char_, 'b'), "a", 1) == 'b' );
	static_assert( test_parser_char(as(t<'a'>::char_, 'b'), "b", -1) == 'z' );
	static_assert( test_parser_char(as(prs::int_, 'b'), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(prs::int_), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(prs::int_), "a", -1) == 'z' );
	static_assert( []{
		auto p = t<'a'>::char_;
		return test_parser_char(as(p, 'b'), "a", 1);
	}() == 'b' );
	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename parser, typename value_type>
constexpr static auto transform_special(prs::as_parser<value_type, parser>&& src, auto&& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( prs::as_parser{ src.val, std::move(np) }, nctx );
}

template<typename mutator, typename parser, auto value>
constexpr static auto transform_special(prs::tmpl_as_parser<value, parser>&& src, auto&& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( prs::tmpl_as_parser<value, std::decay_t<decltype(np)>>{ std::move(np) }, nctx );
}

}





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

struct variant_shift_parser_tag {};
template<auto ind> struct variant_shift_parser : base_parser<variant_shift_parser<ind>>, variant_shift_parser_tag {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
		eq(result, *search_in_ctx<ind, variant_shift_tag>(ctx));
		return 0;
	}
};

struct variant_recursion_parser_tag {};
template<auto ind> struct variant_recursion_parser : base_parser<variant_recursion_parser<ind>>, variant_recursion_parser_tag {
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
			update_shift<variant_shift_tag>(ctx, ret);
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
		if constexpr (!need_modify_ctx()) return parse_ind<0>(ctx, src, result);
		else {
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
	}

	template<typename type> constexpr static auto is_type_checker = [](const auto* p){ return type_dc<decltype(*p)> == type_dc<type>; };
	template<template<typename...>class tmpl> constexpr static auto is_spec_checker = [](const auto* p) { return is_specialization_of<std::decay_t<decltype(*p)>, tmpl>; };
	template<typename type> constexpr static auto is_castable_checker = [](const auto* p){ return requires{ static_cast<const type*>(p); }; };
	template<typename type> constexpr static bool _exists_in(auto&& ch) { return exists_in((type*)nullptr, ch, [](const auto* p){ return false; }); }
	template<typename type> constexpr static bool check_contains_shift = _exists_in<type>(is_castable_checker<variant_shift_parser_tag>);
	template<typename type> constexpr static bool check_contains_recursion = _exists_in<type>(is_castable_checker<variant_recursion_parser_tag>);
	template<typename type> constexpr static bool check_use_variant_result = _exists_in<type>(is_spec_checker<use_variant_result_parser>);

	constexpr static bool need_modify_ctx() {
		return (check_contains_recursion<parsers> + ...) + (check_contains_shift<parsers> + ...) + (check_use_variant_result<parsers> + ...);
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

	static_assert( ({ char r{};run_parse(t<'a'>::char_|t<'b'>::char_, "a", r);r;}) == 'a' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b', "b", r);r;}) == 'b' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "c", r);}) == 1 );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "c", r);r;}) == 'c' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "d", r);}) == -1 );

	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "b", r);r;}) == 'c' );
	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "a", r);r;}) == 'a' );
	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "a", r);  }) ==  1 );

	static_assert( [&] {
		char r='z';
		const auto pr = run_parse(prs::nop | t<'a'>::char_, "b", r);
		return (r=='z') + 2*(pr == 0);
	}() == 3 );

	static_assert( (t<'a'>::char_ | t<'b'>::char_ >> t<0>::v_rec).need_modify_ctx() );
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

	static_assert( [&] {
		char r{};
		const auto pr = run_parse(t<'z'>::char_ | (prs::nop >> (t<'a'>::char_ | t<'b'>::char_ >> t<1>::v_rec)), "bbba", r);
		return (pr==4) + 2*(r=='a');
	}() == 3 );

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

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<parser left, parser right> struct different_parser : base_parser<different_parser<left, right>> {
	left lp;
	right rp;
	constexpr different_parser( left l, right r ) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_check_parser fake_result;
		if(rp.parse(ctx, src, fake_result) >= 0) return -1;
		return lp.parse(ctx, src, result);
	}
};
template<parser left, parser right> different_parser(left, right) -> different_parser<left, right>;

template<parser left, parser right> constexpr auto operator-(left&& l, right&& r) {
	return different_parser( std::forward<decltype(l)>(l), std::forward<decltype(r)>(r)); }

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_different() {


	static_assert( [] {
		char r{};
		parse_result nls=0;
		auto ctx = make_ctx<new_line_count_tag>(&nls);
		auto parser = +(a::any - t<'\n'>::char_);
		auto pr = parser.parse(ctx, a::make_source("ab\n"), r);
		return (pr==2) + 2*(nls==0);
	}() == 3 );
	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename left_type, typename right_type>
constexpr static auto transform(prs::different_parser<left_type, right_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto lp = transform<mutator>(std::move(src.lp), nctx);
	auto rp = transform<mutator>(std::move(src.rp), nctx);
	return transform_apply<mutator>(prs::different_parser( std::move(lp), std::move(rp) ), nctx);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {
template<typename type> concept optional = requires(type& o){ o.emplace(); o.reset(); };
}

namespace ascip_details::prs {

template<ascip_details::parser parser> struct opt_parser : base_parser<opt_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr opt_parser(opt_parser&&) =default ;
	constexpr opt_parser(const opt_parser&) =default ;
	constexpr opt_parser() =default ;
	constexpr opt_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		using result_type = std::decay_t<decltype(result)>;

		if(!src) return 0;
		if constexpr(optional<result_type>) {
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

template<parser p> constexpr auto operator-(p&& _p) { return opt_parser<std::decay_t<p>>{ _p }; }

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_optional() {
	static_assert( test_parser_char(-t<'a'>::char_, "b", 0) == 'z' );
	static_assert( test_parser_char(-t<'a'>::char_, "a", 1) == 'a' );
	return true;
}
} // namespace ascip_details::prs






namespace ascip_details::prs {

template<parser parser> struct negate_parser : base_parser<negate_parser<parser>> {
	parser p;
	constexpr negate_parser() =default ;
	constexpr negate_parser(negate_parser&&) =default ;
	constexpr negate_parser(const negate_parser&) =default ;
	constexpr negate_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto nctx = make_ctx<inverted_result_tag>(1, ctx);
		auto ret = p.parse(nctx, std::move(src), result);
		//TODO: BUG: !(char_<'a'> >> char_<'b'>) returns 1 ?
		return ret * (-1); //TODO: what if the ret is 0?
	}
};

constexpr auto operator!(parser auto&& p) {
	using p_type = std::decay_t<decltype(p)>;
	if constexpr (is_specialization_of<p_type, negate_parser>) return p.p;
	else return negate_parser<p_type>{ std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_negate() {

	static_assert( ({char r{};(!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);}) == -1 );
	static_assert( ({char r{};(!t<'a'>::char_).parse(make_test_ctx(), make_source('b'), r);}) ==  1 );
	static_assert( ({char r='z';(!t<'a'>::char_).parse(make_test_ctx(), make_source('b'), r);r;}) ==  'z' );

	static_assert( ({char r{};(!!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);}) == 1 );
	static_assert( ({char r{};(!!!!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);r;}) == 'a' );


	static_assert( [] {
		int nls=0; char r{};
		auto ctx = make_ctx<new_line_count_tag>(&nls);
		auto pr = (!t<'\n'>::char_).parse(ctx, a::make_source("\n"), r);
		return (pr<0) + 2*(nls==0);
	}() == 3 );
	static_assert( [] {
		int nls=0; char r{};
		auto ctx = make_ctx<new_line_count_tag>(&nls);
		auto pr = (!t<'a'>::char_).parse(ctx, a::make_source("\n"), r);
		return (pr==1) + 2*(nls==1);
	}() == 3 );

	static_cast<const decltype(auto(t<'a'>::char_))&>(!!t<'a'>::char_);
	static_cast<const decltype(auto(t<'a'>::char_))&>(!!!!t<'a'>::char_);
	static_cast<const opt_parser<negate_parser<char_parser<'a'>>>&>(-(!t<'a'>::char_));
	static_cast<const opt_parser<char_parser<'a'>>&>(-(!!t<'a'>::char_));

	return true;
}

}

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs {

struct unary_list_shift_tag {};

template<auto ind> struct unary_list_shift_parser : base_parser<unary_list_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
		eq(result, *search_in_ctx<ind, unary_list_shift_tag>(ctx));
		return 0;
	}
};

template<parser parser> struct unary_list_parser : base_parser<unary_list_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr unary_list_parser(unary_list_parser&&) =default ;
	constexpr unary_list_parser(const unary_list_parser&) =default ;
	constexpr unary_list_parser() =default ;
	constexpr unary_list_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		parse_result shift_storage = 0;
		auto nctx = make_ctx<list_shift_tag, unary_list_shift_tag, any_shift_tag>(&shift_storage, ctx);
		parse_result ret = -1;
		parse_result cur_r = 0;
		while (src && 0<=cur_r) {
			cur_r = call_parse(nctx, src, result);
			shift_storage = cur_r;
			ret += cur_r * (0<=cur_r);
			src += cur_r * (0<=cur_r);
		}
		if(src) pop(result);
		return (ret+1)*(0<=ret) + ret*(ret<0);
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return p.parse(ctx, src, result.o);
		else return p.parse(ctx, src, empback(result));
	}
};

template<parser p> constexpr auto operator+(p&& _p) {
	return unary_list_parser<std::decay_t<p>>{ _p };
}

template<parser p> constexpr auto operator*(p&& _p) {
	return -( +(std::forward<decltype(_p)>(_p)) );
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs {

struct binary_list_shift_tag {};

template<auto ind> struct binary_list_shift_parser : base_parser<binary_list_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
		eq(result, *search_in_ctx<ind, binary_list_shift_tag>(ctx));
		return 0;
	}
};

template<parser left, parser right>
struct binary_list_parser : base_parser<binary_list_parser<left, right>> {
	left lp;
	right rp;
	constexpr binary_list_parser(left l, right r) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		parse_result shift_storage=0;
		auto nctx = make_ctx<binary_list_shift_tag, list_shift_tag, any_shift_tag>(&shift_storage, ctx);
		type_parse_without_result fake_result;
		parse_result skip=0, cur = 0, ret = -1;
		while (skip >= 0) {
			src += skip;
			cur = call_parse(nctx, src, result);
			if( cur <= 0 ) {
				pop(result);
				break;
			}
			ret += skip + cur;
			src += cur;
			skip = rp.parse(nctx, src, fake_result);
			shift_storage = skip*(skip>=0) + cur*(cur>=0);
		}
		return (ret+1)*(0<=ret) + ret*(ret<0);
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return lp.parse(ctx, src, result.o);
		else return lp.parse(ctx, src, empback(result));
	}
};

template<parser left, parser right> constexpr auto operator%(left&& l, const right& r) {
	return binary_list_parser( std::forward<decltype(l)>(l), std::forward<decltype(r)>(r) );
}

constexpr auto operator%(parser auto&& l, char r) {
	return binary_list_parser( std::forward<decltype(l)>(l), value_parser{r} );
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename left_type, typename right_type>
constexpr static auto transform(prs::binary_list_parser<left_type, right_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto lp = transform<mutator>(std::move(src.lp), nctx);
	auto rp = transform<mutator>(std::move(src.rp), nctx);
	return transform_apply<mutator>(prs::binary_list_parser( std::move(lp), std::move(rp) ), nctx);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs {

template<parser parser> struct reparse_parser : base_parser<reparse_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto nlr = make_new_line_count_resetter(ctx, result);
		auto r = p.parse(static_cast<decltype(ctx)&&>(ctx), src, result);
		return r * (r<0);
	}
};

template<parser type> constexpr auto reparse(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return reparse_parser<ptype>{ {}, std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_reparse() {
	static_assert( []{
		char r;
		auto p = (reparse(a::any) >> a::any).parse(a::make_test_ctx(), a::make_source("ab"), r);
		return (p==1) + 2*(r=='a');
	}() == 3);
	static_assert( []{
		char r='z';
		auto p = (reparse(t<'b'>::char_) >> a::any).parse(a::make_test_ctx(), a::make_source("ab"), r);
		return (p==-1) + 2*(r=='z');
	}() == 3);
	static_assert( [] {
		char r{}; int nls=7;
		auto ctx = make_ctx<new_line_count_tag>(&nls);
		auto p = reparse(t<'\n'>::char_).parse(ctx, a::make_source("\n"), r);
		return (p==0) + 2*(nls==7);
	}() == 3);
	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<typename good_result, parser parser>
struct result_checker_parser : base_parser<result_checker_parser<good_result, parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert(
			   requires{is_parse_non_result(result).ok;}
			|| std::is_same_v<good_result, std::decay_t<decltype(result)>>
			, "can only parser to required type" );
		return p.parse(static_cast<decltype(ctx)&&>(ctx), std::move(src), result);
	}
};
template<typename needed, parser type> struct cast_parser : base_parser<cast_parser<needed,type>> {
	type p;
	constexpr static auto& check_result(auto& result) {
		if constexpr( requires{is_parse_non_result(result).ok;} ) return result;
		else {
			static_assert(requires{ static_cast<needed&>(result); }, "the result must to be castable to needed type" );
			return static_cast<needed&>(result);
		}
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(ctx, src, check_result(result));
	}
};

template<typename result, parser type> constexpr auto check(type&& p) {
	return result_checker_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename result, parser type> constexpr auto cast(type&& p){
	return cast_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_checkers() {
  constexpr auto ca = t<'a'>::char_;
	//NOTE: for c++23 we can only test if it works and cannot test if it fails
	static_assert( requires(char& r) {
		check<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(char& r) {
		cast<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(type_parse_without_result& r) {
		check<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(type_check_parser& r) {
		cast<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	return true;
}

}

namespace ascip_details {
template<typename mutator, typename type, typename parser>
constexpr static auto transform_special(prs::cast_parser<type,parser>&& src, auto& ctx) {
	return transform_apply<mutator>(cast<type>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
template<typename mutator, typename type, typename parser>
constexpr static auto transform_special(prs::result_checker_parser<type,parser>&& src, auto& ctx) {
	return transform_apply<mutator>(check<type>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs {

struct rvariant_stack_tag {};
struct rvariant_cpy_result_tag {};
struct rvariant_shift_tag {};

template<typename, parser...> struct rvariant_parser;

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



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
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

struct rvariant_lrec_parser : base_parser<rvariant_lrec_parser> {
  constexpr static bool is_special_info_parser=true;
  constexpr parse_result parse(auto&& ctx, auto, auto& result) const {
    if constexpr (!requires{ is_parse_non_result(result).ok; }) {
      result = std::move(*search_in_ctx<rvariant_cpy_result_tag>(ctx));
      search_in_ctx<rvariant_cpy_result_tag>(ctx) = nullptr;
    }
    return 0;
  }
};

template<auto stop_ind>
struct rvariant_rrec_parser : base_parser<rvariant_rrec_parser<stop_ind>> {
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    if(!src) return 0;
    auto* var = *search_in_ctx<rvariant_stack_tag>(ctx);
    return var->call_parse(stop_ind+1, std::move(src), result);
  }
};

struct rvariant_rrec_pl_parser : base_parser<rvariant_rrec_pl_parser> {
  constexpr static parse_result parse(auto&&, auto, auto&) { return 0; }
};

template<auto ind> struct rvariant_rec_parser : base_parser<rvariant_rec_parser<ind>> {
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    auto* var = *search_in_ctx<ind, rvariant_stack_tag>(ctx);
    return var->call_parse(0, std::move(src), result);
  }
} ;

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<parser parser> struct rvariant_top_result_parser : base_parser<rvariant_top_result_parser<parser>> { parser p; };
template<typename p> rvariant_top_result_parser(p) -> rvariant_top_result_parser<p>;

template<parser type> constexpr auto rv_result(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return rvariant_top_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) };
}

template<template<class>class wrapper, parser parser> constexpr static auto is_top_result_parser_helper(const wrapper<parser>& p) -> decltype(p.p);
template<typename parser>
constexpr static bool is_top_result_parser() {
	if constexpr (requires{ is_top_result_parser_helper(std::declval<parser>()); })
		return
			   is_top_result_parser<decltype(is_top_result_parser_helper(std::declval<parser>()))>()
			|| is_specialization_of<parser, rvariant_top_result_parser>;
	else return is_specialization_of<parser, rvariant_top_result_parser>;
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs::rv_utils {

template<typename cur_parser_t> constexpr bool contains_recursion() {
	auto checker = [](const auto* p){return std::is_same_v<std::decay_t<decltype(*p)>, rvariant_lrec_parser>;};
	auto stop = [](const auto* p){
		const bool is_rv = requires{ p->maker; };
		return is_rv && !is_specialization_of<cur_parser_t, rvariant_parser>;
	};
	return !exists_in((cur_parser_t*)nullptr, checker, stop);
}

}


namespace ascip_details::prs {

template<auto ind> struct rvariant_shift_parser : base_parser<rvariant_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
	constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
		eq(result, *search_in_ctx<ind, rvariant_shift_tag>(ctx));
		return 0;
	}
};

template<typename maker_type, parser... parsers>
struct rvariant_parser : base_parser<rvariant_parser<maker_type, parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;

	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	constexpr static auto size() { return sizeof...(parsers); }
	template<auto ind> constexpr static bool is_term() { return rv_utils::contains_recursion<__type_pack_element<ind, parsers...>>(); }
	template<auto ind> consteval static auto cur_ind() {
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		if constexpr (is_top_result_parser<cur_parser_t>()) return -1;
		else {
			auto cur = 0;
			auto cnt = 0;
			(void)( ((ind==cnt)||(++cnt,cur+=!is_top_result_parser<parsers>(),false)) || ... );
			return cur;
		}
	}
	constexpr auto move_result(auto& result) const {
		if constexpr (requires{is_parse_non_result(result).ok;}) return result;
		else return maker(result);
	}
	template<auto ind> constexpr parse_result parse_term(auto&& ctx, auto src, auto& result) const {
		if constexpr (ind < 0 || sizeof...(parsers) < ind) return -1;
		else if constexpr (!is_term<ind>()) return parse_term<ind-1>(ctx, src, result);
		else {
			auto cur = get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(result));
			if(cur < 0) return parse_term<ind-1>(ctx, src, result);
            update_shift<rvariant_shift_tag>(ctx, cur);
			return cur;
		}
	}
	template<auto ind, auto stop_pos> constexpr parse_result parse_nonterm(auto&& ctx, auto src, auto& result, auto shift) const {
		if(!src) return shift;
		if constexpr (ind < stop_pos) return shift;
		else if constexpr (is_term<ind>()) {
			if constexpr (ind == 0) return shift;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, shift);
		}
		else {
			type_check_parser result_for_check;
			parse_result prev_pr = 0;
			while( get<ind>(seq).parse(ctx, src, result_for_check) >= 0 ) {
				auto cur = move_result(result);
				if constexpr (!requires{is_parse_non_result(result).ok;})
					search_in_ctx<rvariant_cpy_result_tag>(ctx) = &cur;
				auto pr = get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(result));
				src += pr / (pr>=0);
				prev_pr += pr;
                update_shift<rvariant_shift_tag>(ctx, pr);
			}
			auto total_shift = shift + prev_pr*(prev_pr>0);
			if constexpr (ind==0) return total_shift;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, total_shift);
		}
	}
	template<auto stop_pos> constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
		auto term_r = parse_term<sizeof...(parsers)-1>(ctx, src, result);
		if(term_r < 0) return term_r;
		auto nonterm_r = parse_nonterm<sizeof...(parsers)-1, stop_pos>(ctx, src += term_r, result, 0);
		return term_r + (nonterm_r*(nonterm_r>0));
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		using copied_result_type = decltype(move_result(result));
		using mono_type = rv_utils::monomorphic<decltype(src), std::decay_t<decltype(result)>>;
		const mono_type* mono_ptr;
		parse_result shift_storage=0;
		auto rctx = make_ctx<rvariant_shift_tag, any_shift_tag>(&shift_storage,
			make_ctx<rvariant_stack_tag>(&mono_ptr,
			make_ctx<rvariant_cpy_result_tag>((copied_result_type*)nullptr, ctx) ) );
		auto mono = rv_utils::mk_mono(this, rctx, src, result);
		mono_ptr = &mono;
		return mono.parse_mono(0, src, result);
	}
};

template<typename... t> rvariant_parser(t...) ->  rvariant_parser<t...>;

}

namespace ascip_details::prs {

template<auto ind>
struct rvariant_mutator {
	struct context{};
	constexpr static auto create_ctx() { return context{}; }
	constexpr static auto create_ctx(auto&&,auto&&) { return context{}; }
	template<typename type>
	constexpr static auto apply(rvariant_rrec_pl_parser&&,auto&&) {
		return rvariant_rrec_parser<ind>{};
	}
};

template<parser type, parser... types> constexpr auto rv(auto&& maker, type&& first, types&&... list) {
	return [&maker]<auto... inds>(std::index_sequence<inds...>, auto&&... parsers) {
		return rvariant_parser(
				std::move(maker),
				transform<rvariant_mutator<inds>>(std::move(parsers))...
				);
	}(
			std::make_index_sequence<sizeof...(list)+1>{}
		, std::forward<decltype(first)>(first)
		, std::forward<decltype(list)>(list)...
	);
}

}

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

template<typename mutator, typename type, typename... parsers>
constexpr static auto transform(prs::rvariant_parser<type, parsers...>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>( transform_apply_to_each<mutator,prs::rvariant_parser,0>( std::move(src.seq),nctx,std::move(src.maker)), nctx );
}

}

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
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<typename parser, typename tag, typename value_type> struct ctx_change_parser : base_parser<ctx_change_parser<parser, tag, value_type>> {
	value_type value;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto new_ctx = make_ctx<tag>(value, ctx);
		return p.parse(new_ctx, src, result);
	}
};

template<typename parser, typename act_type, typename... tags> struct ctx_use_parser : base_parser<ctx_use_parser<parser, act_type, tags...>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		if(-1 < ret) act(result, search_in_ctx<0, tags>(ctx)...);
		return ret;
	}
};

template<typename parser, typename act_type, typename... tags> struct ctx_use_as_result_parser : base_parser<ctx_use_as_result_parser<parser, act_type, tags...>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, search_in_ctx<0, tags>(ctx)...);
		if(-1 < ret) act(result, search_in_ctx<0, tags>(ctx)...);
		return ret;
	}
};

} // namespace ascip_details::prs

namespace ascip_details {

template<typename tag, typename value_type, parser type> constexpr auto add_to_ctx(value_type&& value, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return prs::ctx_change_parser<type, tag, value_type>{ {}, std::forward<decltype(value)>(value), std::forward<decltype(p)>(p) };
}
template<typename... tags, parser type> constexpr auto from_ctx(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return prs::ctx_use_parser<ptype, act_type, tags...>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}
template<typename... tags, parser type> constexpr auto result_from_ctx(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return prs::ctx_use_as_result_parser<ptype, act_type, tags...>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename mutator, typename parser, typename tag, typename value_type>
constexpr static auto transform(prs::ctx_change_parser<parser, tag, value_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( prs::ctx_change_parser<decltype(np), tag, value_type>{ {}, std::move(src.value), std::move(np) }, nctx );
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs {

template<parser pt> struct lexeme_parser : base_parser<lexeme_parser<pt>> { [[no_unique_address]] pt p;
	constexpr lexeme_parser() =default ;
	constexpr lexeme_parser(lexeme_parser&&) =default ;
	constexpr lexeme_parser(const lexeme_parser&) =default ;
	constexpr lexeme_parser(pt p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result);
	}
};
template<parser pt> struct skip_parser : base_parser<skip_parser<pt>> { [[no_unique_address]] pt p;
	constexpr skip_parser() =default ;
	constexpr skip_parser(skip_parser&&) =default ;
	constexpr skip_parser(const skip_parser&) =default ;
	constexpr skip_parser(pt p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result);
	}
};

template<parser skip, parser base> struct injected_parser : base_parser<injected_parser<skip,base>> {
	[[no_unique_address]] skip s;
	[[no_unique_address]] base b;
	static type_parse_without_result skip_result;
	constexpr injected_parser() noexcept =default ;
	constexpr injected_parser(injected_parser&&) noexcept =default ;
	constexpr injected_parser(const injected_parser&) noexcept =default ;
	constexpr injected_parser(skip s, base b) : s(std::forward<decltype(s)>(s)), b(std::forward<decltype(b)>(b)) {}
	constexpr static auto& get_result_for_skipper(auto& result) {
		if constexpr(requires{is_checking(result).ok;}) return result;
		else return skip_result;
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto sr = s.parse(ctx, src, get_result_for_skipper(result));
		sr *= (0<=sr);
		src += sr;
		if constexpr (!can_read_after_end<decltype(src)>()) if(!src) return -1;
		auto mr = b.parse(ctx, src, result);
		return (sr * (0<=mr)) + mr; // 0<=mr ? mr+sr : mr;
	}
};

template<parser skip, parser base> type_parse_without_result injected_parser<skip, base>::skip_result{};

} // namespace ascip_details::prs

namespace ascip_details {

template<parser type> constexpr auto lexeme(type&& p) {
	return prs::lexeme_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) };
}

template<parser type> constexpr auto skip(type&& p) {
	return prs::skip_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) };
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)









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

namespace ascip_details {

template<parser parser_type, parser skipper>
constexpr auto inject_skipping(parser_type&& to, skipper&& what) {
	using mutator = prs::injection_mutator<std::decay_t<skipper>>;
	auto ret = transform<mutator>(std::move(to));
	if constexpr(ascip_details::is_specialization_of<std::decay_t<decltype(ret)>, prs::injected_parser>)
		return ret.b;
	else return ret;
}

template<parser p1, parser p2> constexpr auto make_injected(const p1& l, const p2& r) {
	return prs::injected_parser<p1, p2>( l, r );
}

namespace prs {
template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_injection_parser() {
	constexpr auto cs = t<' '>::char_;
	constexpr auto ca = t<'a'>::char_;

	static_assert( ({ char r='z'; make_injected(cs, ca).parse(make_test_ctx(), make_source(" a"), r);
	}) == 2, "inejction parser can parse");
	static_assert( ({ char r='z'; make_injected(cs, ca).parse(make_test_ctx(), make_source("  b"), r);
	}) == -1, "inejction parser cannot parse invalid data");
	static_assert( ({ char r='z';
		make_injected(cs, ca).parse(make_test_ctx(), make_source(" a"), r);
	r;}) == 'a', "the first parameter of inejction parser skipped");
	static_assert( ({ char r='z';
		make_injected(cs, ca).parse(make_test_ctx(), make_source("ba"), r);
	}) == -1, "inejction parser fails if cannot parse first parameter");
	static_assert( [&]{ char r='z';
		auto pr=make_injected(cs, ca).parse(make_test_ctx(), make_source("a"), r);
		return (pr==1) + (2*(r=='a'));}() == 3, "inejction parser parse if only first parameter fails");
	static_assert( [&]{ char r='z';
		auto pr=make_injected(cs, ca).parse(make_test_ctx(), make_source("aa"), r);
		return (pr==1) + (2*(r=='a'));}() == 3, "inejction parser parse if only first parameter fails");

	return true;
}
template<typename a, auto p1, auto p2, template<auto>class tt=a::template tmpl>
constexpr static bool test_seq_injection() {
	//TODO: test something like skip(++lexeme(parser))([](auto& v){return &v;})
	constexpr auto cs = tt<' '>::char_;
	constexpr auto ca = tt<'a'>::char_;
	constexpr auto cb = tt<'b'>::char_;

	using p1_t = decltype(auto(p1));
	using p2_t = decltype(auto(p2));
	using inj_t = injected_parser<p2_t,p1_t>;

	(void)static_cast<const p1_t&>(inject_skipping(p1, p2));
	(void)static_cast<const opt_seq_parser<inj_t, inj_t>&>(inject_skipping(p1 >> p1, p2));

	(void)static_cast<const opt_seq_parser<p1_t, p1_t, p1_t>&>(inject_skipping(lexeme(p1 >> p1 >> p1), p2));
	(void)static_cast<const opt_seq_parser<p1_t, opt_seq_parser<p1_t, p1_t>>&>(inject_skipping(lexeme(p1 >> lexeme(p1 >> p1)), p2));
	(void)static_cast<const opt_seq_parser<p1_t, injected_parser<p2_t, opt_seq_parser<p1_t, p1_t>>>&>(inject_skipping(lexeme(p1 >> skip(lexeme(p1 >> p1))), p2));
	(void)static_cast<const opt_seq_parser<p1_t, injected_parser<p2_t, opt_seq_parser<inj_t, inj_t>>>&>(inject_skipping(lexeme(p1 >> skip(lexeme(skip(p1 >> p1)))), p2));
	(void)static_cast<const opt_seq_parser<inj_t, injected_parser<p2_t, opt_seq_parser<p1_t, p1_t>>>&>(inject_skipping(lexeme(p1) >> lexeme(p1 >> p1), p2));
	(void)static_cast<const opt_seq_parser<p1_t, opt_seq_parser<inj_t, inj_t>>&>(
			inject_skipping(lexeme(p1 >> skip(p1 >> p1)), p2));

	constexpr auto lambda_ret_val = [](auto&v){return &v;};
	using lambda_ret_val_t = std::decay_t<decltype(lambda_ret_val)>;
	(void)static_cast<const semact_parser<injected_parser<p2_t, opt_seq_parser<p1_t, opt_seq_parser<inj_t, inj_t>>>, lambda_ret_val_t>&>(inject_skipping(lexeme(p1 >> skip(p1 >> lexeme(p1)))(lambda_ret_val), p2));

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


	/* TODO: remove support of lambda in seq?
	static_assert( ({ char r='z'; int t=0;
		auto p = ca >> cb >> [&t](...){++t;return 0;};
		p.parse(make_test_ctx(), make_source("ab"), r);
	t;}) == 1, "injection works win lambda in seq");
	static_assert( ({ char r='z'; int t=0;
		auto p = ca >> cb([&t](...){++t;});
		p.parse(make_test_ctx(), make_source("ab"), r);
	t; }) == 1, "injection works with semact" );
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping(ca >> cb([&t](...){++t;}), +a::space);
		p.parse(make_test_ctx(), make_source("ab"), r);
	t; }) == 1, "injection works with semact" );
	*/
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping(ca >> cast<char>(cb([&t](...){++t;})), +a::space);
		p.parse(make_test_ctx(), make_source("abc"), r);
	t; }) == 1, "injection works with semact" );


	(void)static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping( p1|p1, p2 ));
	(void)static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping( p1|p1|p1, p2 ));
	(void)static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		result_checker_parser<char,inj_t>
		>&>(inject_skipping( check<char>(p1)|check<char>(p1), p2 ));

	(void)static_cast<const result_checker_parser<int, inj_t>&>(inject_skipping( check<int>(p1), p2 ));
	(void)static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( check<int>(p1 >> p1), p2 ));
	(void)static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,seq_num_rfield_val<inj_t, _seq_rfield_val<2>>>>&>(inject_skipping( check<int>(p1 >> fnum<2>(p1)), p2 ));
	{
		auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
		auto var = rv(rmaker, p1, p1);
		(void)static_cast<rvariant_parser<decltype(rmaker), p1_t, p1_t>&>(var);
		(void)static_cast<const rvariant_parser<decltype(rmaker), inj_t, inj_t>&>(inject_skipping(var, p2));
	}


	static_assert( ({ char r{}; const auto parser = +a::alpha; const auto skipper = +a::space;
		inject_skipping(parser, skipper).parse(make_test_ctx(), make_source(" a b c "), r); }) == -1 );

	(void)static_cast<const binary_list_parser<inj_t, inj_t>&>(inject_skipping( p1 % p1, p2 ));

	return true;
}

template<typename a>
constexpr static bool test_injection() {
	return test_injection_parser<a>() && test_seq_injection<a, a::template char_<'a'>, a::space>();
}

}
}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<auto ind> struct any_shift_parser : base_parser<any_shift_parser<ind>> {
	constexpr static bool is_special_info_parser=true;
  constexpr static parse_result parse(auto&& ctx, auto, auto& result) {
    eq(result, *search_in_ctx<ind, any_shift_tag>(ctx));
    return 0;
  }
};

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_any_shift() {
  struct result{ char s{}; int shift{}; };
  static_assert( [] { result r;
    auto pr = parse(t<'a'>::char_ >> ++t<0>::any_shift, +p::space, p::make_source("  a"), r);
    return (pr==3) + 2*(r.shift==3);
  }() == 3 );
  static_assert( [] { result r;
    auto pr = parse(p::seq_enable_recursion >> t<'a'>::char_ >> ++t<0>::any_shift, +p::space, p::make_source("  a"), r);
    return (pr==3) + 2*(r.shift==3);
  }() == 3 );
  return true;
}

}

struct ascip {
  using parse_result = ascip_details::parse_result;
  template<typename t> using base_parser = ascip_details::prs::base_parser<t>;

  static constexpr auto nop = ascip_details::prs::nop_parser{};
  template<auto s> static constexpr auto char_ = ascip_details::prs::char_parser<s>{};
  template<auto val> constexpr static auto _char = omit(char_<val>);
  template<ascip_details::string_literal s> static constexpr auto lit = ascip_details::prs::literal_parser<s>{};
  static constexpr auto space = ascip_details::prs::space_parser{};
  static constexpr auto any = ascip_details::prs::any_parser{};
  constexpr static auto nl = _char<'\n'>;

  static constexpr auto int_ = ascip_details::prs::int_parser{};
  template<auto base=10> constexpr static auto uint_ = ascip_details::prs::uint_parser<base>{};
  static constexpr auto fp = ascip_details::prs::float_point_parser{};

  // ranges
  constexpr static auto lower = ascip_details::prs::range_parser<'a','z'>{};
  constexpr static auto upper = ascip_details::prs::range_parser<'A','Z'>{};
  constexpr static auto digit = ascip_details::prs::range_parser<'0','9'>{};
  constexpr static auto d10 = ascip_details::prs::range_parser<'0', '9'>{};
  constexpr static auto ascii = ascip_details::prs::range_parser<(char)0x01,(char)0x7F>{};
  constexpr static auto alpha = lexeme(lower | upper);

  // sequence
  constexpr static auto cur_pos = ascip_details::prs::cur_pos_parser{};
  template<auto ind> constexpr static auto seq_shift = ascip_details::prs::seq_shift_parser<ind>{};
  template<auto ind> constexpr static auto rec = ascip_details::prs::seq_recursion_parser<ind>{};
  constexpr static auto seq_enable_recursion = ascip_details::prs::seq_enable_recursion_parser{};
  constexpr static ascip_details::prs::seq_inc_rfield sfs{} ;

  // functions
  constexpr static auto make_default_context(auto&&... args) { return ascip_details::make_default_context(std::forward<decltype(args)>(args)...); }
  constexpr static auto make_test_ctx(auto&&... args) { return ascip_details::make_test_ctx(std::forward<decltype(args)>(args)...); }
  constexpr static auto make_source(auto&& src) { return ascip_details::make_source(std::forward<decltype(src)>(src)); }
  constexpr static auto inject_skipping(auto&& p, auto&& s) { return ascip_details::inject_skipping(std::forward<decltype(p)>(p), std::forward<decltype(s)>(s) ); }
  constexpr static auto fwd(auto& o) { return ascip_details::fwd( o ); }
  constexpr static void write_out_error_msg(auto& os, auto fn, auto msg, auto expt, auto src, auto ln ) {
    ascip_details::write_out_error_msg(os, std::move(fn), std::move(msg), std::move(expt), std::move(src), std::move(ln));
  }

  // rvariant
  constexpr static ascip_details::prs::rvariant_lrec_parser rv_lrec{};
  constexpr static ascip_details::prs::rvariant_rrec_pl_parser rv_rrec{};
  template<auto stop_ind> constexpr static ascip_details::prs::rvariant_rrec_parser<stop_ind> _rv_rrec{};
  template<auto ind> constexpr static auto rv_rec = ascip_details::prs::rvariant_rec_parser<ind>{};
  template<auto ind> constexpr static auto v_rec  = ascip_details::prs::variant_recursion_parser<ind>{};

  // shifts
  template<auto ind> constexpr static auto ulist_shift = ascip_details::prs::unary_list_shift_parser<ind>{};
  template<auto ind> constexpr static auto blist_shift = ascip_details::prs::binary_list_shift_parser<ind>{};
  template<auto ind> constexpr static auto variant_shift = ascip_details::prs::variant_shift_parser<ind>{};
  template<auto ind> constexpr static auto rv_shift = ascip_details::prs::rvariant_shift_parser<ind>{};
  template<auto ind> constexpr static auto any_shift = ascip_details::prs::any_shift_parser<ind>{};
  template<auto ind> constexpr static auto shift = ascip_details::prs::any_shift_parser<ind>{};

  constexpr static auto dquoted_string = lexeme(_char<'"'> >> *(as<'"'>(char_<'\\'> >> char_<'"'>)| (ascip::any - char_<'"'>)) >> _char<'"'>);
  constexpr static auto squoted_string = lexeme(_char<'\''> >> *(as<'\''>(char_<'\\'> >> char_<'\''>)| (ascip::any - char_<'\''>)) >> _char<'\''>);
  constexpr static auto quoted_string = lexeme(squoted_string | dquoted_string);

  // helpers
  template<auto s> struct tmpl {
    constexpr static auto& nop = ascip::nop;
    constexpr static auto& char_ = ascip::char_<s>;
    constexpr static auto& _char = ascip::_char<s>;
    constexpr static auto& space = ascip::space;
    constexpr static auto& any = ascip::any;
    constexpr static auto& nl = ascip::nl;
    constexpr static auto& rec = ascip::rec<s>;
    constexpr static auto& _rv_rrec = ascip::_rv_rrec<s>;
    constexpr static auto& rv_rec = ascip::rv_rec<s>;
    constexpr static auto& v_rec = ascip::v_rec<s>;
    constexpr static auto& uint_ = ascip::uint_<s>;
    constexpr static auto& seq_shift = ascip::seq_shift<s>;
    constexpr static auto& ulist_shift = ascip::ulist_shift<s>;
    constexpr static auto& blist_shift = ascip::blist_shift<s>;
    constexpr static auto& variant_shift = ascip::variant_shift<s>;
    constexpr static auto& rv_shift = ascip::rv_shift<s>;
    constexpr static auto& any_shift = ascip::any_shift<s>;
    constexpr static auto& shift = ascip::shift<s>;
  };
};

template<typename parser> constexpr auto ascip_details::prs::base_parser<parser>::operator()(auto act) const {
  return semact_parser<parser, decltype(auto(act))>{ {},
    static_cast<decltype(act)&&>(act),
    static_cast<const parser&>(*this)
  };
}
