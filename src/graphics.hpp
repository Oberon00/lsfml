// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#ifndef LSFML_GRAPHICS_HPP_INCLUDED
#define LSFML_GRAPHICS_HPP_INCLUDED

#include "graphics/rect_export.hpp"

namespace apollo { class stack_reference; }

apollo::stack_reference transform_export(lua_State* L);
apollo::stack_reference render_target_export(lua_State* L);

#endif // LSFML_GRAPHICS_HPP_INCLUDED
