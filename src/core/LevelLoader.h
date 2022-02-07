#pragma once
#include <core/asset_store.hpp>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>
#include <entt/entt.hpp>

#include <memory>

class LevelLoader
{
public:
    LevelLoader();
    ~LevelLoader();

    //Old implementation
    // void LoadLevel(sol::state &lua, const std::unique_ptr<Registry> &registry, const std::unique_ptr<AssetStore> &assetStore, SDL_Renderer *renderer, int level);

    //kjb inplementation
    // void LoadTiledLevel(sol::state& lua, const std::unique_ptr<Registry> &registry, const std::unique_ptr<AssetStore> &assetStore, SDL_Renderer *renderer, int level);
    void LoadTiledLevel(sol::state& lua, entt::registry &registry, const std::unique_ptr<AssetStore> &assetStore, SDL_Renderer *renderer, int level);
};