// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#ifndef LSFML_RESOURCE_SPRITE_HPP_INCLUDED
#define LSFML_RESOURCE_SPRITE_HPP_INCLUDED

#include <lsfml/resource_media.hpp>

#include <SFML/Graphics/Sprite.hpp>

namespace lsfml {

using resource_sprite = resource_media<sf::Sprite, sf::Texture>;

inline void update_resource_media(
    sf::Sprite& s, resource_sprite::res_ptr const& tx)
{
    s.setTexture(*tx);
}

} // namespace lsfml

#endif // LSFML_RESOURCE_SPRITE_HPP_INCLUDED
