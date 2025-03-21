//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ascip.hpp"

#include <vector>
#include <memory>

using p = ascip;

struct base {virtual ~base() noexcept =default ;};
struct content {
  char o{};
  std::vector<std::unique_ptr<base>> storage;
  constexpr base& operator[](auto ind) { return *storage[ind]; }
};
struct element : base {
  char i{}, c{};
  constexpr static auto struct_fields_count() { return 2; }
  constexpr static auto mk() {
    return p::char_<'1'>++ >> p::char_<'e'>;
  }
};
struct element_2 : base {
  char i{}, c{};
  constexpr static auto struct_fields_count() { return 2; }
  constexpr static auto mk() {
    return p::char_<'2'>++ >> p::char_<'e'>;
  }
};
struct block : base {
  struct ctx_test{};
  char i{}, c{};
  content elements;
  constexpr static auto struct_fields_count() { return 3; }
  constexpr static auto mk() {
  auto ctx_test_exists = []<typename ctx_t>([[maybe_unused]] auto ret, ctx_t*, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
    static_assert(  exists_in_ctx<ctx_test>(std::decay_t<ctx_t>{}) == true );
  };
    return add_to_ctx<ctx_test>(1, p::char_<'3'>++ >> p::char_<'b'>++ >> p::rec<0>(ctx_test_exists));
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
  auto no_ctx_test = []<typename ctx_t>([[maybe_unused]] auto ret, ctx_t*, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
    static_assert(  exists_in_ctx<block::ctx_test>(std::decay_t<ctx_t>{}) == false );
  };
  return p::seq_enable_recursion >> p::char_<'o'>++ >> use_seq_result(p::nop(no_ctx_test)) >> +(
    element::mk()(mk_ptr<element>())
  | element_2::mk()(mk_ptr<element_2>())
  | block::mk()(mk_ptr<block>())
  );
}

struct top_level {
  char o{};
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
  //TODO: GCC15: GCCBUG: cannot use dynamic_cast here: the operation will be failed, clang17 compiles normally
}() == 15);

struct external_parser : p::base_parser<external_parser> {
  constexpr auto mk_parser() const {
    return p::char_<'i'> >> p::char_<'n'>;
  }
  constexpr p::parse_result parse(auto&& ctx, auto src, auto& result) const {
    return continue_parse(ctx, mk_parser(), src, result);
  }
};

static_assert( ascip_details::parser<external_parser> );

static_assert( [] {
  char r{};
  const auto p = parse(p::char_<'o'> >> +external_parser{}, +p::space, p::make_source("oinin"), r);
  return (p==5) + 2*(r=='n');
}() == 3 );

static_assert( []{
  struct use_ctx_test_tag {};
  char r{};
  int val{};
  const auto p = parse(add_to_ctx<use_ctx_test_tag>(7, p::char_<'a'> >> from_ctx<use_ctx_test_tag>([&](auto& r, auto& v){val = v;}, p::char_<'b'>)), +p::space, p::make_source("ab"), r);
  return (p == 2) + 2*(r=='b') + 4*(val==7);
}() == 7 );
static_assert( []{
  struct use_ctx_test_tag {};
  char r{};
  int val{1};
  const auto p = parse(create_in_ctx<use_ctx_test_tag>([](char&){return 11;},
    (p::char_<'a'> >> from_ctx<use_ctx_test_tag>([&](auto& r, auto& v){val = v;static_assert(std::is_same_v<std::decay_t<decltype(r)>, char>);}, p::char_<'b'>))) | p::char_<'u'>,
    +p::space, p::make_source("ab"), r);
  return (p == 2) + 2*(r=='b') + 4*(val==11);
}() == 7 );
static_assert( []{
  struct use_ctx_test_tag {};
  char r{};
  const auto p = parse(add_to_ctx<use_ctx_test_tag>(0, p::char_<'a'> >> result_from_ctx<use_ctx_test_tag>([&](auto& r, auto& v){ r = v; }, p::uint_<>)), +p::space, p::make_source("a122"), r);
  return (p == 4) + 2*(r=='z');
}() == 3 );

int main(int,char**) {
}
