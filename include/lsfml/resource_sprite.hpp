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
