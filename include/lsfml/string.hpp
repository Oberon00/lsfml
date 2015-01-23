#ifndef LSFML_STRING_HPP_INCLUDED
#define LSFML_STRING_HPP_INCLUDED

#include <SFML/System/String.hpp>

#include <apollo/builtin_types.hpp>

namespace apollo {

template <>
struct convert_cref_by_val<sf::String>: std::true_type {};

template<>
struct converter<sf::String>: converter_base<sf::String> {

    static void push(lua_State* L, sf::String const& v)
    {
        std::basic_string<sf::Uint8> const str = v.toUtf8();
        lua_pushlstring(L,
            reinterpret_cast<char const*>(str.data()),
            str.size());
    }

    static unsigned n_conversion_steps(lua_State* L, int idx)
    {
        return apollo::n_conversion_steps<std::string>(L, idx);
    }

    static sf::String from_stack(lua_State* L, int idx)
    {
        size_t len;
        char const* str = lua_tolstring(L, idx, &len);
        return sf::String::fromUtf8(str, str + len);
    }
};

} // namespace apollo

#endif // LSFML_STRING_HPP_INCLUDED
