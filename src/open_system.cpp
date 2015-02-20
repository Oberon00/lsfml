#include "open_util.hpp"

#include <lsfml/vector.hpp>

#include <apollo/create_class.hpp>
#include <apollo/operator.hpp>
#include <apollo/property.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Vector2.hpp>


std::string time_to_str(sf::Time const& t)
{
    if (t.asSeconds() >= 1.f)
        return std::to_string(t.asSeconds()) + 's';
    return std::to_string(t.asMicroseconds()) + "us";
}

template <typename T>
std::string vec2_to_str(sf::Vector2<T> const& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}

template <typename T>
int new_vec2(lua_State* L)
{
    if (lua_gettop(L) == 0) {
        apollo::push(L, sf::Vector2<T>());
    } else {
        apollo::push(L, sf::Vector2<T>(
            apollo::to<T>(L, 1), apollo::to<T>(L, 2)));
    }
    return 1;
}

template <typename T>
apollo::stack_reference export_vector2(lua_State* L)
{
    using vec = sf::Vector2<T>;
    apollo::new_table(L)
        ("x", FN(APOLLO_MEMBER_GETTER(vec::x)))
        ("set_x", FN(APOLLO_MEMBER_SETTER(vec::x)))
        ("y", FN(APOLLO_MEMBER_GETTER(vec::y)))
        ("set_y", FN(APOLLO_MEMBER_SETTER(vec::y)))
        ("new", apollo::raw_function::caught<&new_vec2<T>>());

    apollo::stack_reference vec_members(L, -1);

#define OP(n) \
    ("__" #n, FN(apollo::op::##n<vec const&, vec const&>))

    apollo::export_class<sf::Vector2<T>>(L)
        ("__index", vec_members)
        ("__tostring", FN(vec2_to_str<T>))
        OP(eq)
        OP(add)
        OP(sub)
        ("__mul", FN(apollo::op::mul<vec const&, T>))
        ("__div", FN(apollo::op::div<vec const&, T>));

#undef OP

    return vec_members;
}

template <typename T>
std::string vec3_to_str(sf::Vector3<T> const& v)
{
    return "("
        + std::to_string(v.x)
        + ", " + std::to_string(v.y)
        + ", " + std::to_string(v.z)
        + ")";
}

template <typename T>
int new_vec3(lua_State* L)
{
    if (lua_gettop(L) == 0) {
        apollo::push(L, sf::Vector3<T>());
    } else {
        apollo::push(L, sf::Vector3<T>(
            apollo::to<T>(L, 1),
            apollo::to<T>(L, 2),
            apollo::to<T>(L, 3)));
    }
    return 1;
}

template <typename T>
apollo::stack_reference export_vector3(lua_State* L)
{
    using vec = sf::Vector3<T>;
    apollo::new_table(L)
        ("x", FN(APOLLO_MEMBER_GETTER(vec::x)))
        ("set_x", FN(APOLLO_MEMBER_SETTER(vec::x)))
        ("y", FN(APOLLO_MEMBER_GETTER(vec::y)))
        ("set_y", FN(APOLLO_MEMBER_SETTER(vec::y)))
        ("z", FN(APOLLO_MEMBER_GETTER(vec::z)))
        ("set_z", FN(APOLLO_MEMBER_SETTER(vec::z)))
        ("new", apollo::raw_function::caught<&new_vec3<T>>());

    apollo::stack_reference vec_members(L, -1);

#define OP(n) \
    ("__" #n, FN(apollo::op::##n<vec const&, vec const&>))

    apollo::export_class<sf::Vector3<T>>(L)
        ("__index", vec_members)
        ("__tostring", FN(vec3_to_str<T>))
        OP(eq)
        OP(add)
        OP(sub)
        ("__mul", FN(apollo::op::mul<vec const&, T>))
        ("__div", FN(apollo::op::div<vec const&, T>));

#undef OP

    return vec_members;
}

LSFML_MODULE(lsfml_system)
{
    apollo::register_class<sf::Time>(L);
    apollo::new_table(L) // Time
        ("as_seconds", FN(sf::Time::asSeconds))
        ("as_milliseconds", FN(sf::Time::asMilliseconds))
        ("as_microseconds", FN(sf::Time::asMicroseconds))
        ("ZERO", sf::Time::Zero);

    apollo::stack_reference time_members(L, -1);

#define OP(n) \
    ("__" #n, FN(apollo::op::##n<sf::Time const&, sf::Time const&>))

    apollo::push_class_metatable<sf::Time>(L);
    apollo::rawset_table(L, -1)
        ("__index", time_members)
        ("__tostring", FN(time_to_str))
        OP(eq)
        OP(add)
        OP(lt)
        OP(le)
        OP(sub)
        OP(mod)
        ("__mul", FN(apollo::op::mul<sf::Time const&, float>))
        ("__div", FN(apollo::op::div<sf::Time const&, float>))
        ("__unm", FN(apollo::op::unm<sf::Time const&>));
#undef OP
    lua_pop(L, 1);


    apollo::new_table(L) // Clock
        ("new", FN(apollo::ctor_wrapper<sf::Clock>))
        ("elapsed_time", FN(sf::Clock::getElapsedTime))
        ("restart", FN(sf::Clock::restart));

    apollo::stack_reference clock_members(L, -1);

    apollo::export_class<sf::Clock>(L)("__index", clock_members);

    auto vec2_members = export_vector2<lua_Number>(L);
    auto vec3_members = export_vector3<lua_Number>(L);

    apollo::new_table(L) // System module
        ("Time", time_members)
        ("Clock", clock_members)
        ("Vec2", vec2_members)
        ("Vec3", vec3_members)
        ("seconds", FN(sf::seconds))
        ("milliseconds", FN(sf::milliseconds))
        ("microseconds", FN(sf::microseconds))
        ("sleep", FN(sf::sleep));
    return 1;
}