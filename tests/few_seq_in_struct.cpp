#include "ascip.hpp"

#include <tuple>
#include <vector>
#include <memory>

using p = ascip<std::tuple>;

struct test_1 {
	char a, b, c, d;
};

static_assert( get<1>(ascip_details::tuple<int,char>{10, 'a'}) == 'a' );
static_assert( get<0>(ascip_details::tuple<int,char>{10, 'a'}) == 10 );

static_assert( []{
	test_1 r1;
	auto r = parse(
		p::char_<'a'>++ >> use_seq_result(skip(fnum<1>(p::char_<'b'>)++ >> p::char_<'c'>))++ >> ++p::char_<'d'>,
		p::make_source("abcd"), r1);
	return
	   (r==4) +
	2 *(r1.a == 'a') +
	4 *(r1.b == 'b') +
	8 *(r1.c == 'c') +
	16*(r1.d == 'd')
	;
}() == 31 );

constexpr auto ab_req = (p::char_<'a'>|'b') >> -(p::_char<'('> >> p::req<1> >> p::_char<')'>);
static_assert( []{
	char r1;
	const auto r = parse(ab_req, +p::space, p::make_source("a ( b )"), r1);
	return
	    (r==7)
	+ 2*(r1=='b')
	;
}() == 3 );

static_assert( [] {
	char r1;
	const auto r = parse(p::char_<'a'> >> (p::char_<'b'> | (p::char_<'r'> >> p::req<1>)), p::make_source("arab"), r1);
	return (r==4)
	+ 2*(r1=='b')
	;
}() == 3, "can use recursion with | parser" );

struct base {virtual ~base() noexcept =default ;};
struct content {
	char o;
	std::vector<std::unique_ptr<base>> storage;
	constexpr base& operator[](auto ind) { return *storage[ind]; }
};
struct element : base {
	char i, c;
	constexpr static auto struct_fields_count() { return 2; }
	constexpr static auto mk() {
		return p::char_<'1'>++ >> p::char_<'e'>;
	}
};
struct element_2 : base {
	char i, c;
	constexpr static auto struct_fields_count() { return 2; }
	constexpr static auto mk() {
		return p::char_<'2'>++ >> p::char_<'e'>;
	}
};
struct block : base {
	struct ctx_test{};
	char i, c;
	content elements;
	constexpr static auto struct_fields_count() { return 3; }
	constexpr static auto mk() {
	auto ctx_test_exists = []<typename ctx_t>([[maybe_unused]] auto ret, ctx_t&, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
		static_assert(  exists_in_ctx<ctx_test>(std::decay_t<ctx_t>{}) == true );
	};
		return add_to_ctx<ctx_test>(1, p::char_<'3'>++ >> p::char_<'b'>++ >> p::req<1>(ctx_test_exists));
	}
};

template<typename type> constexpr auto mk_ptr() {
	return [](auto& v) {
		auto ptr = std::make_unique<type>();
		auto* ret = ptr.get();
		v = std::move(ptr);
		return ret;
	};
}
constexpr auto mk_content_parser() {
	auto no_ctx_test = []<typename ctx_t>([[maybe_unused]] auto ret, ctx_t&, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
		static_assert(  exists_in_ctx<block::ctx_test>(std::decay_t<ctx_t>{}) == false );
	};
	return p::char_<'o'>++ >> use_seq_result(p::nop(no_ctx_test)) >> +(
	  element::mk()(mk_ptr<element>())
	| element_2::mk()(mk_ptr<element_2>())
	| block::mk()(mk_ptr<block>())
	);
}

struct top_level {
	char o;
	content holder;
	constexpr static auto  mk() {
		return p::char_<'t'> >> ++mk_content_parser();
	}
};

static_assert( [] {
	top_level r;
	const auto p = parse(top_level::mk(), p::make_source("to1e2e3bo1e"), r);
	return (p==11)
	+ 2*(r.holder.storage.size()==3)
	+ 4*(static_cast<block&>(r.holder[2]).elements.storage.size()==1)
	+ 8*(static_cast<element&>(static_cast<block&>(r.holder[2]).elements[0]).i == '1')
	;
}() == 15);

int main(int,char**) {
	return 0;
}
