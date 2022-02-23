#include <string>
#include <iostream>

#include <systems/player_controller_system.hpp>
#include <utilities/lualoader.hpp>

//statics
Uint8 PlayerControllerSystem::previous_keyboard_state[SDL_NUM_SCANCODES];
Uint8 PlayerControllerSystem::current_keyboard_state[SDL_NUM_SCANCODES];
std::map<Joystick::Joystick_Buttons, SDL_Scancode> PlayerControllerSystem::player_button_mapping;

void PlayerControllerSystem::setup()
{
    InitializeKeyboardStateArrays();
    create_player_key_mappings();
}

void PlayerControllerSystem::InitializeKeyboardStateArrays()
{
    memset(previous_keyboard_state, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(current_keyboard_state, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * SDL_NUM_SCANCODES);
}
void PlayerControllerSystem::update_keyboard_states()
{
    memcpy(previous_keyboard_state, current_keyboard_state, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(current_keyboard_state, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}
bool PlayerControllerSystem::IsKeyHeldDown(SDL_Scancode key)
{
    return previous_keyboard_state[key] && current_keyboard_state[key];
}

bool PlayerControllerSystem::KeyJustPressed(const SDL_Scancode key)
{
    return current_keyboard_state[key] && !previous_keyboard_state[key];
}

bool PlayerControllerSystem::KeyJustReleased(const SDL_Scancode key)
{
    return !current_keyboard_state[key] && previous_keyboard_state[key];
}

void PlayerControllerSystem::create_player_key_mappings()
{
    sol::state temp;
    auto loaded_lua_controller_table = utilities::load_lua_table(temp, "cfg.lua", "controller_config");

    auto up_button_joystick_key = Joystick::get_button_from_lua_string(Joystick::UP_STRING);
    auto up_button_sdl_key = Joystick::get_sdl_key_from_string(loaded_lua_controller_table[Joystick::UP_STRING]);
    player_button_mapping[up_button_joystick_key] = up_button_sdl_key;

    auto confirm_button_key = Joystick::get_button_from_lua_string(Joystick::CONFIRM_STRING);
    auto confirm_button_sdl_key = Joystick::get_sdl_key_from_string(loaded_lua_controller_table[Joystick::CONFIRM_STRING]);
    player_button_mapping[confirm_button_key] = confirm_button_sdl_key;
}

void PlayerControllerSystem::update()
{
    update_keyboard_states();
}
SDL_Scancode PlayerControllerSystem::player_button_lookup(Joystick::Joystick_Buttons button)
{
    auto but = player_button_mapping[button];

    return player_button_mapping[button];
    // return player_button_mapping.at(button);
}