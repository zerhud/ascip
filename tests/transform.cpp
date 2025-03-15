//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "factory.hpp"

struct transform_noncopyable {
	constexpr transform_noncopyable(){}
	constexpr transform_noncopyable(const transform_noncopyable&) =delete ;
	constexpr transform_noncopyable& operator=(const transform_noncopyable&) =delete ;
};

struct test_parser : p::base_parser<test_parser>{constexpr auto parse(auto&& ctx,auto src, auto&)const{return 0;}};
struct test_parser2 : p::base_parser<test_parser2>{constexpr auto parse(auto&& ctx,auto src, auto&)const{return 0;}};
template<typename pt> struct test_wrapper : p::base_parser<test_wrapper<pt>>{ pt p; };
template<auto ind,typename pt> struct transform_val_p : p::base_parser<transform_val_p<ind,pt>>{ pt p; };

static_assert( ascip_details::parser<test_parser2> );

struct t1_to_t2_mutator {
	struct context{ };
	struct in_lexeme_context{ };
	constexpr static auto create_ctx(){ return context{}; }
	constexpr static auto create_ctx(auto&& src, auto&& ctx) {
		if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, ascip_details::prs::lexeme_parser>)
			return in_lexeme_context{ };
		else if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, ascip_details::prs::skip_parser>)
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

constexpr auto test_transform_t_to_p(auto&& parser) {
	return transform<t1_to_t2_mutator>(std::forward<decltype(parser)>(parser));
}
constexpr bool test_transform_modify_leaf() {
	using namespace ascip_details::prs;
#ifndef __clang__
	static_assert( std::is_same_v<test_parser2, decltype(test_transform_t_to_p(test_parser{}))> );
	static_assert( std::is_same_v<test_parser2, decltype(test_transform_t_to_p(test_parser2{}))> );
	static_assert( std::is_same_v<skip_parser<test_parser2>, decltype(test_transform_t_to_p(skip(test_parser{})))> );
	static_assert( std::is_same_v<skip_parser<test_parser2>, decltype(test_transform_t_to_p(skip(skip(test_parser{}))))> );
	static_assert( std::is_same_v<skip_parser<char_parser<'a'>>, decltype(test_transform_t_to_p(skip(char_<'a'>)))> );
	static_assert( std::is_same_v<lexeme_parser<test_parser>, decltype(test_transform_t_to_p(lexeme(test_parser{})))> );
	static_assert( std::is_same_v<lexeme_parser<skip_parser<test_parser2>>, decltype(test_transform_t_to_p(lexeme(skip(test_parser{}))))> );
	static_assert( std::is_same_v<opt_seq_parser<char_parser<'a'>, test_parser2>, decltype(test_transform_t_to_p(char_<'a'> >> test_parser{}))> );
	static_assert( std::is_same_v<skip_parser<test_parser2>, decltype(test_transform_t_to_p(skip(test_wrapper{ {}, test_parser{} })))> );
	static_assert( std::is_same_v<seq_inc_rfield_val<test_parser2, _seq_rfield_val<1>>, decltype(test_transform_t_to_p(finc<1>(test_parser{})))> );
	static_assert( std::is_same_v<seq_num_rfield_val<test_parser2, _seq_rfield_val<1>>, decltype(test_transform_t_to_p(fnum<1>(test_parser{})))> );
	static_assert( std::is_same_v<cast_parser<int,test_parser2>, decltype(test_transform_t_to_p(cast<int>(test_parser{})))> );
	static_assert( std::is_same_v<result_checker_parser<int,test_parser2>, decltype(test_transform_t_to_p(check<int>(test_parser{})))> );
	static_assert( std::is_same_v<by_ind_parser<0, test_parser2>, decltype(test_transform_t_to_p(by_ind<0>(test_parser{})))> );

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

	struct tag1 {};
	static_assert( std::is_same_v<
			add_to_ctx_parser<test_parser2, tag1, int>,
			decltype(test_transform_t_to_p( add_to_ctx<tag1>(1, test_parser{}) ))
			> );
	auto void_act = [](auto&&...){};
	static_assert( std::is_same_v<
			ctx_use_parser<test_parser2, decltype(void_act), tag1>,
			decltype(test_transform_t_to_p( from_ctx<tag1>(void_act, test_parser{}) ))
			> );
	static_assert( std::is_same_v<
			ctx_use_as_result_parser<test_parser2, decltype(void_act), tag1>,
			decltype(test_transform_t_to_p( result_from_ctx<tag1>(void_act, test_parser{}) ))
			> );
#endif
	return true;
}

int main(int,char**) {
}