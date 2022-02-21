#pragma once

#include <SDL2/SDL.h>
#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <events/EventBus.h>
#include <core/asset_store.hpp>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class World
{
private:
    bool isRunning;
    int millisecsPreviousFrame = 0;
    SDL_Window *window;
    // SDL_Renderer *renderer;
    SDL_Rect camera;

    sol::state lua;
    // entt::registry reg;
    // std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;

    const char *windowWidthStr = "window_width";
    const char *windowHeightStr = "window_height";
    const char *unscaledWidthStr = "unscaled_width";
    const char *unscaledHeightStr = "unscaled_height";


public:
    World();
    ~World();
    void Initialize();
    void Run();
    void Setup();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
    static SDL_Renderer *renderer;
    static std::unique_ptr<AssetStore> assetStore;

    static int windowWidth;
    static int windowHeight;
    static int unscaledHeight;
    static int unscaledWidth;
    static int mapWidth;
    static int mapHeight;
    static int screenScaleRatioWidth;
    static int screenScaleRatioHeight;
    static bool isDebug;
    static constexpr char *config_file_string = (char*)"cfg.lua";
};