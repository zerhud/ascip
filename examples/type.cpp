#include <string>
#include <vector>
#include <memory>

#include "ascip.hpp"

struct type {
	std::string name;
	std::vector<std::unique_ptr<type>> sub_types;
};

template<typename gh, template<auto> class term=gh::template tmpl>
constexpr auto make_grammar() {
	auto ident = lexeme(gh::alpha >> *(gh::alpha | gh::d10 | term<'_'>::char_));
	static_assert([&]{
		std::string r;
		parse(ident, gh::make_source("ok"), r);
		return (r[0] == 'o') + (2 * (r[1] == 'k'));
	}() == 3, "can parser ident");

	constexpr auto type_p =
		ident++ //++ for increase result field number
		>> gh::seq_enable_recursion // now we can restart the parser, it will be the number 0
		                            // (if there was an inner parser with enabled recursion, this parser will be number 1, and so on)
		>> -(term<'<'>::_char // req with () operator, we pass lambda to create the result for reqursion
			>> (term<0>::req([](auto& r) { // the 0: we call the closest parser with enabled recursion
				r.reset(new type());
				return r.get(); // we can also return a reference, but it requires to specify the lambda return type
			}) % ',')
			>> term<'>'>::_char);
	static_assert([=]{
		type r;
		parse(type_p, +gh::space, gh::make_source("a"), r);
		return r.name[0];
	}() == 'a');
	static_assert([&] {
		type r;
		parse(type_p, +gh::space, gh::make_source("a<b,c>"), r);
		r.name[0] /= (r.name[0] == 'a');
		r.sub_types[0]->name[0] /= (r.sub_types[0]->name[0] == 'b');
		r.sub_types[1]->name[0] /= (r.sub_types[1]->name[0] == 'c');
		return r.sub_types.size();
	}() == 2);

	return type_p;
}

using parser = ascip;

int main(int, char**) {
	auto g = make_grammar<parser>();
	static_assert([]{
		type r;
		parse(make_grammar<parser>(), +parser::space, parser::make_source("box<list<string>,int>"), r);
		return (r.sub_types[0]->sub_types[0]->name == "string") +
		       (2 * (r.sub_types[1]->name == "int"));
	}() == 3);
	return 0;
}
