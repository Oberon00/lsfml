// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#include "open_util.hpp"
#include "graphics.hpp"
#include <lsfml/vector.hpp>
#include <lsfml/rect.hpp>

#include <apollo/builtin_types.hpp>
#include <apollo/create_class.hpp>
#include <apollo/raw_function.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace {

int transform_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    if (n_args == 0) {
        apollo::emplace_object<sf::Transform>(L);
    } else if (n_args == 9) {
        auto const flt = [L](int idx) {
            return apollo::to<float>(L, idx);
        };
        apollo::emplace_object<sf::Transform>(L,
            flt(1), flt(2), flt(3),
            flt(4), flt(5), flt(6),
            flt(7), flt(8), flt(9));
    } else {
        throw std::invalid_argument("Bad argument count for Tranform.new");
    }
    return 1;
}

int transform_mul(lua_State* L)
{
    auto lhs_ref = apollo::to<sf::Transform const&>(L, 1);
    auto const& lhs = apollo::unwrap_ref(lhs_ref);
    if (apollo::is_convertible<sf::Transform const&>(L, 2)) {
        auto rhs_ref =
            apollo::unchecked_to<sf::Transform const&>(L, 2);
        return apollo::push(L, lhs * apollo::unwrap_ref(rhs_ref));
    }
    if (apollo::is_convertible<sf::Vector2f const&>(L, 2)) {
        auto rhs_ref =
            apollo::unchecked_to<sf::Vector2f const&>(L, 2);
        return apollo::push(L, lhs * apollo::unwrap_ref(rhs_ref));
    }

    throw std::invalid_argument("Bad right operand for Transform.__mul");
}

int transform_point(lua_State* L)
{
    auto self_ref = apollo::to<sf::Transform const&>(L, 1);
    auto const& self = apollo::unwrap_ref(self_ref);
    int const n_args = lua_gettop(L);
    if (lua_gettop(L) == 1) {
        return apollo::push(L, self.transformPoint(
            APOLLO_TO_ARG(L, 2, sf::Vector2f const&)));
    }
    if (n_args == 2) {
        return apollo::push(L, self.transformPoint(
            apollo::to<float>(L, 2),
            apollo::to<float>(L, 3)));
    }

    throw std::invalid_argument(
        "Bad argument count for Tranform.transfom_point");
}

int transform_combine(lua_State* L)
{
    apollo::to<sf::Transform&>(L, 1).combine(
        APOLLO_TO_ARG(L, 2, sf::Transform const&));
    lua_settop(L, 1);
    return 1;
}

int transform_translate(lua_State* L)
{
    auto& self = apollo::to<sf::Transform&>(L, 1);
    int const n_args = lua_gettop(L);
    if (lua_gettop(L) == 1) {
        self.translate(APOLLO_TO_ARG(L, 2, sf::Vector2f const&));
    } else if (n_args == 2) {
        self.translate(
            apollo::to<float>(L, 2),
            apollo::to<float>(L, 3));
    } else {
        throw std::invalid_argument(
            "Bad argument count for Tranform.translate");
    }
    lua_settop(L, 1);
    return 1;
}

int transform_rotate(lua_State* L)
{
    auto& self = apollo::to<sf::Transform&>(L, 1);
    int const n_args = lua_gettop(L);
    if (lua_gettop(L) == 1) {
        self.rotate(apollo::to<float>(L, 2));
    } else if (n_args == 2) {
        self.rotate(
            apollo::to<float>(L, 2),
            APOLLO_TO_ARG(L, 3, sf::Vector2f const&));
    } else if (n_args == 3) {
        self.rotate(
            apollo::to<float>(L, 2),
            apollo::to<float>(L, 3),
            apollo::to<float>(L, 4));
    } else {
        throw std::invalid_argument(
            "Bad argument count for Tranform.translate");
    }
    lua_settop(L, 1);
    return 1;
}

int transform_scale(lua_State* L)
{
    auto& self = apollo::to<sf::Transform&>(L, 1);
    int const n_args = lua_gettop(L);
    if (lua_gettop(L) == 1) {
        self.scale(APOLLO_TO_ARG(L, 2, sf::Vector2f const&));
    } else if (n_args == 2) {
        if (apollo::is_convertible<float>(L, 2)) {
            self.scale(
                apollo::unchecked_to<float>(L, 2),
                apollo::to<float>(L, 3));
        } else {
            self.scale(
                APOLLO_TO_ARG(L, 2, sf::Vector2f const&),
                APOLLO_TO_ARG(L, 2, sf::Vector2f const&));
        }
    } else if (n_args == 4) {
        self.scale(
            apollo::to<float>(L, 2), apollo::to<float>(L, 3),
            apollo::to<float>(L, 4), apollo::to<float>(L, 5));
    } else {
        throw std::invalid_argument(
            "Bad argument count for Tranform.translate");
    }
    lua_settop(L, 1);
    return 1;
}

} // anonymous namespace

apollo::stack_reference transform_export(lua_State* L)
{
    apollo::register_class<sf::Transform>(L);
    apollo::new_table(L)
        ("new", apollo::raw_function::caught<&transform_new>())
        ("inverse", FN(sf::Transform::getInverse))
        ("transform_point", apollo::raw_function::caught<&transform_point>())
        ("transform_rect", FN(sf::Transform::transformRect))
        ("combine", apollo::raw_function::caught<&transform_combine>())
        ("translate", apollo::raw_function::caught<&transform_translate>())
        ("rotate", apollo::raw_function::caught<&transform_rotate>())
        ("scale", apollo::raw_function::caught<&transform_scale>())
        // TODO: Expose matrix cells (getMatrix())
        ("IDENTITY", sf::Transform::Identity);

    apollo::stack_reference transform_members(L, -1);

    apollo::push_class_metatable<sf::Transform>(L);
    apollo::rawset_table(L, -1)
        ("__index", transform_members)
        ("__mul", apollo::raw_function::caught<&transform_mul>());
    lua_pop(L, 1);

    return transform_members;
}
