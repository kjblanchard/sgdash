#include <core/logger.hpp>
#include <core/levelloader.hpp>

//lib resources.
#include <core/world.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

//Systems
#include <systems/draw_system.hpp>
#include <systems/sound_system.hpp>
#include <systems/movement_system.hpp>
#include <systems/gravity_system.hpp>
#include <systems/collision_system.hpp>

#include <iostream>

#include <utilities/lualoader.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <imgui/imgui_impl_sdl.h>

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

    // Initialize the camera view with the entire screen area
    camera.x = 0;
    camera.y = 0;
    camera.w = unscaledWidth;
    camera.h = unscaledHeight;
    SDL_RenderSetLogicalSize(renderer, unscaledWidth, unscaledHeight);

    isRunning = true;
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    SDL_RenderSetVSync(renderer,1);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
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

    auto levelloader = LevelLoader();
    levelloader.LoadTiledLevel(lua, reg, assetStore, renderer, 1);
}

void World::Update()
{
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
    {
        SDL_Delay(5);
    }
    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks();
    eventBus->Reset();
    SoundSystem::Update();
    GravitySystem::Update(reg, deltaTime);
    MovementSystem::Update(reg,deltaTime);
    CollisionSystem::Update(reg,deltaTime);


}

static int counter = 0;
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

        ImGui::ShowDemoWindow();
        {
            static float f = 0.0f;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

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
