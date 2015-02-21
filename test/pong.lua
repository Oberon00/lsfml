local sys = require 'lsfml.system'
local win = require 'lsfml.window'
local gfx = require 'lsfml.graphics'
local sfx = require 'lsfml.audio'

math.randomseed(os.time())

-- Define some constants
local GAME_W = 800
local GAME_H = 600
local PADDLE_SZ = sys.Vec2.new(25, 100)
local BALL_R = 10

-- Create the wnd of the application
local wnd = gfx.RenderWindow.new(win.VideoMode.new(GAME_W, GAME_H, 32),
    "LSFML Pong", win.style.TITLEBAR + win.style.CLOSE)
wnd:set_vertical_sync_enabled(true)

-- Load the sounds used in the game
local ball_sound_buf = sfx.SoundBuffer.new()
assert(ball_sound_buf:load_from_file("resources/ball.wav"))
local ball_sound = sfx.Sound.new(ball_sound_buf)

-- Create the left paddle
local l_paddle = gfx.RectangleShape.new(PADDLE_SZ - sys.Vec2.new(3, 3))
l_paddle:set_outline_thickness(3)
l_paddle:set_outline_color(gfx.Color.BLACK)
l_paddle:set_fill_color(gfx.Color.new(100, 100, 200))
l_paddle:set_origin(PADDLE_SZ / 2)

-- Create the right paddle
local r_paddle = gfx.RectangleShape.new(PADDLE_SZ - sys.Vec2.new(3, 3))
r_paddle:set_outline_thickness(3)
r_paddle:set_outline_color(gfx.Color.BLACK)
r_paddle:set_fill_color(gfx.Color.new(200, 100, 100))
r_paddle:set_origin(PADDLE_SZ / 2)

-- Create the ball
local ball = gfx.CircleShape.new()
ball:set_radius(BALL_R - 3)
ball:set_outline_thickness(3)
ball:set_outline_color(gfx.Color.BLACK)
ball:set_fill_color(gfx.Color.WHITE)
ball:set_origin(BALL_R / 2, BALL_R / 2)

-- Load the text font
local font = gfx.Font.new()
assert(font:load_from_file("resources/sansation.ttf"))

-- Initialize the pause message
local pause_msg = gfx.Text.new()
pause_msg:set_font(font)
pause_msg:set_character_size(40)
pause_msg:set_position(170, 150)
pause_msg:set_color(gfx.Color.WHITE)
pause_msg:set_string("Welcome to LSFML pong!\nPress space to start the game")


-- Define the paddle's properties
local ai_timer = sys.Clock.new()
local AI_TIME = sys.seconds(0.1)
local PADDLE_SPEED = 400
local r_paddle_speed = 0
local BALL_SPEED = 400
local ball_angle = 0

local clock = sys.Clock.new()
local is_playing = false
local evt = win.Event.new()
local KEY_PRESSED = win.Event.KEY_PRESSED
local CLEAR_COLOR = gfx.Color.new(50, 200, 50)

while wnd:is_open() do

    while wnd:poll_event(evt) do -- Handle events

        -- Window closed or escape key pressed: exit
        if evt.type == win.Event.CLOSED
            or evt.type == KEY_PRESSED and evt.code == win.keyboard.ESCAPE
        then
            wnd:close()
            break
        end

        -- Space key pressed: play
        if evt.type == KEY_PRESSED and evt.code == win.keyboard.SPACE then
            if not is_playing then
                -- (re)start the game
                is_playing = true
                clock:restart()

                -- Reset the position of the paddles and ball
                l_paddle:set_position(10 + PADDLE_SZ:x() / 2, GAME_H / 2)
                r_paddle:set_position(GAME_W - 10 - PADDLE_SZ:x() / 2, GAME_H / 2)
                ball:set_position(GAME_W / 2, GAME_H / 2)

                -- Reset the ball angle
                repeat
                    -- Make sure the ball initial angle is not too much vertical
                    ball_angle = math.random() * 360 * 2 * math.pi / 360
                until math.abs(math.cos(ball_angle)) >= 0.7
            end
        end

    end

    if is_playing then
        local dt = clock:restart():as_seconds()

        -- Move the player's paddle
        if win.keyboard.is_key_pressed(win.keyboard.UP)
            and l_paddle:position():y() - PADDLE_SZ:y() / 2 > 5
        then
            l_paddle:move(0, -PADDLE_SPEED * dt)
        end
        if win.keyboard.is_key_pressed(win.keyboard.DOWN)
            and l_paddle:position():y() + PADDLE_SZ:y() / 2 < GAME_H - 5
        then
            l_paddle:move(0, PADDLE_SPEED * dt)
        end

        -- Move the computer's paddle
        if (r_paddle_speed < 0 and r_paddle:position():y() - PADDLE_SZ:y() / 2 > 5) or
            (r_paddle_speed > 0 and r_paddle:position():y() + PADDLE_SZ:y() / 2 < GAME_H - 5)
        then
            r_paddle:move(0, r_paddle_speed * dt)
        end

        -- Update the computer's paddle direction according to the ball position
        if ai_timer:elapsed_time() > AI_TIME then
            ai_timer:restart()
            if ball:position():y() + BALL_R > r_paddle:position():y() + PADDLE_SZ:y() / 2 then
                r_paddle_speed = PADDLE_SPEED
            elseif ball:position():y() - BALL_R < r_paddle:position():y() - PADDLE_SZ:y() / 2 then
                r_paddle_speed = -PADDLE_SPEED
            else
                r_paddle_speed = 0
            end
        end

        -- Move the ball
        local factor = BALL_SPEED * dt
        ball:move(math.cos(ball_angle) * factor, math.sin(ball_angle) * factor)

        -- Check collisions between the ball and the screen
        if ball:position():x() - BALL_R < 0 then
            is_playing = false
            pause_msg:set_string("You lost!\nPress space to restart or\nescape to exit")
        end
        if ball:position():x() + BALL_R > GAME_W then
            is_playing = false
            pause_msg:set_string("You won!\nPress space to restart or\nescape to exit")
        end
        if ball:position():y() - BALL_R < 0 then
            ball_sound:play()
            ball_angle = -ball_angle
            ball:set_position(ball:position():x(), BALL_R + 0.1)
        end
        if ball:position():y() + BALL_R > GAME_H then
            ball_sound:play()
            ball_angle = -ball_angle
            ball:set_position(ball:position():x(), GAME_H - BALL_R - 0.1)
        end

        -- Check the collisions between the ball and the paddles
        -- Left Paddle
        if ball:position():x() - BALL_R < l_paddle:position():x() + PADDLE_SZ:x() / 2 and 
            ball:position():x() - BALL_R > l_paddle:position():x() and
            ball:position():y() + BALL_R >= l_paddle:position():y() - PADDLE_SZ:y() / 2 and
            ball:position():y() - BALL_R <= l_paddle:position():y() + PADDLE_SZ:y() / 2
        then
            if ball:position():y() > l_paddle:position():y() then
                ball_angle = math.pi - ball_angle + math.random() * 20 * math.pi / 180
            else
                ball_angle = math.pi - ball_angle - math.random() * 20 * math.pi / 180
            end

            ball_sound:play()
            ball:set_position(l_paddle:position():x() + BALL_R + PADDLE_SZ:x() / 2 + 0.1, ball:position():y())
        end

        -- Right Paddle
        if ball:position():x() + BALL_R > r_paddle:position():x() - PADDLE_SZ:x() / 2 and
            ball:position():x() + BALL_R < r_paddle:position():x() and
            ball:position():y() + BALL_R >= r_paddle:position():y() - PADDLE_SZ:y() / 2 and
            ball:position():y() - BALL_R <= r_paddle:position():y() + PADDLE_SZ:y() / 2
        then
            if ball:position():y() > r_paddle:position():y() then
                ball_angle = math.pi - ball_angle + math.random() * 20 * math.pi / 180
            else
                ball_angle = math.pi - ball_angle - math.random() * 20 * math.pi / 180
            end

            ball_sound:play()
            ball:set_position(r_paddle:position():x() - BALL_R - PADDLE_SZ:x() / 2 - 0.1, ball:position():y())
        end
    end

    -- Clear the window
    wnd:clear(CLEAR_COLOR)

    if is_playing then
        -- Draw the paddles and the ball
        wnd:draw(l_paddle)
        wnd:draw(r_paddle)
        wnd:draw(ball)
    else
        -- Draw the pause message
        wnd:draw(pause_msg)
    end

    -- Display things on screen
    wnd:display()
end
