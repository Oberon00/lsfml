#include <boost/config.hpp>
#include <apollo/error.hpp>
#include <apollo/builtin_types.hpp>

#define LSFML_MODULE(name) \
    static int open(lua_State* L);                                     \
                                                                       \
    extern "C" BOOST_SYMBOL_EXPORT int luaopen_ ## name (lua_State* L) \
    {                                                                  \
        return apollo::exceptions_to_lua_errors_L(L, &open);           \
    }                                                                  \
    static int open(lua_State* L) // No semicolon.

#define FN(...) APOLLO_TO_RAW_FUNCTION(&__VA_ARGS__)