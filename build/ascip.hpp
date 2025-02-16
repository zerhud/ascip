namespace ascip_details {

template<typename type, template<typename...>class tmpl> constexpr bool is_specialization_of = false;
template<template<typename...>class type, typename... args> constexpr bool is_specialization_of<type<args...>, type> = true;
template<template<typename...>class tmpl> constexpr static auto is_spec_checker = [](const auto* p) { return is_specialization_of<std::decay_t<decltype(*p)>, tmpl>; };

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
struct use_result_tag {};

template<typename value_t, typename... t> struct context_frame {
  constexpr static auto tags = type_list<t...>{};
  value_t value;
};
template<typename l_value_t, typename... l_t, typename r_value_t, typename... r_t>
constexpr auto operator+(context_frame<l_value_t, l_t...> left, context_frame<r_value_t, r_t...> right) {
  return tuple<context_frame<l_value_t, l_t...>, context_frame<r_value_t, r_t...>>{ std::move(left), std::move(right) };
}
template<typename l_value_t, typename... l_t> constexpr auto operator+(int right, context_frame<l_value_t, l_t...> left) { return std::move(left) + right; }
template<typename l_value_t, typename... l_t> constexpr auto operator+(context_frame<l_value_t, l_t...> left, int) {
  return tuple<context_frame<l_value_t, l_t...>>{ std::move(left) };
}
template<typename... l_frames, typename r_value_t, typename... r_t>
constexpr auto operator+(tuple<l_frames...> left, context_frame<r_value_t, r_t...> right) {
  return [&]<auto... inds>(std::index_sequence<inds...>) {
    return tuple<l_frames..., context_frame<r_value_t, r_t...>>{ get<inds>(left)..., right};
  }(std::make_index_sequence<sizeof...(l_frames)>{});
}
template<typename... l_frames>
constexpr auto operator+(tuple<l_frames...> left, int) {
  return left;
}
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

template<template<typename...>class tuple, typename... types> constexpr decltype(auto) repack(tuple<types...> tup, auto&& fnc) {
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
template<bool cond, typename... tag> constexpr auto make_ctx_if(auto&& val, auto prev_ctx) {
  if constexpr (cond) return make_ctx<tag...>(std::forward<decltype(val)>(val), std::move(prev_ctx));
  else return std::move(prev_ctx);
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
template<bool cond, typename... _tags, typename value, typename... frames> constexpr auto replace_in_ctx(value&& val, tuple<frames...> prev) {
  constexpr type_list<_tags...> tags{};
  if constexpr (!cond) return std::move(prev);
  else if constexpr((contains_any(frames::tags, tags) + ...)) return repack(std::move(prev), [&](auto... pack) {
    return (... + [&](auto cur){
      if constexpr(contains_any(decltype(cur)::tags, tags)) return context_frame<std::decay_t<value>, _tags...>{val};
      else return cur;
    }(std::move(pack)));
  });
  else return make_ctx<_tags...>(std::forward<decltype(val)>(val), std::move(prev));
}

template<typename tag, typename... frames> constexpr auto remove_from_ctx(tuple<frames...> prev) {
  return repack(std::move(prev), [](auto... pack) {
    return (... + [](auto cur) {
      if constexpr (contains(decltype(cur)::tags, type_c<tag>)) return 1;
      else return std::move(cur);
    }(std::move(pack)));
  });
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
	constexpr string_literal() =default ;
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

	constexpr auto zero_pos() const {
		for (auto i=0;i<str_size;++i) if (!value[i]) return i;
		return str_size;
	}

	template<auto sz>
	friend constexpr auto operator+(const string_literal& left, const char_type(&right)[sz]) {
		string_literal<char_type, str_size + sz - 1> ret;
		for (auto i=0;i<str_size-1;++i) ret.value[i] = left[i];
		for (auto i=0;i<sz;++i) ret.value[i+str_size-1] = right[i];
		return ret;
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
	static_assert( [] {
		string_literal src("src");
		auto ret = src + "_test";
		return (ret.size()==8) + 2*(ret == "src_test");
	}() == 3 );
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


namespace ascip_details {
//TODO: remove static keyword in methods in this file
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
constexpr bool exists_in(auto* src, const auto& checker) {
	return exists_in(src, checker, [](const auto*){ return false; });
}

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
struct any_shift_parser_tag {};
struct list_shift_tag {};
using parse_result = decltype(-1);

namespace prs { template<typename parser> struct base_parser; }

template<typename type> concept parser = std::is_base_of_v<prs::base_parser<std::decay_t<type>>, std::decay_t<type>> ;
template<typename type> concept nonparser = !parser<type>;
constexpr auto is_any_shift_parser = [](auto* p){ return requires{static_cast<const any_shift_parser_tag*>(p);}; };

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
struct must_parser_default_exception



{
	virtual ~must_parser_default_exception() =default ;
	virtual char symbol() const =0 ;
	virtual int line_number() const =0 ;
	virtual const char* what() const noexcept =0 ;
};
template<string_literal msg, parser type> struct must_parser : base_parser<must_parser<msg, type>> {
	type p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		return call_if_error(ctx, result, ret, src);
	}

	constexpr static auto call_if_error(auto& ctx, auto& result, auto orig_ret, auto& src) {
		if (0 <= orig_ret) return orig_ret;
		auto err = search_in_ctx<err_handler_tag>(ctx);
		if constexpr(requires{(*err)(0, msg);}) return (*err)(new_line_count(ctx), msg);
		else if constexpr(requires{(*err)(0, src, msg);}) return (*err)(new_line_count(ctx), src, msg);
		else if constexpr(requires{(*err)(msg);}) return (*err)(msg);
		else if constexpr(requires{(*err)(result, 0, msg);}) return (*err)(result, new_line_count(ctx), msg);
		else if constexpr(requires{(*err)(result, src, 0, msg);}) return (*err)(result, src, new_line_count(ctx), msg);
		else if constexpr(requires{(*err)(result, msg);}) return (*err)(result, msg);
		else {
			// for test parsers with must parser without err_handler_tag in context
			struct exception : must_parser_default_exception {
				constexpr exception(decltype(new_line_count(ctx)) line, char src, decltype(msg) message)
				: line(line), src(src), message(message)
				{}
				decltype(new_line_count(ctx)) line;
				char src;
				decltype(msg) message;

				constexpr char symbol() const override { return src; }
				constexpr int line_number() const override { return line; }
				constexpr const char* what() const noexcept override {
					return message.value;
				}
			};

			throw exception{new_line_count(ctx), src(), msg};
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

template<typename... parsers> struct opt_seq_parser ;

struct seq_stack_tag{};
struct seq_shift_stack_tag{};
struct seq_result_stack_tag{};
}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

