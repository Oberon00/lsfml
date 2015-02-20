#ifndef LSFML_GRAPHICS_HPP_INCLUDED
#define LSFML_GRAPHICS_HPP_INCLUDED

#include "graphics/rect_export.hpp"

namespace apollo { class stack_reference; }

apollo::stack_reference transform_export(lua_State* L);
apollo::stack_reference render_target_export(lua_State* L);

#endif // LSFML_GRAPHICS_HPP_INCLUDED
