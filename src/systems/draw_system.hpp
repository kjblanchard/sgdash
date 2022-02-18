#pragma once

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

class AssetStore;

class DrawSystem
{
public:
    static void Update(entt::registry &reg, SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera);
};