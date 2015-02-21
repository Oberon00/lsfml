// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#ifndef LSFML_VECTOR_HPP_INCLUDED
#define LSFML_VECTOR_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <apollo/class.hpp>

namespace lsfml {

using vector2l = sf::Vector2<lua_Number>;
using vector3l = sf::Vector3<lua_Number>;

} // namespace lsfml

namespace apollo {

template <typename T>
struct convert_cref_by_val<sf::Vector2<T>>: std::true_type {};

template <>
struct convert_cref_by_val<lsfml::vector2l>: std::false_type {};

template<typename T>
struct converter<sf::Vector2<T>,
    typename std::enable_if<!std::is_same<T, lua_Number>::value>::type
>: converter_base<sf::Vector2<T>> {

    using type = sf::Vector2<T>;

    static int push(lua_State* L, type const& v)
    {
        apollo::push_object(L, static_cast<lsfml::vector2l>(v));
        return 1;
    }

    static unsigned n_conversion_steps(lua_State* L, int idx)
    {
        return add_conversion_step(
            apollo::n_object_conversion_steps<lsfml::vector2l>(L, idx));
    }

    static type to(lua_State* L, int idx)
    {
        return static_cast<type>(unwrap_ref(
            apollo::to_object<lsfml::vector2l>(L, idx)));
    }
};

template <typename T>
struct convert_cref_by_val<sf::Vector3<T>>: std::true_type {};

template <>
struct convert_cref_by_val<lsfml::vector3l>: std::false_type {};

template<typename T>
struct converter<sf::Vector3<T>,
    typename std::enable_if<!std::is_same<T, lua_Number>::value>::type
>: converter_base<sf::Vector3<T>> {

    using type = sf::Vector3<T>;

    static int push(lua_State* L, type const& v)
    {
        apollo::push_object(L, static_cast<lsfml::vector3l>(v));
        return 1;
    }

    static unsigned n_conversion_steps(lua_State* L, int idx)
    {
        return add_conversion_step(
            apollo::n_object_conversion_steps<lsfml::vector3l>(L, idx));
    }

    static type to(lua_State* L, int idx)
    {
        return static_cast<type>(unwrap_ref(
            apollo::to_object<lsfml::vector3l>(L, idx)));
    }
};
} // namespace apollo

#endif // LSFML_VECTOR_HPP_INCLUDED
