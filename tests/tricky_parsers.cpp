//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "factory.hpp"

using namespace std::literals;

constexpr auto by_table_test_tail(auto mk) {
  std::vector<int> r;
  std::vector<char> table;
  auto prs = +by_table(mk, [&](auto& v){
    for(int i=0;i<table.size();++i) if(table[i]==v) return i;
    table.emplace_back(v);
    return (int)table.size()-1;
  }, p::lower);
  auto src = "a c b c"sv;
  auto pr = parse(prs, +p::space, p::make_source(src), r);
  return (pr==src.size())
  + 2*(r.size()==4) + 4*(r[0]==0) + 8*(r[1]==1) + 16*(r[2]==2) + 32*(r[3]==1)
  + 64*(table.size()==3) + 128*(table[0]=='a') + 256*(table[1]=='c') + 512*(table[2]=='b')
  ;
}

static_assert( by_table_test_tail([]{return char{};}) == 1023 );
static_assert( [] {
  char r{};
  return by_table_test_tail([&]{return &r;});
}() == 1023 );
static_assert( [] {
  char tmp_r{};
  std::vector<int> r;
  auto prs = +by_table([&]{return &tmp_r;}, [](auto& v){ std::unreachable(); return 0; }, p::lower);
  return parse(prs, +p::space, p::make_source(" A B"), r);
}() < 0, "nothing happens if cannot parse" );

int main(int,char**) {
}