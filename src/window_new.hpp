// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#ifndef LSFML_WINDOW_NEW_HPP_INCLUDED
#define LSFML_WINDOW_NEW_HPP_INCLUDED

#include <apollo/lua_include.hpp>
#include <apollo/class.hpp>
#include <apollo/builtin_types.hpp>
#include <lsfml/string.hpp>

template <typename T>
int window_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    switch (n_args) {
        case 0:
            apollo::emplace_object<T>(L);
        break;

        case 2:
            apollo::emplace_object<T>(L,
                apollo::to<sf::VideoMode>(L, 1).get(),
                apollo::to<sf::String>(L, 2));
        break;

        case 3:
            apollo::emplace_object<T>(L,
                apollo::to<sf::VideoMode>(L, 1).get(),
                apollo::to<sf::String>(L, 2),
                apollo::to<sf::Uint32>(L, 3));
        break;

        case 4:
            apollo::emplace_object<T>(L,
                apollo::to<sf::VideoMode>(L, 1).get(),
                apollo::to<sf::String>(L, 2),
                apollo::to<sf::Uint32>(L, 3),
                apollo::to<sf::ContextSettings>(L, 4).get());
        break;

        default:
            throw std::invalid_argument(
                "Bad argument count for Window constructor.");
    }
    return 1;
}

#endif // LSFML_WINDOW_NEW_HPP_INCLUDED
