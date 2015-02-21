// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#ifndef LSFML_RESOURCE_TEXT_HPP_INCLUDED
#define LSFML_RESOURCE_TEXT_HPP_INCLUDED

#include <lsfml/resource_media.hpp>

#include <SFML/Graphics/Text.hpp>

namespace lsfml {

using resource_text = resource_media<sf::Text, sf::Font>;

inline void update_resource_media(
    sf::Text& s, resource_text::res_ptr const& fnt)
{
    s.setFont(*fnt);
}
} // namespace lsfml

#endif // LSFML_RESOURCE_TEXT_HPP_INCLUDED
