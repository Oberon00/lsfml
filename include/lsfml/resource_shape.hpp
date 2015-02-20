#ifndef LSFML_RESOURCE_SHAPE_HPP_INCLUDED
#define LSFML_RESOURCE_SHAPE_HPP_INCLUDED

#include <lsfml/resource_media.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

namespace lsfml {

using resource_rectangle = resource_media<sf::RectangleShape, sf::Texture>;
using resource_circle = resource_media<sf::CircleShape, sf::Texture>;
using resource_convex = resource_media<sf::ConvexShape, sf::Texture>;

inline void update_resource_media(
    sf::Shape& s, std::shared_ptr<sf::Texture> const& tx)
{
    s.setTexture(tx.get());
}

} // namespace lsfml

#endif // LSFML_RESOURCE_SHAPE_HPP_INCLUDED
