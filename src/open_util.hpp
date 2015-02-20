#ifndef LSFML_OPEN_UTIL_HPP_INCLUDED
#define LSFML_OPEN_UTIL_HPP_INCLUDED

#include <boost/config.hpp>
#include <apollo/error.hpp>
#include <apollo/to_raw_function.hpp>

#define LSFML_MODULE(name) \
    static int open(lua_State* L);                                     \
                                                                       \
    extern "C" BOOST_SYMBOL_EXPORT int luaopen_ ## name (lua_State* L) \
    {                                                                  \
        return apollo::exceptions_to_lua_errors_L(L, &open);           \
    }                                                                  \
    static int open(lua_State* L) // No semicolon.

#define FN(...) APOLLO_TO_RAW_FUNCTION(&__VA_ARGS__)

#endif // LSFML_OPEN_UTIL_HPP_INCLUDED