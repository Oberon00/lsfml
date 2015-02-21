// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#ifndef LSFML_RESOURCE_SOUND_HPP_INCLUDED
#define LSFML_RESOURCE_SOUND_HPP_INCLUDED

#include <lsfml/resource_media.hpp>

#include <SFML/Audio/Sound.hpp>

namespace lsfml {

using resource_sound = resource_media<sf::Sound, sf::SoundBuffer>;

inline void update_resource_media(
    sf::Sound& s, resource_sound::res_ptr const& sbuf)
{
    s.setBuffer(*sbuf);
}

} // namespace lsfml

#endif // LSFML_RESOURCE_SOUND_HPP_INCLUDED
