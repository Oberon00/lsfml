#ifndef LSFML_VECFN_HPP_INCLUDED
#define LSFML_VECFN_HPP_INCLUDED

#include <lsfml/vector.hpp>

#include <apollo/builtin_types.hpp>
#include <apollo/raw_function.hpp>
#include <SFML/System/Vector2.hpp>

template <typename C, typename T,
    void(C::* Xyfn)(T, T),
    void(C::* Vecfn)(sf::Vector2<T> const&)>
int vec_fn_impl(lua_State* L)
{
    auto& self = apollo::to<C&>(L, 1);
    int const n_args = lua_gettop(L);
    if (lua_gettop(L) == 2) {
        (self.*Vecfn)(APOLLO_TO_ARG(L, 2, sf::Vector2<T> const&));
    } else if (n_args == 3) {
        (self.*Xyfn)(
            apollo::to<T>(L, 2),
            apollo::to<T>(L, 3));
    } else {
        throw std::invalid_argument(
            "Bad argument count! Expected two scalars or single vector.");
    }
    return 0;
}

template <typename C, typename T,
    void(C::* Xyfn)(T, T),
    void(C::* Vecfn)(sf::Vector2<T> const&)>
apollo::raw_function vec_fn()
{
    return apollo::raw_function::caught<
        &vec_fn_impl<C, T, Xyfn, Vecfn>>();
}

template <typename C,
    void(C::* Xyfn)(float, float),
    void(C::* Vecfn)(sf::Vector2f const&)>
apollo::raw_function vecf_fn()
{
    return apollo::raw_function::caught<
        &vec_fn_impl<C, float, Xyfn, Vecfn>>();
}


#endif // LSFML_VECFN_HPP_INCLUDED
