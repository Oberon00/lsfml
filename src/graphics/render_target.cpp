// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#include "open_util.hpp"
#include "graphics.hpp"
#include <lsfml/rect.hpp>
#include <lsfml/vector.hpp>

#include <apollo/builtin_types.hpp>
#include <apollo/create_class.hpp>
#include <apollo/raw_function.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace {

int render_target_clear(lua_State* L)
{
    auto& self = apollo::to<sf::RenderTarget&>(L, 1);
    auto n_args = lua_gettop(L);
    if (n_args == 1)
        self.clear();
    else if (n_args == 2)
        self.clear(apollo::to<sf::Color const&>(L, 2).get());
    else
        throw std::invalid_argument("Bad argument count for RenderTarget.clear");
    return 0;
}

int render_target_map_pc(lua_State* L)
{
    auto& self = apollo::to<sf::RenderTarget&>(L, 1);
    auto point = apollo::to<sf::Vector2i>(L, 2);
    auto n_args = lua_gettop(L);
    if (n_args == 2) {
        apollo::push(L, self.mapPixelToCoords(point));
    } else if (n_args == 3) {
        auto view_cref = apollo::to<sf::View const&>(L, 3);
        apollo::push(L, self.mapPixelToCoords(point, view_cref.get()));
    } else {
        throw std::invalid_argument(
            "Bad argument count for RenderTarget.map_coords_to_pixel");
    }
    return 1;
}

int render_target_map_cp(lua_State* L)
{
    auto& self = apollo::to<sf::RenderTarget&>(L, 1);
    auto point = apollo::to<sf::Vector2f>(L, 2);
    auto n_args = lua_gettop(L);
    if (n_args == 2) {
        apollo::push(L, self.mapCoordsToPixel(point));
    } else if (n_args == 3) {
        auto view_cref = apollo::to<sf::View const&>(L, 3);
        apollo::push(L, self.mapCoordsToPixel(point, view_cref.get()));
    } else {
        throw std::invalid_argument(
            "Bad argument count for RenderTarget.map_coords_to_pixel");
    }
    return 1;
}

int render_target_draw(lua_State* L)
{
    auto& self = apollo::to<sf::RenderTarget&>(L, 1);
    auto drawable_cref = apollo::to<sf::Drawable const&>(L, 2);
    auto n_args = lua_gettop(L);
    if (n_args == 2) {
        self.draw(drawable_cref.get());
    } else if (n_args == 3) {
        self.draw(
            drawable_cref.get(),
            apollo::to<sf::RenderStates const&>(L, 3).get());
    } else {
        throw std::invalid_argument(
            "Bad argument count for RenderTarget.draw");
    }
    return 0;
}

} // anonymous namespace

apollo::stack_reference render_target_export(lua_State* L)
{
    apollo::new_table(L)
        ("clear", apollo::raw_function::caught<&render_target_clear>())
        ("set_view", FN(sf::RenderTarget::setView))
        ("view", FN(sf::RenderTarget::getView))
        ("default_view", FN(sf::RenderTarget::getDefaultView))
        ("viewport", FN(sf::RenderTarget::getViewport))
        ("map_pixel_to_coords",
            apollo::raw_function::caught<&render_target_map_pc>())
        ("map_coords_to_pixel",
            apollo::raw_function::caught<&render_target_map_cp>())
        ("draw", apollo::raw_function::caught<&render_target_draw>())
        ("size", FN(sf::RenderTarget::getSize))
        ("push_gl_states", FN(sf::RenderTarget::pushGLStates))
        ("pop_gl_states", FN(sf::RenderTarget::popGLStates))
        ("reset_gl_states", FN(sf::RenderTarget::resetGLStates));

    apollo::stack_reference render_target_members(L, -1);

    apollo::export_class<sf::RenderTarget>(L)
        ("__index", render_target_members);

    return render_target_members;
}
