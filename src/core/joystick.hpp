#pragma once
#include <string>
#include <SDL2/SDL_scancode.h>
class Joystick
{
public:
    enum class Joystick_Buttons
    {
        DEFAULT,
        UP,
        RIGHT,
        DOWN,
        LEFT,
        CONFIRM,
        CANCEL,
        START,
        SELECT
    };

    static Joystick::Joystick_Buttons get_button_from_lua_string(std::string lua_joystick_string)
    {
        if (lua_joystick_string == UP_STRING)
            return Joystick_Buttons::UP;
        else
            return Joystick_Buttons::DEFAULT;
    }

    static SDL_Scancode get_sdl_key_from_string(std::string lua_joystick_string)
    {
        if (lua_joystick_string == "w")
            return SDL_SCANCODE_W;
    }

    static constexpr char *UP_STRING = (char *)"u";
    static constexpr char *RIGHT_STRING = (char *)"r";
    static constexpr char *DOWN_STRING = (char *)"d";
    static constexpr char *LEFT_STRING = (char *)"l";
    static constexpr char *CONFIRM_STRING = (char *)"confirm";
    static constexpr char *CANCEL_STRING = (char *)"cancel";
    static constexpr char *START_STRING = (char *)"start";
    static constexpr char *SELECT_STRING = (char *)"select";

private:
};