#include <ecs/ECS.h>
#include <essential/logger.hpp>
#include <essential/LevelLoader.h>

//lib resources.
#include <essential/world.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

#include <systems/RenderSystem.h>
#include <systems/sound_system.hpp>
#include <iostream>

#include <utilities/lualoader.hpp>

// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_sdlrenderer.h>
// #include <imgui/imgui_impl_sdl.h>

int World::windowWidth;
int World::windowHeight;
int World::mapWidth;
int World::mapHeight;
int World::unscaledHeight;
int World::unscaledWidth;
int World::screenScaleRatioHeight;
int World::screenScaleRatioWidth;

World::World()
{
    isRunning = false;
    isDebug = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
    Logger::Log("World constructor called!");
}

World::~World()
{
    Logger::Log("World destructor called!");
}

void World::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    // if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Err("Error initializing SDL.");
        return;
    }
    if (TTF_Init() != 0)
    {
        Logger::Err("Error initializing SDL TTF.");
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    // windowWidth = 800;
    // windowHeight = 600;
    sol::state tempLua;
    auto config = utilities::load_lua_table(tempLua, "cfg.lua", "window_config");
    windowWidth = config[windowWidthStr];
    windowHeight = config[windowHeightStr];
    unscaledWidth = config[unscaledWidthStr];
    unscaledHeight = config[unscaledHeightStr];
    screenScaleRatioWidth = windowWidth / unscaledWidth;
    screenScaleRatioHeight = windowWidth / unscaledWidth;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        Logger::Err("Error creating SDL window.");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    // Initialize the ImGui context
    // ImGui::CreateContext();

    // ImGui_ImplSDL2_InitForSDLRenderer(window);
    // ImGui_ImplSDLRenderer_Init(renderer);
    // ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

    // Initialize the camera view with the entire screen area
    camera.x = 0;
    camera.y = 0;
    camera.w = unscaledWidth;
    camera.h = unscaledHeight;
    SDL_RenderSetLogicalSize(renderer, unscaledWidth, unscaledHeight);

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}

void World::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        // ImGui SDL input
        // ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        // ImGuiIO &io = ImGui::GetIO();
        // int mouseX, mouseY;
        // const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        // io.MousePos = ImVec2(mouseX / screenScaleRatioWidth, mouseY / screenScaleRatioHeight);
        // io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        // io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

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

    registry->AddSystem<RenderSystem>();
    registry->AddSystem<SoundSystem>();
    registry->GetSystem<SoundSystem>().Setup();

    auto levelloader = LevelLoader();
    levelloader.LoadTiledLevel(lua,registry,assetStore,renderer,1);

}

void World::Update()
{
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
    {
        SDL_Delay(timeToWait);
    }
    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks();
    eventBus->Reset();
    registry->Update();

    registry->GetSystem<SoundSystem>().Update();
}

void World::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
    // if (isDebug)
    // {
    //     registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
    //     registry->GetSystem<RenderGuiSystem>().Update(registry);

    // }

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
    // ImGui_ImplSDLRenderer_Shutdown();
    // ImGui_ImplSDL2_Shutdown();
    // ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
