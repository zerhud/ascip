#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <optional>
#include <iostream>

#include "ascip2.hpp"

struct kv_pair {
	std::optional<std::string> name;
	std::string value;
	constexpr static auto struct_fields_count() { return 2; }
};

template<typename gh,template<auto>class term=gh::template tmpl>
constexpr auto make_grammar() {
	//NOTE: the * parser is now -+ parser
	return -(+gh::alpha) >> term<':'>::_char >> ++(+gh::alpha);
}

using parser = ascip;

static_assert( !ascip_details::optional<int> );
static_assert( !ascip_details::optional<std::string> );
static_assert( ascip_details::optional<std::optional<int>> );

static_assert( []{
	kv_pair r;
	parse(make_grammar<parser>(), +parser::space, parser::make_source("name:value"), r);
	return *r.name;
}() == "name" );

static_assert( []{
	kv_pair r;
	parse(make_grammar<parser>(), +parser::space, parser::make_source(":value"), r);
	return r.name.has_value();
}() == false );

bool has_value(const auto& v){
	if constexpr(requires{ v.has_value(); }) return v.has_value();
	else return true;
}
auto& value(const auto& v) {
	if constexpr(requires{ *v; }) return *v;
	else return v;
}
template<typename type> constexpr auto type_name() { return __PRETTY_FUNCTION__; }
int main(int,char**) {
	kv_pair r;

	parse(make_grammar<parser>(), +parser::space, parser::make_source("name:value"), r);
	std::cout << "has_value: " << has_value(r.name) << " the value: " << value(r.name) << std::endl;

	parse(make_grammar<parser>(), +parser::space, parser::make_source(":value"), r);
	std::cout << "has_value: " << has_value(r.name) << " the value: " << value(r.name) << std::endl;

	return 0;
}
