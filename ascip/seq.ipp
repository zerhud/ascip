#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

constexpr static auto call_err_method(auto& method, auto& ctx, auto src, auto& result, auto message) {
	if constexpr (requires{ { method(ctx, src, result) } -> std::same_as<parse_result>; })
		return method(ctx, src, result);
	else if constexpr (requires{ { method(result,src,0,message) } -> std::same_as<void>; })
		return (method(
			result,
			src,
			search_in_ctx<ascip_details::new_line_count_tag>(ctx),
			message
			), -1);
	else return method(
			result,
			src,
			search_in_ctx<ascip_details::new_line_count_tag>(ctx),
			message
			);
}

struct seq_stack_tag{};
struct seq_shift_stack_tag{};
struct seq_result_stack_tag{};
struct seq_crop_ctx_tag {};
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
template<ascip_details::parser parser>
struct use_seq_result_parser : base_parser<use_seq_result_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		auto& result = *search_in_ctx<seq_result_stack_tag>(ctx);
		return p.parse(ctx, src, result);
	}
};
template<auto ind>
struct seq_reqursion_parser : base_parser<seq_reqursion_parser<ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr( ascip_details::is_in_concept_check(decltype(auto(ctx)){})  ) return 0;
		else return !!src ? by_ind_from_ctx<ind, seq_stack_tag>(ctx)->parse_without_prep(crop_ctx<ind, seq_crop_ctx_tag>(ctx), static_cast<decltype(src)&&>(src), result) : -1;
	}
};
template<auto ind> constexpr static auto req = seq_reqursion_parser<ind>{};

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

constexpr static struct seq_inc_rfield : base_parser<seq_inc_rfield> {constexpr parse_result parse(auto&&,auto,auto&)const {return 0;} } sfs{} ;
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

	template<template<typename...>class tmpl>
	constexpr static auto is_spec_checker = [](const auto* p) {
		return ascip_details::is_specialization_of<std::decay_t<decltype(*p)>, tmpl>;
	};
	template<typename type>
	constexpr static bool _exists_in(auto&& ch) {
		return exists_in((type*)nullptr, ch, [](const auto* p){
			return requires{ p->seq; } && !requires{ static_cast<const com_seq_parser<concrete, parsers...>*>(p); };
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

	template<typename type, template<typename...>class tmpl> constexpr static int grab_num_val() { 
		int val = 0;
		exists_in((type*)nullptr, [&val](const auto* p){
			constexpr bool is_num = ascip_details::is_specialization_of<std::decay_t<decltype(*p)>, tmpl>;
			if constexpr (is_num) val = std::decay_t<decltype(*p)>::value;
			return is_num;
		}, [](const auto* p) {
			return requires{ p->seq; } && !requires{ static_cast<const com_seq_parser<concrete, parsers...>*>(p); };
		});
		return val;
	}
	template<typename type> constexpr static int num_field_val() { return grab_num_val<type, seq_num_rfield_val>(); }
	template<typename type> constexpr static auto inc_field_val() { return grab_num_val<type, seq_inc_rfield_val>(); }

	constexpr auto on_error(auto val) const { return static_cast<const concrete*>(this)->on_error(val); }

	template<auto find> constexpr auto call_parse(ascip_details::parser auto& p, auto&& ctx, auto src, auto& result) const requires is_struct_requires_pd {
		if constexpr (is_field_separator<decltype(auto(p))>) return p.parse(ctx, src, result);
		else if constexpr ( std::is_same_v<std::decay_t<decltype(result)>, ascip_details::type_any_eq_allow> )
			return p.parse(ctx, src, result);
		else {
			return p.parse(ctx, src, ascip_reflection::get<find>(result));
		}
	}
	template<auto find> constexpr auto call_parse(ascip_details::parser auto& p, auto&& ctx, auto src, auto& result) const requires (!is_struct_requires_pd) {
		return p.parse(ctx, src, result);
	}
	template<auto find> constexpr auto call_parse(auto& p, auto&& ctx, auto src, auto& result) const requires (!ascip_details::parser<decltype(auto(p))>) {
		auto& prev_src = *search_in_ctx<concrete>(ctx);
		return call_err_method(p, ctx, src, result, "unknown");
	}
	template<auto find, auto pind, typename cur_t, typename... tail> constexpr parse_result parse_seq(auto&& ctx, auto src, auto& result) const {
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
	constexpr parse_result parse_and_store_shift(auto&& ctx, auto src, auto& result) const {
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
		  make_ctx<seq_result_stack_tag>(&result,
		    make_ctx<seq_stack_tag>(this,
			    ascip_details::make_ctx<concrete>(&src, ctx)
		    )
		  )
		);
		return parse_and_store_shift<0,0>(make_ctx<seq_crop_ctx_tag>(1, cur_ctx), src, result);
	}
	constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
		return parse_seq<0, 0, parsers...>(std::forward<decltype(ctx)>(ctx), std::move(src), result);
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
#ifndef __clang__
	static_assert( ({ char r='z', l='a';
		(char_<'a'> >> char_<'b'> >> [&](auto& result, auto src, auto line, auto msg) {
		 src.ind /= (src.ind==2);
		 result /= (result=='b');
		 line /= (line==1);
		 l = 'u' * (result == 'b');
		 }).parse(make_test_ctx(), make_source("ab"), r);
	l;}) == 'u');
	static_assert(
		test_parser_parse_r_str(char_<'a'> >> char_<'b'> >> [](auto& result, auto src, auto line, auto msg){return 1;}, "ab", 3, 'a', 'b'),
		"lambda value is added to position" );
	static_assert(
		test_parser_parse_r_str(char_<'a'> >> [](auto&& ctx, auto src, auto& result){return 1;}, "a", 2, 'a'),
		"lambda value is added to position" );
#endif
	return true;
}

constexpr static bool test_seq_result_fields() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
	struct with_2_chars { char a, b; };
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>, "ab", 2), 'a', 'b' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, ++char_<'a'> >> --char_<'b'>, "ab", 2), 'b', 'a' ) );
	static_cast<const opt_seq_parser<char_parser<'a'>, char_parser<'b'>, char_parser<'c'>>&>(char_<'a'> >> char_<'b'> >> char_<'c'>);
	static_cast<const opt_seq_parser<char_parser<'a'>, int_parser, char_parser<'c'>, int_parser>&>(char_<'a'> >> int_ >> char_<'c'> >> int_);
	static_assert( test_cmp_struct( test_parser_parse(with_2_chars{}, char_<'a'>++ >> char_<'b'>-- >> char_<'c'>, "abc", 3), 'c', 'b' ) );
	return true;
#pragma GCC diagnostic pop
}
constexpr static bool test_seq_finc() {
	struct with_3_chars { char a, b, c; };
	static_assert( test_cmp_struct( test_parser_parse(with_3_chars{}, finc<2>(char_<'a'>) >> --char_<'b'> >> --char_<'c'>, "abc", 3), 'c', 'b', 'a' ) );
	static_assert( test_cmp_struct( test_parser_parse(with_3_chars{}, ++char_<'a'> >> ++char_<'b'> >> finc<-2>(char_<'c'>), "abc", 3), 'c', 'a', 'b' ) );
	return true;
}
constexpr static bool test_seq_single_field() {
	struct with_1_field { decltype(mk_str()) val; };
#ifndef __clang__ // cannot since clang17 :(
	static_assert( test_parser_parse(with_1_field{}, fnum<0>(char_<'a'>) >> char_<'b'>, "ab", 2).val[0] == 'a' );
	static_assert( test_parser_parse(with_1_field{}, finc<0>(char_<'a'>) >> char_<'b'>, "ab", 2).val[1] == 'b' );
	static_assert( test_parser_parse(with_1_field{}, sfs >> --char_<'a'> >> char_<'b'>, "ab", 2).val[1] == 'b' );
#endif
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
		auto p=((char_<'a'>|'b')++ >> -(_char<'('> >> use_seq_result(req<1>([&r2,&ok](auto& r){ok='o';return &r2;})) >> _char<')'>));
		p.parse(make_test_ctx(), make_source("a(b)"), r);
		return r.n * (r2.n == 'b') * (ok=='o');
	}() == 'a', "check semact for create value");
	static_assert( []{
		semact_req_tester r;
		auto p=(char_<'a'>++ >> -(omit(char_<'('>) >> use_seq_result(req<1>([](auto& r){r=new semact_req_tester('b');return r;})) >> omit(char_<')'>)));
		p.parse(make_test_ctx(), make_source("a(a)"), r);
		char ret = r.ptr->n * (r.ptr->ptr == nullptr);
		delete r.ptr;
		return ret;
	}() == 'a', "check semact for create value");

	return true;
}
constexpr static bool test_seq_must() {
#ifndef __clang__
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
#endif
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
	constexpr auto ab_req = (char_<'a'>|'b') >> -use_seq_result(_char<'('> >> use_seq_result(req<1>) >> _char<')'> >> ++cur_shift) >> ++cur_shift;
	static_assert( [&]{
		struct { char a='z'; int shift; } r;
		ab_req.parse(make_test_ctx(), make_source("a(b)"), r);
		return (r.a=='b') + 2*(r.shift==5);
	}() == 3, "can parse current shift in reqursion (the last brace is included)");
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
