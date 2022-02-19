#pragma once
#include <core/joystick.hpp>
#include <vector>

struct PlayerControllerComponent
{
    int player_number;
    std::vector<Joystick::Joystick_Buttons> buttons_just_pressed;
    std::vector<Joystick::Joystick_Buttons> buttons_just_released;
    std::vector<Joystick::Joystick_Buttons> buttons_pressed;
};