#pragma once

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


template<typename value_t, ascip_details::parser parser_t >
inline constexpr auto as( parser_t&& p, value_t&& val ){ return decltype(auto(p))::holder::as(p, val); }
template<ascip_details::parser parser_t >
inline constexpr auto omit( parser_t&& p ){ return decltype(auto(p))::holder::omit(p); }
