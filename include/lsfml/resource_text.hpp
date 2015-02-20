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
