// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#include "graphics.hpp"
#include "open_util.hpp"
#include "vecfn.hpp"
#include "window_new.hpp"

#include <lsfml/vector.hpp>
#include <lsfml/rect.hpp>
#include <lsfml/string.hpp>
#include <lsfml/resource_sprite.hpp>
#include <lsfml/resource_shape.hpp>
#include <lsfml/resource_text.hpp>

#include <SFML/Graphics.hpp>

#include <apollo/create_class.hpp>
#include <apollo/operator.hpp>
#include <apollo/property.hpp>
#include <apollo/emplace_ctor.hpp>

namespace {


int color_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    if (n_args == 0) {
        apollo::emplace_object<sf::Color>(L);
    } else if (n_args == 3) {
        apollo::emplace_object<sf::Color>(L,
            apollo::to<sf::Uint8>(L, 1),
            apollo::to<sf::Uint8>(L, 2),
            apollo::to<sf::Uint8>(L, 3));
    } else if (n_args == 4) {
        apollo::emplace_object<sf::Color>(L,
            apollo::to<sf::Uint8>(L, 1),
            apollo::to<sf::Uint8>(L, 2),
            apollo::to<sf::Uint8>(L, 3),
            apollo::to<sf::Uint8>(L, 4));
    } else {
        throw std::invalid_argument("Bad argument count for Color.new");
    }
    return 1;
}

int view_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    if (n_args == 0) {
        apollo::emplace_object<sf::View>(L);
    } else if (n_args == 1) {
        apollo::emplace_object<sf::View>(L,
            APOLLO_TO_ARG(L, 1, sf::FloatRect const&));
    } else if (n_args == 2) {
        apollo::emplace_object<sf::View>(L,
            APOLLO_TO_ARG(L, 1, sf::Vector2f const&),
            APOLLO_TO_ARG(L, 2, sf::Vector2f const&));
    } else {
        throw std::invalid_argument("Bad argument count for View.new");
    }
    return 1;
}

int image_create(lua_State* L)
{
    auto& self = apollo::to<sf::Image&>(L, 1);
    self.create(
        apollo::to<unsigned>(L, 2), apollo::to<unsigned>(L, 3),
        apollo::unwrap_ref(apollo::to(L, 4, sf::Color())));
    return 0;
}

int image_mask(lua_State* L)
{
    auto& self = apollo::to<sf::Image&>(L, 1);
    self.createMaskFromColor(
        APOLLO_TO_ARG(L, 2, sf::Color), apollo::to<sf::Uint8>(L, 3, 0));
    return 0;
}

int image_copy(lua_State* L)
{
    auto& self = apollo::to<sf::Image&>(L, 1);
    int const n_args = lua_gettop(L);
    auto src = n_args >= 5 ? APOLLO_TO_ARG(L, 5, sf::IntRect) : sf::IntRect();
    auto apply_alpha = n_args >= 6 ? apollo::to<bool>(L, 6) : false;
    self.copy(
        APOLLO_TO_ARG(L, 2, sf::Image const&),
        apollo::to<unsigned>(L, 3),
        apollo::to<unsigned>(L, 4),
        src,
        apply_alpha);
    return 0;
}

void image_set_pixel(sf::Image& self, unsigned x, unsigned y, sf::Color c)
{
    auto sz = self.getSize();
    if (x < sz.x && y < sz.y)
        self.setPixel(x, y, c);
    else
        throw std::out_of_range("Image.set_pixel: coordinates out of bounds.");
}

sf::Color image_pixel(sf::Image const& self, unsigned x, unsigned y)
{
    auto sz = self.getSize();
    if (x < sz.x && y < sz.y)
        return self.getPixel(x, y);
    else
        throw std::out_of_range("Image.pixel: coordinates out of bounds.");
}

int texture_update(lua_State* L)
{
    auto& self = apollo::to<sf::Texture&>(L, 1);
    unsigned x = 0, y = 0;
    if (lua_gettop(L) > 2) {
        x = apollo::to<unsigned>(L, 3);
        y = apollo::to<unsigned>(L, 4);
    }

    auto const check_args = [x, y, &self](sf::Vector2u sz) -> void {
        auto right = sz.x + x, bottom = sz.y + y;
        if (right >= self.getSize().x || bottom >= self.getSize().y) {
            throw std::out_of_range(
                "Texture.update: offset + size out of bounds.");
        }
    };

    if (!apollo::is_convertible<sf::Window const&>(L, 2)) {
        if (!apollo::is_convertible<sf::Image const&>(L, 2)) {
            throw std::invalid_argument(
                "Texture.update: Image or Window expected.");
        }
        auto img_ref = apollo::unchecked_to<sf::Image const&>(L, 2);
        check_args(apollo::unwrap_ref(img_ref).getSize());
        self.update(apollo::unwrap_ref(img_ref), x, y);
    } else {
        auto wnd_ref = apollo::unchecked_to<sf::Window const&>(L, 2);
        check_args(apollo::unwrap_ref(wnd_ref).getSize());
        self.update(apollo::unwrap_ref(wnd_ref), x, y);
    }

    return 0;
}

int texture_load_img(lua_State* L)
{
    auto& self = apollo::to<sf::Texture&>(L, 1);
    auto area_ref = apollo::to<sf::IntRect>(L, 3, sf::IntRect());
    return apollo::push(L, self.loadFromImage(
        APOLLO_TO_ARG(L, 2, sf::Image const&),
        apollo::unwrap_ref(area_ref)));
}

int sprite_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    if (n_args == 0) {
        apollo::emplace_object<lsfml::resource_sprite>(L);
    } else if (n_args == 1 || n_args == 2) {
        apollo::emplace_object<lsfml::resource_sprite>(L,
            APOLLO_TO_ARG(L, 1, lsfml::resource_sprite::res_ptr));
        if (n_args == 2) {
            auto& self = apollo::unchecked_to<lsfml::resource_sprite&>(L, -1);
            self.setTextureRect(APOLLO_TO_ARG(L, 2, sf::IntRect));
        }
    } else {
        throw std::invalid_argument("Bad argument count for View.new");
    }
    return 1;
}

sf::Vector2f shape_point(sf::Shape const& self, std::size_t index)
{
    if (index < self.getPointCount())
        return self.getPoint(index);
    throw std::out_of_range("Shape.point: index out of range");
}

int rectangle_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    sf::Vector2f sz;
    if (n_args == 1) {
        sz = APOLLO_TO_ARG(L, -1, sf::Vector2f const&);
    } else if (n_args != 0) {
        throw std::invalid_argument(
            "Bad argument count for RectangleShape.new");
    }
    apollo::emplace_object<lsfml::resource_rectangle>(L);
    apollo::unchecked_to<lsfml::resource_rectangle&>(L, -1).setSize(sz);
    return 1;
}

int circle_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    float r = 0;
    unsigned n_points = 30;
    if (n_args >= 1)
        r = apollo::to<float>(L, 1);
    if (n_args == 2)
        n_points = apollo::to<unsigned>(L, 2);
    else if (n_args > 2)
        throw std::invalid_argument("Bad argument count for CircleShape.new");

    apollo::emplace_object<lsfml::resource_circle>(L);
    auto& self = apollo::unchecked_to<lsfml::resource_circle&>(L, -1);
    self.setPointCount(n_points);
    self.setRadius(r);
    return 1;
}

int convex_new(lua_State* L)
{
    int const n_args = lua_gettop(L);
    unsigned n_points = 0;
    if (n_args == 1)
        n_points = apollo::to<unsigned>(L, 1);
    else if (n_args > 1)
        throw std::invalid_argument("Bad argument count for ConvexShape.new");

    apollo::emplace_object<lsfml::resource_convex>(L);
    auto& self = apollo::unchecked_to<lsfml::resource_convex&>(L, -1);
    self.setPointCount(n_points);
    return 1;
}

void convex_set_point(
    sf::ConvexShape& self, std::size_t index, sf::Vector2f const& p)
{
    if (index < self.getPointCount())
        self.setPoint(index, p);
    else
        throw std::out_of_range("ConvexShape.set_point: index out of range");
}

int text_new(lua_State* L)
{
    int const n_args = lua_gettop(L);

    if (n_args != 0 && n_args != 2 && n_args != 3)
        throw std::invalid_argument("Bad argument count for Text.new");

    apollo::emplace_object<lsfml::resource_text>(L);

    auto& self = apollo::unchecked_to<lsfml::resource_text&>(L, -1);
    if (n_args > 0) {
        self.setString(APOLLO_TO_ARG(L, 1, sf::String const&));
        self.set_resource(apollo::to<lsfml::resource_text::res_ptr>(L, 2));
        if (n_args > 2)
            self.setCharacterSize(apollo::to<unsigned>(L, 3));
    }
    return 1;
}

std::string const& font_family(sf::Font const& self)
{
    return self.getInfo().family;
}

template <typename T>
void add_texture_members(lua_State* L, int idx)
{
    apollo::rawset_table(L, idx)
        ("set_texture", FN(T::set_resource))
        ("texture", FN(T::resource))
        ("set_texture_rect", FN(T::setTextureRect))
        ("texture_rect", FN(T::getTextureRect));
}

template <typename T>
void add_bounds_members(lua_State* L, int idx)
{
    apollo::rawset_table(L, idx)
        ("local_bounds", FN(T::getLocalBounds))
        ("global_bounds", FN(T::getGlobalBounds));
}

} // anonymous namespace

LSFML_MODULE(lsfml_graphics)
{
    lua_newtable(L);
    int const mod_table = lua_gettop(L);
    auto const set_module = [L, mod_table]() {
        lua_rawset(L, mod_table);
    };

    // Color //
    lua_pushliteral(L, "Color");
    apollo::register_class<sf::Color>(L);
#define PROP(n) \
        (#n, FN(APOLLO_MEMBER_GETTER(sf::Color::n))) \
        ("set_" #n, FN(APOLLO_MEMBER_SETTER(sf::Color::n)))
#define OP(n) \
        ("__" #n, FN(apollo::op::##n<sf::Color const&, sf::Color const&>))
    apollo::new_table(L)
        ("new", apollo::raw_function::caught<&color_new>())
        OP(eq)
        OP(add)
        OP(sub)
        OP(mul)
        PROP(r)
        PROP(g)
        PROP(b)
        PROP(a)
        ("BLACK", sf::Color::Black)
        ("WHITE", sf::Color::White)
        ("RED", sf::Color::Red)
        ("GREEN", sf::Color::Green)
        ("BLUE", sf::Color::Blue)
        ("YELLOW", sf::Color::Yellow)
        ("MAGENTA", sf::Color::Magenta)
        ("CYAN", sf::Color::Cyan)
        ("TRANSPARENT", sf::Color::Transparent);
#undef OP
#undef PROP
    apollo::push_class_metatable<sf::Color>(L);
    apollo::rawset_table(L, -1)("__index", apollo::stack_reference(L, -1));
    lua_pop(L, 1);
    set_module();

    // Transformable //
#define TFN(n) vecf_fn<sf::Transformable, \
    &sf::Transformable::n, &sf::Transformable::n>()
    apollo::new_table(L)
        ("set_position", TFN(setPosition))
        ("set_rotation", FN(sf::Transformable::setRotation))
        ("set_scale", TFN(setScale))
        ("set_origin", TFN(setOrigin))
        ("position", FN(sf::Transformable::getPosition))
        ("rotation", FN(sf::Transformable::getRotation))
        ("scale", FN(sf::Transformable::getScale))
        ("origin", FN(sf::Transformable::getOrigin))
        ("move", TFN(move))
        ("rotate", FN(sf::Transformable::rotate))
        ("transform", FN(sf::Transformable::getTransform))
        ("inverse_transform", FN(sf::Transformable::getInverseTransform))
        ("scale", TFN(scale));
#undef TFN
    apollo::export_class<sf::Transformable>(L)
        ("__index", apollo::stack_reference(L, -1));
    lua_pushliteral(L, "Transformable");
    lua_pushvalue(L, -2);
    set_module();
    int const transformable_members = lua_gettop(L);

    // View //
    lua_pushliteral(L, "View");
#define VFN(n) vecf_fn<sf::View, &sf::View::n, &sf::View::n>()
    apollo::new_table(L)
        ("new", apollo::raw_function::caught<&view_new>())
        ("set_center", VFN(setCenter))
        ("set_size", VFN(setSize))
        ("set_rotation", FN(sf::View::setRotation))
        ("set_viewport", FN(sf::View::setViewport))
        ("reset", FN(sf::View::reset))
        ("center", FN(sf::View::getCenter))
        ("size", FN(sf::View::getSize))
        ("rotation", FN(sf::View::getRotation))
        ("viewport", FN(sf::View::getViewport))
        ("move", VFN(move))
        ("rotate", FN(sf::View::rotate))
        ("zoom", FN(sf::View::zoom))
        ("transform", FN(sf::View::getTransform))
        ("inverse_transform", FN(sf::View::getInverseTransform));
#undef VFN
    apollo::export_class<sf::View>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // Image //
    lua_pushliteral(L, "Image");
    apollo::new_table(L)
        ("new", apollo::get_raw_emplace_ctor_wrapper<sf::Image>())
        ("create", apollo::raw_function::caught<&image_create>())
        ("load_from_file", FN(sf::Image::loadFromFile))
        ("save_to_file", FN(sf::Image::saveToFile))
        ("size", FN(sf::Image::getSize))
        ("create_mask_from_color", apollo::raw_function::caught<&image_mask>())
        ("copy", apollo::raw_function::caught<&image_copy>())
        ("set_pixel", FN(image_set_pixel))
        ("pixel", FN(image_pixel))
        ("flip_horizontally", FN(sf::Image::flipHorizontally))
        ("flip_vertically", FN(sf::Image::flipVertically));
    apollo::export_class<sf::Image>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // Texture //
    lua_pushliteral(L, "Texture");
    apollo::new_table(L)
        ("new", FN(apollo::new_wrapper<std::shared_ptr<sf::Texture>>))
        ("create", FN(sf::Texture::create))
        ("load_from_file", FN(sf::Texture::loadFromFile))
        ("load_from_image", apollo::raw_function::caught<&texture_load_img>())
        ("size", FN(sf::Texture::getSize))
        ("copy_to_image", FN(sf::Texture::copyToImage))
        ("update", apollo::raw_function::caught<&texture_update>())
        ("set_smooth", FN(sf::Texture::setSmooth))
        ("is_smooth", FN(sf::Texture::isSmooth))
        ("set_repeated", FN(sf::Texture::setRepeated))
        ("is_repeated", FN(sf::Texture::isRepeated))
        ("flip_horizontally", FN(sf::Image::flipHorizontally))
        ("flip_vertically", FN(sf::Image::flipVertically))
        ("maximum_size", FN(sf::Texture::getMaximumSize));
    apollo::export_class<sf::Texture>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // Font //
    lua_pushliteral(L, "Font");
    apollo::new_table(L)
        ("new", FN(apollo::new_wrapper<std::shared_ptr<sf::Font>>))
        ("load_from_file", FN(sf::Font::loadFromFile))
        ("family", FN(font_family)) // Replaces getInfo
        ("glyph", FN(sf::Font::getGlyph))
        ("kerning", FN(sf::Font::getKerning))
        ("line_spacing", FN(sf::Font::getLineSpacing))
        ("underline_position", FN(sf::Font::getUnderlinePosition))
        ("underline_thickness", FN(sf::Font::getUnderlineThickness));
        // Missing: getTexture
    apollo::export_class<sf::Font>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // Glyph //
    lua_pushliteral(L, "Glyph");
    apollo::new_table(L)
        ("advance", FN(APOLLO_MEMBER_GETTER(sf::Glyph::advance)))
        ("bounds", FN(APOLLO_MEMBER_GETTER(sf::Glyph::bounds)))
        ("textureRect", FN(APOLLO_MEMBER_GETTER(sf::Glyph::textureRect)));
        // Missing: getTexture
    apollo::export_class<sf::Glyph>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // Drawable //
    apollo::register_class<sf::Drawable>(L);

    // Sprite (resource_sprite) //
    lua_pushliteral(L, "Sprite");
    lua_newtable(L);
    apollo::extend_table(L, -1, transformable_members);
    apollo::rawset_table(L, -1)
        ("new", apollo::raw_function::caught<&sprite_new>())
        ("set_color", FN(lsfml::resource_sprite::setColor))
        ("color", FN(lsfml::resource_sprite::getColor));
    add_bounds_members<lsfml::resource_sprite>(L, -1);
    add_texture_members<lsfml::resource_sprite>(L, -1);
    apollo::register_class<sf::Sprite, sf::Drawable, sf::Transformable>(L);
    apollo::export_class<lsfml::resource_sprite, sf::Sprite>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // Shape //
    lua_newtable(L);
    apollo::extend_table(L, -1, transformable_members);
    apollo::rawset_table(L, -1)
        ("set_fill_color", FN(sf::Shape::setFillColor))
        ("fill_color", FN(sf::Shape::getFillColor))
        ("set_outline_color", FN(sf::Shape::setOutlineColor))
        ("outline_color", FN(sf::Shape::getOutlineColor))
        ("set_outline_thickness", FN(sf::Shape::setOutlineThickness))
        ("outline_thickness", FN(sf::Shape::getOutlineThickness))
        ("point_count", FN(sf::Shape::getPointCount))
        ("point", FN(shape_point));
    add_bounds_members<sf::Shape>(L, -1);
    apollo::export_class<sf::Shape, sf::Drawable, sf::Transformable>(L)
        ("__index", apollo::stack_reference(L, -1));
    lua_pushliteral(L, "Shape");
    lua_pushvalue(L, -2);
    set_module();
    int const shape_members = lua_gettop(L);


    // RectangleShape //
    lua_pushliteral(L, "RectangleShape");
    lua_newtable(L);
    apollo::extend_table(L, -1, shape_members);
    apollo::rawset_table(L, -1)
        ("new", apollo::raw_function::caught<&rectangle_new>())
        ("set_size", FN(lsfml::resource_rectangle::setSize))
        ("size", FN(lsfml::resource_rectangle::getSize));
    add_texture_members<lsfml::resource_rectangle>(L, -1);
    apollo::register_class<sf::RectangleShape, sf::Shape>(L);
    apollo::export_class<lsfml::resource_rectangle, sf::RectangleShape>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // CircleShape //
    lua_pushliteral(L, "CircleShape");
    lua_newtable(L);
    apollo::extend_table(L, -1, shape_members);
    apollo::rawset_table(L, -1)
        ("new", apollo::raw_function::caught<&circle_new>())
        ("set_radius", FN(lsfml::resource_circle::setRadius))
        ("radius", FN(lsfml::resource_circle::getRadius))
        ("set_point_count", FN(lsfml::resource_circle::setPointCount));
    add_texture_members<lsfml::resource_circle>(L, -1);
    apollo::register_class<sf::CircleShape, sf::Shape>(L);
    apollo::export_class<lsfml::resource_circle, sf::CircleShape>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // ConvexShape //
    lua_pushliteral(L, "ConvexShape");
    lua_newtable(L);
    apollo::extend_table(L, -1, shape_members);
    apollo::rawset_table(L, -1)
        ("new", apollo::raw_function::caught<&convex_new>())
        ("set_point", FN(convex_set_point));
    add_texture_members<lsfml::resource_convex>(L, -1);
    apollo::register_class<sf::ConvexShape, sf::Shape>(L);
    apollo::export_class<lsfml::resource_convex, sf::ConvexShape>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // RenderTarget //
    render_target_export(L);
    int const render_target_members = lua_gettop(L);
    lua_pushliteral(L, "RenderTarget");
    lua_pushvalue(L, -2);
    set_module();

    // Text //
    lua_pushliteral(L, "Text");
    lua_newtable(L);
    apollo::extend_table(L, -1, transformable_members);
    apollo::rawset_table(L, -1)
        ("new", apollo::raw_function::caught<&text_new>())
        ("set_string", FN(lsfml::resource_text::setString))
        ("string", FN(lsfml::resource_text::getString))
        ("font", FN(lsfml::resource_text::resource))
        ("set_font", FN(lsfml::resource_text::set_resource))
        ("character_size", FN(lsfml::resource_text::getCharacterSize))
        ("set_character_size", FN(lsfml::resource_text::setCharacterSize))
        ("set_style", FN(lsfml::resource_text::setStyle))
        ("style", FN(lsfml::resource_text::getStyle))
        ("set_color", FN(lsfml::resource_text::setColor))
        ("get_color", FN(lsfml::resource_text::getColor))
        ("find_character_pos", FN(lsfml::resource_text::findCharacterPos))
        ("REGULAR", sf::Text::Regular)
        ("BOLD", sf::Text::Bold)
        ("ITALIC", sf::Text::Italic)
        ("UNDERLINED", sf::Text::Underlined)
        ("STRIKE_THROUGH", sf::Text::StrikeThrough);
    add_bounds_members<lsfml::resource_text>(L, -1);
    apollo::register_class<sf::Text, sf::Drawable, sf::Transformable>(L);
    apollo::export_class<lsfml::resource_text, sf::Text>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    // RenderWindow //
    lua_pushliteral(L, "RenderWindow");
    lua_newtable(L);
    apollo::extend_table(L, -1, render_target_members);
    // TODO: Ensure Window is registered.
    apollo::push_class_metatable<sf::Window>(L);
    lua_pushliteral(L, "__index");
    lua_rawget(L, -2);
    BOOST_ASSERT(lua_type(L, -1) == LUA_TTABLE);
    apollo::extend_table(L, -3, -1);
    lua_pop(L, 2); // Pop Window metatable and __index table.
    apollo::rawset_table(L, -1)
        ("new", apollo::raw_function::caught<&window_new<sf::RenderWindow>>())
        ("capture", FN(sf::RenderWindow::capture));
    apollo::export_class<sf::RenderWindow, sf::RenderTarget, sf::Window>(L)
        ("__index", apollo::stack_reference(L, -1));
    set_module();

    lua_pushliteral(L, "Rect");
    rect_export<lua_Number>(L);
    set_module();

    lua_pushliteral(L, "Transform");
    transform_export(L);
    set_module();

    lua_settop(L, mod_table);
    return 1;
}
