#include <tuple>
#include "ascip.hpp"


struct base { char a, b; };
struct child : base { char c; };

template<typename gh, template<auto>class th=gh::template term>
constexpr void example() {
    // for parse in base class due language limits we can use:
    //   - cast method, it will case the result to base class
    //   - () operator and return reference or pointer to what we want
    constexpr auto parser = cast<base>(th<'a'>::char_++ >> th<'b'>::char_) >> th<'c'>::char_([](auto&r)->char&{return r.c;});
    constexpr auto skip = +gh::space;
    static_assert( [&]{ child r;
        parse(parser, +gh::space, gh::make_source("a b c"), r);
        return (r.a=='a') + (2*(r.b=='b')) + (4*(r.c=='c'));
    }() == 7 );
}

using parser = ascip<std::tuple>;

int main(int,char**) {
    example<parser>();
    return 0;
}
