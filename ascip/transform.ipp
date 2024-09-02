#pragma once

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
#ifndef __clang__
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
	static_assert( std::is_same_v<seq_num_rfield_val<test_parser2, _seq_num_rfield_val<1>>, decltype(test_transform_t_to_p(fnum<1>(test_parser{})))> );
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
#endif
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
