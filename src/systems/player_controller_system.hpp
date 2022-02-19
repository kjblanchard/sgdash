#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <core/joystick.hpp>
class PlayerControllerSystem
{
public:
    static bool IsKeyHeldDown(SDL_Scancode key);
    static bool KeyJustPressed(SDL_Scancode key);
    static bool KeyJustReleased(SDL_Scancode key);

    static void setup();
    static void create_player_key_mappings();
    static std::map<Joystick::Joystick_Buttons,SDL_Scancode> player_button_mapping;
    static void update();
    static void update_keyboard_states();

    static SDL_Scancode player_button_lookup(Joystick::Joystick_Buttons button);

    static Uint8 previous_keyboard_state[SDL_NUM_SCANCODES];
    static Uint8 current_keyboard_state[SDL_NUM_SCANCODES];
    static void InitializeKeyboardStateArrays();
};