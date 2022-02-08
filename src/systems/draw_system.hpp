#pragma once

#include <algorithm>
#include <components/transform_component.hpp>
#include <components/sprite_component.hpp>
#include <core/asset_store.hpp>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

class DrawSystem
{
public:
    static void Update(entt::registry &reg, SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera);
};