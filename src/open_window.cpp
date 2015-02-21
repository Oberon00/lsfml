// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#include "open_util.hpp"

#include "window_new.hpp"

#include <lsfml/vector.hpp>
#include <lsfml/string.hpp>

#include <SFML/Window.hpp>

#include <apollo/create_class.hpp>
#include <apollo/operator.hpp>
#include <apollo/property.hpp>
#include <apollo/emplace_ctor.hpp>

namespace {

std::string video_mode_to_str(sf::VideoMode const& v)
{
    return std::to_string(v.width)
        + 'x' + std::to_string(v.height)
        + '@' + std::to_string(v.bitsPerPixel) + "bpp";
}

int new_videomode(lua_State* L)
{
    int const n_args = lua_gettop(L);
    switch (n_args) {
        case 0:
            apollo::emplace_object<sf::VideoMode>(L);
        break;

        case 2:
            apollo::emplace_object<sf::VideoMode>(L,
                apollo::to<unsigned>(L, 1),
                apollo::to<unsigned>(L, 2));
        break;

        case 3:
            apollo::emplace_object<sf::VideoMode>(L,
                apollo::to<unsigned>(L, 1),
                apollo::to<unsigned>(L, 2),
                apollo::to<unsigned>(L, 3));
        break;

        default:
            throw std::invalid_argument(
                "Bad argument count for VideoMode constructor.");
    }
    return 1;
}

std::size_t n_fullscreen_modes()
{
    return sf::VideoMode::getFullscreenModes().size();
}

sf::VideoMode const* get_fullscreen_mode(std::size_t i)
{
    return &sf::VideoMode::getFullscreenModes().at(i - 1);
}

int create_window(lua_State* L)
{
    auto& w = apollo::to<sf::Window&>(L, 1);
    int const n_args = lua_gettop(L);
    switch (n_args) {
        case 3:
            w.create(
                apollo::to<sf::VideoMode>(L, 2).get(),
                apollo::to<sf::String>(L, 3));
        break;

        case 4:
            w.create(
                apollo::to<sf::VideoMode>(L, 2).get(),
                apollo::to<sf::String>(L, 3),
                apollo::to<sf::Uint32>(L, 4));
        break;

        case 5:
            w.create(
                apollo::to<sf::VideoMode>(L, 2).get(),
                apollo::to<sf::String>(L, 3),
                apollo::to<sf::Uint32>(L, 4),
                apollo::to<sf::ContextSettings>(L, 5).get());
        break;

        default:
            throw std::invalid_argument(
                "Bad argument count for Window.create().");
    }
    return 0;
}

int index_event(lua_State* L)
{
    auto const& self = *apollo::to<sf::Event const*>(L, 1);
    auto prop = apollo::to<char const*>(L, 2);
    if (std::strcmp(prop, "type") == 0) {
        apollo::push(L, self.type);
    } else switch (self.type) {
        case sf::Event::Resized: // size
            if (std::strcmp(prop, "width") == 0)
                apollo::push(L, self.size.width);
            else if (std::strcmp(prop, "height") == 0)
                apollo::push(L, self.size.height);
            else
                lua_pushnil(L);
        break;

        case sf::Event::KeyPressed:  // key
        case sf::Event::KeyReleased: //
            if (std::strcmp(prop, "code") == 0)
                apollo::push(L, self.key.code);
            else if (std::strcmp(prop, "control") == 0)
                apollo::push(L, self.key.control);
            else if (std::strcmp(prop, "alt") == 0)
                apollo::push(L, self.key.alt);
            else if (std::strcmp(prop, "shift") == 0)
                apollo::push(L, self.key.shift);
            else if (std::strcmp(prop, "system") == 0)
                apollo::push(L, self.key.system);
            else
                lua_pushnil(L);
        break;

        case sf::Event::TextEntered: // text
            if (std::strcmp(prop, "unicode") == 0)
                apollo::push(L, self.text.unicode);
            else
                lua_pushnil(L);
        break;

        case sf::Event::MouseMoved: // mouseMove
            if (std::strcmp(prop, "x") == 0)
                apollo::push(L, self.mouseMove.x);
            else if (std::strcmp(prop, "y") == 0)
                apollo::push(L, self.mouseMove.y);
            else
                lua_pushnil(L);
        break;

        case sf::Event::MouseButtonPressed:  // mouseButton
        case sf::Event::MouseButtonReleased: //
            if (std::strcmp(prop, "button") == 0)
                apollo::push(L, self.mouseButton.button);
            else if (std::strcmp(prop, "x") == 0)
                apollo::push(L, self.mouseButton.x);
            else if (std::strcmp(prop, "y") == 0)
                apollo::push(L, self.mouseButton.y);
            else
                lua_pushnil(L);
        break;

        case sf::Event::MouseWheelMoved: // mouseWheel
            if (std::strcmp(prop, "x") == 0)
                apollo::push(L, self.mouseWheel.x);
            else if (std::strcmp(prop, "y") == 0)
                apollo::push(L, self.mouseWheel.y);
            else if (std::strcmp(prop, "delta") == 0)
                apollo::push(L, self.mouseWheel.delta);
            else
                lua_pushnil(L);
        break;

        case sf::Event::JoystickMoved: // joystickMove
            if (std::strcmp(prop, "axis") == 0)
                apollo::push(L, self.joystickMove.axis);
            else if (std::strcmp(prop, "position") == 0)
                apollo::push(L, self.joystickMove.position);
            else if (std::strcmp(prop, "joystick_id") == 0)
                apollo::push(L, self.joystickMove.joystickId);
            else
                lua_pushnil(L);
        break;

        case sf::Event::JoystickButtonPressed:  // joystickButton
        case sf::Event::JoystickButtonReleased: //
            if (std::strcmp(prop, "button") == 0)
                apollo::push(L, self.joystickButton.button);
            else if (std::strcmp(prop, "position") == 0)
                apollo::push(L, self.joystickMove.position);
            else if (std::strcmp(prop, "joystick_id") == 0)
                apollo::push(L, self.joystickButton.joystickId);
            else
                lua_pushnil(L);
        break;

        case sf::Event::JoystickConnected:    // joystickConnect
        case sf::Event::JoystickDisconnected: //
            if (std::strcmp(prop, "joystick_id") == 0)
                apollo::push(L, self.joystickConnect.joystickId);
            else
                lua_pushnil(L);
        break;

        case sf::Event::TouchBegan: // touch
        case sf::Event::TouchMoved: //
        case sf::Event::TouchEnded: //
            if (std::strcmp(prop, "finger") == 0)
                apollo::push(L, self.touch.finger);
            else if (std::strcmp(prop, "x") == 0)
                apollo::push(L, self.touch.x);
            else if (std::strcmp(prop, "y") == 0)
                apollo::push(L, self.touch.y);
            else
                lua_pushnil(L);
        break;

        case sf::Event::SensorChanged: // sensor
            if (std::strcmp(prop, "type") == 0)
                apollo::push(L, self.sensor.type);
            else if (std::strcmp(prop, "x") == 0)
                apollo::push(L, self.sensor.x);
            else if (std::strcmp(prop, "y") == 0)
                apollo::push(L, self.sensor.y);
            else if (std::strcmp(prop, "z"))
                apollo::push(L, self.sensor.z);
            else
                lua_pushnil(L);
        break;

        default:
            lua_pushnil(L);
    }
    return 1;
}

int mouse_position(lua_State* L)
{
    int const n_args = lua_gettop(L);
    if (n_args == 0) {
        apollo::push(L, sf::Mouse::getPosition());
    } else if (n_args == 1) {
        apollo::push(L, sf::Mouse::getPosition(
            apollo::to<sf::Window const&>(L, 1).get()));
    } else {
        throw std::invalid_argument("Bad argument count for mouse.position.");
    }
    return 1;
}

int mouse_set_position(lua_State* L)
{
    int const n_args = lua_gettop(L);
    auto p = apollo::to<sf::Vector2i>(L, 1);
    if (n_args == 1) {
        sf::Mouse::setPosition(p);
    } else if (n_args == 2) {
        sf::Mouse::setPosition(p,
            apollo::to<sf::Window const&>(L, 1).get());
    } else {
        throw std::invalid_argument("Bad argument count for mouse.set_position.");
    }
    return 0;
}

int touch_position(lua_State* L)
{
    int const n_args = lua_gettop(L);
    auto finger = apollo::to<unsigned>(L, 1);
    if (n_args == 1) {
        apollo::push(L, sf::Touch::getPosition(finger));
    } else if (n_args == 2) {
        apollo::push(L, sf::Touch::getPosition(finger,
            apollo::to<sf::Window const&>(L, 2).get()));
    } else {
        throw std::invalid_argument("Bad argument count for touch.position.");
    }
    return 1;
}

}

LSFML_MODULE(lsfml_window)
{
    apollo::new_table(L) // VideoMode
        ("new", apollo::raw_function::caught<&new_videomode>())
        ("desktop_mode", FN(sf::VideoMode::getDesktopMode))
        ("fullscreen_mode_count", FN(n_fullscreen_modes))
        ("fullscreen_mode", FN(get_fullscreen_mode))
        ("is_valid", FN(sf::VideoMode::isValid));

    apollo::stack_reference video_mode_members(L, -1);

#define OP(n) \
    ("__" #n, FN(apollo::op::##n<sf::VideoMode const&, sf::VideoMode const&>))

    apollo::export_class<sf::VideoMode>(L)
        ("__index", video_mode_members)
        ("__tostring", FN(video_mode_to_str))
        OP(eq)
        OP(lt)
        OP(le);
#undef OP

#define PROP(ln, cn) \
    (ln, FN(APOLLO_MEMBER_GETTER(sf::ContextSettings::cn))) \
    ("set_" ln, FN(APOLLO_MEMBER_SETTER(sf::ContextSettings::cn)))

    apollo::new_table(L) // ContextSettings
        ("new", FN(apollo::ctor_wrapper<sf::ContextSettings>))
        PROP("depth_bits", depthBits)
        PROP("stencil_bits", stencilBits)
        PROP("antialiasing_level", antialiasingLevel)
        PROP("major_version", majorVersion)
        PROP("minor_version", minorVersion);
#undef PROP

    apollo::stack_reference context_settings_members(L, -1);

    apollo::export_class<sf::ContextSettings>(L)
        ("__index", context_settings_members);


    #define PROP(ln, cn) \
        (ln, FN(APOLLO_MEMBER_GETTER(sf::Joystick::Identification::cn)))

    apollo::new_table(L) // joystick.Identification
        PROP("name", name)
        PROP("vendor_id", vendorId)
        PROP("product_id", productId);
#undef PROP

    apollo::stack_reference joystick_identification_members(L, -1);

    apollo::export_class<sf::Joystick::Identification>(L)
        ("__index", context_settings_members);

    apollo::new_table(L) // Event
        ("CLOSED", sf::Event::Closed)
        ("RESIZED", sf::Event::Resized)
        ("LOST_FOCUS", sf::Event::LostFocus)
        ("GAINED_FOCUS", sf::Event::GainedFocus)
        ("TEXT_ENTERED", sf::Event::TextEntered)
        ("KEY_PRESSED", sf::Event::KeyPressed)
        ("KEY_RELEASED", sf::Event::KeyReleased)
        ("MOUSE_WHEEL_MOVED", sf::Event::MouseWheelMoved)
        ("MOUSE_BUTTON_PRESSED", sf::Event::MouseButtonPressed)
        ("MOUSE_BUTTON_RELEASED", sf::Event::MouseButtonReleased)
        ("MOUSE_MOVED", sf::Event::MouseMoved)
        ("MOUSE_ENTERED", sf::Event::MouseEntered)
        ("MOUSE_LEFT", sf::Event::MouseLeft)
        ("JOYSTICK_BUTTON_PRESSED", sf::Event::JoystickButtonPressed)
        ("JOYSTICK_BUTTON_RELEASED", sf::Event::JoystickButtonReleased)
        ("JOYSTICK_MOVED", sf::Event::JoystickMoved)
        ("JOYSTICK_CONNECTED", sf::Event::JoystickConnected)
        ("JOYSTICK_DISCONNECTED", sf::Event::JoystickDisconnected)
        ("TOUCH_BEGAN", sf::Event::TouchBegan)
        ("TOUCH_MOVED", sf::Event::TouchMoved)
        ("TOUCH_ENDED", sf::Event::TouchEnded)
        ("SENSOR_CHANGED", sf::Event::SensorChanged)
        ("COUNT", sf::Event::Count)
        ("new", FN(apollo::ctor_wrapper<sf::Event>));

    apollo::stack_reference event_members(L, -1);

    apollo::export_class<sf::Event>(L)
        ("__index", apollo::raw_function::caught<index_event>());



    apollo::new_table(L) // Window
        ("new", apollo::raw_function::caught<&window_new<sf::Window>>())
        ("create", apollo::raw_function::caught<&create_window>())
        ("close", FN(sf::Window::close))
        ("is_open", FN(sf::Window::isOpen))
        ("settings", FN(sf::Window::getSettings))
        ("poll_event", FN(sf::Window::pollEvent))
        ("wait_event", FN(sf::Window::waitEvent))
        ("position", FN(sf::Window::getPosition))
        ("set_position", FN(sf::Window::setPosition))
        ("size", FN(sf::Window::getSize))
        ("set_size", FN(sf::Window::setSize))
        ("set_title", FN(sf::Window::setTitle))
        ("set_vertical_sync_enabled", FN(sf::Window::setVerticalSyncEnabled))
        ("set_mouse_cursor_visible", FN(sf::Window::setMouseCursorVisible))
        ("set_key_repeat_enabled", FN(sf::Window::setKeyRepeatEnabled))
        ("set_frame_limit", FN(sf::Window::setFramerateLimit))
        ("set_joystick_threshold", FN(sf::Window::setJoystickThreshold))
        ("set_active", FN(sf::Window::setActive)) // Missing default argument.
        ("request_focus", FN(sf::Window::requestFocus))
        ("has_focus", FN(sf::Window::hasFocus))
        ("display", FN(sf::Window::display));
        // TODO: setIcon
        // Intentionally left out: getSystemHandle().

    apollo::stack_reference window_members(L, -1);

    apollo::export_class<sf::Window>(L)
        ("__index", window_members);


    apollo::new_table(L) // Context
        ("new", apollo::get_raw_emplace_ctor_wrapper<sf::Context>())
        ("set_active", FN(sf::Context::setActive));

    apollo::stack_reference context_members(L, -1);

    apollo::export_class<sf::Context>(L)
        ("__index", context_members);


    apollo::new_table(L) // Window module
        ("VideoMode", video_mode_members)
        ("Window", window_members)
        ("Context", context_members)
        ("Event", event_members)
        .subtable("keyboard")
            ("is_key_pressed", FN(sf::Keyboard::isKeyPressed))
            ("set_virtual_keyboard_visible",
                FN(sf::Keyboard::setVirtualKeyboardVisible))
#define KEY(n, v) (n, sf::Keyboard::v)
#include "sfKeyCodes.hpp"
        .end_subtable()
        .subtable("mouse")
            ("is_button_pressed", FN(sf::Mouse::isButtonPressed))
            ("position", apollo::raw_function::caught<&mouse_position>())
            ("set_position",
                apollo::raw_function::caught<&mouse_set_position>())
            ("LEFT", sf::Mouse::Left)
            ("RIGHT", sf::Mouse::Right)
            ("MIDDLE", sf::Mouse::Middle)
            ("XBUTTON1", sf::Mouse::XButton1)
            ("XBUTTON2", sf::Mouse::XButton2)
            ("BUTTON_COUNT", sf::Mouse::ButtonCount)
        .end_subtable()
        .subtable("joystick")
            ("is_connected", FN(sf::Joystick::isConnected))
            ("button_count", FN(sf::Joystick::getButtonCount))
            ("has_axis", FN(sf::Joystick::hasAxis))
            ("is_button_pressed", FN(sf::Joystick::isButtonPressed))
            ("axis_position", FN(sf::Joystick::getAxisPosition))
            ("identification", FN(sf::Joystick::getIdentification))
            ("update", FN(sf::Joystick::update))
            ("COUNT", sf::Joystick::Count)
            ("BUTTON_COUNT", sf::Joystick::ButtonCount)
            ("AXIS_COUNT", sf::Joystick::AxisCount)
            ("X", sf::Joystick::X)
            ("Y", sf::Joystick::Y)
            ("Z", sf::Joystick::Z)
            ("R", sf::Joystick::R)
            ("U", sf::Joystick::U)
            ("V", sf::Joystick::V)
            ("POVX", sf::Joystick::PovX)
            ("POVY", sf::Joystick::PovY)
        .end_subtable()
        .subtable("sensor")
            ("is_available", FN(sf::Sensor::isAvailable))
            ("set_enabled", FN(sf::Sensor::setEnabled))
            ("value", FN(sf::Sensor::getValue))
            ("ACCELEROMETER", sf::Sensor::Accelerometer)
            ("GYROSCOPE", sf::Sensor::Gyroscope)
            ("MAGNETOMETER", sf::Sensor::Magnetometer)
            ("GRAVITY", sf::Sensor::Gravity)
            ("USER_ACCELERATION", sf::Sensor::UserAcceleration)
            ("ORIENTATION", sf::Sensor::Orientation)
            ("COUNT", sf::Sensor::Count)
        .end_subtable()
        .subtable("touch")
            ("is_down", FN(sf::Touch::isDown))
            ("position", apollo::raw_function::caught<&touch_position>())
        .end_subtable()
        .subtable("style")
            ("NONE", sf::Style::None)
            ("TITLEBAR", sf::Style::Titlebar)
            ("RESIZE", sf::Style::Resize)
            ("CLOSE", sf::Style::Close)
            ("FULLSCREEN", sf::Style::Fullscreen)
            ("DEFAULT", sf::Style::Default)
        .end_subtable();
    return 1;
}
