// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#ifndef LSFML_RECT_EXPORT_HPP_INCLUDED
#define LSFML_RECT_EXPORT_HPP_INCLUDED

#include "open_util.hpp"
#include <lsfml/vector.hpp>
#include <lsfml/rect.hpp>

#include <apollo/builtin_types.hpp>
#include <apollo/class.hpp>
#include <apollo/raw_function.hpp>
#include <apollo/reference.hpp>
#include <apollo/property.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <string>

template <typename T>
std::string rect_to_str(sf::Rect<T> const& v)
{
    return "lsfml.graphics.Rect(x="
        + std::to_string(v.left)
        + ", y=" + std::to_string(v.top)
        + "; w=" + std::to_string(v.width)
        + ", h=" + std::to_string(v.height)
        + ")";
}

template <typename T>
int rect_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    if (n_args == 0) {
        apollo::emplace_object<sf::Rect<T>>(L);
    } else if (n_args == 2) {
        apollo::emplace_object<sf::Rect<T>>(L,
            APOLLO_TO_ARG(L, 1, sf::Vector2<T>),
            APOLLO_TO_ARG(L, 2, sf::Vector2<T>));
    } else if (n_args == 4) {
        apollo::emplace_object<sf::Rect<T>>(L,
            apollo::to<T>(L, 1),
            apollo::to<T>(L, 2),
            apollo::to<T>(L, 3),
            apollo::to<T>(L, 4));
    } else {
        throw std::invalid_argument("Bad argument count for Rect.new");
    }
    return 1;
}

template <typename T>
int rect_contains(lua_State* L)
{
    auto self_ref = apollo::to<sf::Rect<T>>(L, 1);
    auto const& self = apollo::unwrap_ref(self_ref);
    int const n_args = lua_gettop(L);
    if (lua_gettop(L) == 2) {
        apollo::push(L, self.contains(
            APOLLO_TO_ARG(L, 2, sf::Vector2<T> const&)));
    } else if (n_args == 3) {
        apollo::push(L, self.contains(
                apollo::to<T>(L, 2),
                apollo::to<T>(L, 3)));
    } else {
        throw std::invalid_argument("Bad argument count for Rect.contains");
    }
    return 1;
}

template <typename T>
int rect_intersection(lua_State* L)
{
    auto self_ref = apollo::to<sf::Rect<T>>(L, 1);
    auto const& self = apollo::unwrap_ref(self_ref);
    sf::Rect<T> intersection;
    if (self.intersects(APOLLO_TO_ARG(L, 2, sf::Rect<T>), intersection)) {
        apollo::push(L, intersection);
    } else {
        lua_pushnil(L);
    };
    return 1;
}

template <typename T>
int rect_intersects(lua_State* L)
{
    return apollo::push(L, APOLLO_TO_ARG(L, 1, sf::Rect<T>).intersects(
        APOLLO_TO_ARG(L, 2, sf::Rect<T>)));
}

template <typename T>
apollo::stack_reference rect_export(lua_State* L)
{
    using rect = sf::Rect<T>;
    apollo::new_table(L)
        ("x", FN(APOLLO_MEMBER_GETTER(rect::left)))
        ("set_x", FN(APOLLO_MEMBER_SETTER(rect::left)))
        ("y", FN(APOLLO_MEMBER_GETTER(rect::top)))
        ("set_y", FN(APOLLO_MEMBER_SETTER(rect::top)))
        ("w", FN(APOLLO_MEMBER_GETTER(rect::width)))
        ("set_w", FN(APOLLO_MEMBER_SETTER(rect::width)))
        ("h", FN(APOLLO_MEMBER_GETTER(rect::height)))
        ("set_h", FN(APOLLO_MEMBER_SETTER(rect::height)))
        ("contains", apollo::raw_function::caught<&rect_contains<T>>())
        ("intersects", apollo::raw_function::caught<&rect_intersects<T>>())
        ("intersection", apollo::raw_function::caught<&rect_intersection<T>>())
        ("new", apollo::raw_function::caught<&rect_new<T>>());

    apollo::stack_reference rect_members(L, -1);

    apollo::export_class<sf::Rect<T>>(L)
        ("__index", rect_members)
        ("__tostring", FN(rect_to_str<T>))
        ("__eq", FN(apollo::op::eq<rect const&, rect const&>));

    return rect_members;
}

#endif // LSFML_RECT_EXPORT_HPP_INCLUDED
