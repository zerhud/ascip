//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ascip.hpp"

static_assert( [] { char r; return ascip::nop.parse(ascip_details::make_default_context(), ascip_details::make_source("test"), r); }()== 0 );
static_assert( ascip::char_<'s'>.test() );
static_assert( ascip_details::prs::test_literal_parser() );
static_assert( ascip_details::prs::test_parser_value() );
static_assert( ascip::space.test() );
static_assert( ascip::any.test() );
static_assert( ascip::int_.test() && ascip::uint_<10>.test() && ascip::fp.test() );
static_assert( ascip_details::prs::test_range_parser() );
static_assert( ascip_details::prs::test_omit<ascip>() );
static_assert( ascip_details::prs::test_as<ascip>() );
static_assert( ascip_details::prs::test_variant<ascip>() );
static_assert( ascip_details::prs::test_negate<ascip>() );
static_assert( ascip_details::prs::test_optional<ascip>() );
static_assert( ascip_details::prs::test_different<ascip>() );
static_assert( ascip_details::prs::test_reparse<ascip>() );
static_assert( ascip_details::prs::test_checkers<ascip>() );
static_assert( ascip_details::prs::test_injection<ascip>() );
static_assert( ascip_details::prs::test_must_parser<ascip>() );
static_assert( ascip_details::prs::test_rvariant<ascip>() );
static_assert( ascip_details::prs::context_parsers<ascip>() );

static_assert( ascip_details::prs::test_semact<ascip>() );
static_assert( ascip_details::test_exists_in<ascip>() );

int main(int,char**) {
	return 0;
}
