#pragma once
#include <entt/entt.hpp>
#include <SDL2/SDL_rect.h>

class CameraSystem
{
public:
    CameraSystem() {}
    static void update(entt::registry& reg, SDL_Rect& camera );
};