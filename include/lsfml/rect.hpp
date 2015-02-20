#ifndef LSFML_RECT_HPP_INCLUDED
#define LSFML_RECT_HPP_INCLUDED

#include <SFML/Graphics/Rect.hpp>
#include <apollo/class.hpp>

namespace lsfml {

using lrect = sf::Rect<lua_Number>;

} // namespace lsfml

namespace apollo {

template <typename T>
struct convert_cref_by_val<sf::Rect<T>>: std::true_type {};

template <>
struct convert_cref_by_val<lsfml::lrect>: std::false_type {};

template<typename T>
struct converter<sf::Rect<T>,
    typename std::enable_if<!std::is_same<T, lua_Number>::value>::type  
>: converter_base<sf::Rect<T>> {

    using type = sf::Rect<T>;

    static int push(lua_State* L, type const& v)
    {
        apollo::push_object(L, static_cast<lsfml::lrect>(v));
        return 1;
    }

    static unsigned n_conversion_steps(lua_State* L, int idx)
    {
        return add_conversion_step(
            apollo::n_object_conversion_steps<lsfml::lrect>(L, idx));
    }

    static type to(lua_State* L, int idx)
    {
        return static_cast<type>(unwrap_ref(
            apollo::to_object<lsfml::lrect>(L, idx)));
    }
};

} // namespace apollo

#endif // LSFML_RECT_HPP_INCLUDED
