#include <string_view>
#include <iostream>

#include "ascip.hpp"

struct version {
  int major=0;
  int minor=0;
};

template<typename gh,template<auto>class term=gh::template term>
constexpr auto make_grammar() {
	return lexeme(term<'v'>::_char >> gh::int_ >> term<'.'>::_char >> ++gh::int_);
	//NOTE:
	//  _char is same as omit(char_)
	//  lexeme is a function found by adl in ascip_details namespace 
	//  ++ prefix and postfix increases result's field number
}

using parser = ascip<>;

constexpr void test_grammar() {
  static_assert( []{
    version result;
    parse(make_grammar<parser>(), +parser::space, parser::make_source("v1.2"), result);
    result.major /= (result.major == 1);
    return result.minor; }() == 2 );
  // we can also use a string_view as a source
  // the source must to be light weight: it will be copied many times
  static_assert( []{
    std::string_view src = "v3.4";
    version result;
    parse(make_grammar<parser>(), +parser::space, parser::make_source("v3.4"), result);
    result.major /= (result.major == 3);
    return result.minor; }() == 4 );
}

int main(int,char**) {
  return 0;
}
