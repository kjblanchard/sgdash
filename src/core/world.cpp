#include <iostream>
#include <stdexcept>

#include <core/world.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <imgui/imgui_impl_sdl.h>

#include <systems/draw_system.hpp>
#include <systems/sound_system.hpp>
#include <systems/movement_system.hpp>
#include <systems/gravity_system.hpp>
#include <systems/collision_system.hpp>

#include <core/logger.hpp>
#include <core/levelloader.hpp>
#include <utilities/lualoader.hpp>

int World::windowWidth;
int World::windowHeight;
int World::mapWidth;
int World::mapHeight;
int World::unscaledHeight;
int World::unscaledWidth;
int World::screenScaleRatioHeight;
int World::screenScaleRatioWidth;
bool World::isDebug = false;

World::World() : isRunning{false}, assetStore{std::make_unique<AssetStore>()}, eventBus{std::make_unique<EventBus>()}
{
}

World::~World()
{
}

void World::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(SDL_GetError());
    if (TTF_Init() != 0)
        throw std::runtime_error(TTF_GetError());

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    sol::state tempLua;

    auto window_config = utilities::load_lua_table(tempLua, "cfg.lua", "window_config");
    windowWidth = window_config[windowWidthStr];
    windowHeight = window_config[windowHeightStr];
    unscaledWidth = window_config[unscaledWidthStr];
    unscaledHeight = window_config[unscaledHeightStr];
    screenScaleRatioWidth = windowWidth / unscaledWidth;
    screenScaleRatioHeight = windowWidth / unscaledWidth;

    auto debug_config = utilities::load_lua_table(tempLua, "cfg.lua", "debug_config");
    World::isDebug = debug_config["all"];

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS);
    if (!window)
        throw std::runtime_error(SDL_GetError());
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        throw std::runtime_error(SDL_GetError());

    // Initialize the camera view with the entire screen area
    camera.x = 0;
    camera.y = 0;
    camera.w = unscaledWidth;
    camera.h = unscaledHeight;
    SDL_RenderSetLogicalSize(renderer, unscaledWidth, unscaledHeight);

    SDL_RenderSetVSync(renderer, 1);

    // Setup Platform/Renderer backends
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    isRunning = true;
}

void World::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        //Handle imgui with the scaled screen.
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX / screenScaleRatioWidth, mouseY / screenScaleRatioHeight);

        // Handle core SDL events (close window, key pressed, etc.)
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            if (sdlEvent.key.keysym.sym == SDLK_d)
            {
                isDebug = !isDebug;
            }
            // eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
            break;
        }
    }
}

void World::Setup()
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);

    SoundSystem::Setup(lua);
    GravitySystem::Setup();

    auto levelloader = LevelLoader();
    levelloader.LoadTiledLevel(lua, reg, assetStore, renderer, 1);
}

void World::Update()
{
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
        SDL_Delay(timeToWait);
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks();
    eventBus->Reset();
    SoundSystem::Update();

    MovementSystem::Update(reg, deltaTime);
    GravitySystem::Update(reg, deltaTime);
}

void World::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    DrawSystem::Update(reg, renderer, assetStore, camera);
    if (isDebug)
    {
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }
    SDL_RenderPresent(renderer);
}

void World::Run()
{
    Setup();
    while (isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void World::Destroy()
{
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
