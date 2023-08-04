#include <tuple>
#include <list>
#include <vector>
#include <iostream>
#include "ascip.hpp"

template<typename gh>
constexpr auto make_grammar_a1() { return +gh::template char_<'a'>; }

// in first make_grammar we need to write a template keywork every time
// or declare char_ with using. there is a term struct in ascip and you
// can use it in template declaration like this:
template<typename gh,template<auto>class term=gh::template term>
constexpr auto make_grammar_a2() { return +term<'a'>::char_; }

using parser = ascip<std::tuple>;

int main(int,char**) {
    static_assert( ({
        std::vector<char> r;
        parse(make_grammar_a1<parser>(), parser::make_source("aaa"), r);
        r.size(); }) == 3 );

    // the list is not constexpr (in c++23)
    // so we cannot store the result in it
    std::list<char> r;
    parse(make_grammar_a2<parser>(), parser::make_source("aaa"), r);
    return r.size() != 3;
}
