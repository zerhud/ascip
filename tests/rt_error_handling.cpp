//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define ASCIP_WITH_STD_EXCEPTION

#include "factory.hpp"
#include <iostream>

using namespace std::literals;
namespace prs = ascip_details::prs;

template<typename gh, template<auto>class sup = gh::template tmpl>
constexpr void test_error_handling() {
	std::cout << "test error handling" << std::endl;
	char r;
	parse(lexeme(gh::any >> must<"test <nl>b">(sup<'a'>::char_)), +gh::space, gh::make_source("\nb"), r,
		[](auto&, auto src, int line, auto msg){
			gh::write_out_error_msg(std::cout, "test_file_1", msg, "ups", src, line);
			return -1;
	});
	std::cout << "\n\nnext test:" << std::endl;
	parse(gh::any >> gh::any >> +sup<'b'>::char_ >> must<"test a<nl>bbbcuu">(sup<'a'>::char_), +gh::space, gh::make_source("a\nbbbcuu"), r,
		[](auto&, auto src, int line, auto msg){
		gh::write_out_error_msg(std::cout, "test_file_2", msg, "ups", src, line);
		return -1;
	});

	bool exception_was=false;
	try {parse(gh::any >> gh::any >> must<"test">(sup<'a'>::char_), +gh::space, gh::make_source("\nba"), r);}
	catch (const prs::must_parser_default_exception& ex) {
		exception_was = true;
		if (ex.line_number()!=2) throw;
		if (std::string_view(ex.what()) != "test"sv ) throw;
	}
	if (!exception_was) throw 1;

	std::cout << "finish test" << std::endl;
}

int main(int,char**) {
  test_error_handling<ascip>();
	return 0;
}