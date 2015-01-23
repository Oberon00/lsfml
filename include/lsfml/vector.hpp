#ifndef LSFML_VECTOR_HPP_INCLUDED
#define LSFML_VECTOR_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>
#include <apollo/class.hpp>

namespace lsfml {

using vector2l = sf::Vector2<lua_Number>;

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

    static void push(lua_State* L, type const& v)
    {
        apollo::push_object(L, static_cast<lsfml::vector2l>(v));
    }

    static unsigned n_conversion_steps(lua_State* L, int idx)
    {
        return add_conversion_step(
            apollo::n_object_conversion_steps<lsfml::vector2l>(L, idx));
    }

    static type from_stack(lua_State* L, int idx)
    {
        return static_cast<type>(unwrap_bound_ref(
            apollo::object_from_stack<lsfml::vector2l>(L, idx)));
    }
};

} // namespace apollo

#endif // LSFML_VECTOR_HPP_INCLUDED
