#pragma once

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../base.hpp"

namespace ascip_details::prs {

struct rvariant_stack_tag {};
struct rvariant_cpy_result_tag {};

template<typename, parser...> struct rvariant_parser;

}
