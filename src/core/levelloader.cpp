#include <core/levelloader.hpp>
#include <core/world.hpp>
#include <utilities/lualoader.hpp>
#include <components/transform_component.hpp>
#include <components/sprite_component.hpp>
#include <components/rigid_body_component.hpp>
#include <components/box_collider_component.hpp>
#include <fstream>
#include <string>

LevelLoader::LevelLoader()
{
}

LevelLoader::~LevelLoader()
{
}

void LevelLoader::LoadTiledLevel(sol::state &lua, entt::registry &registry, const std::unique_ptr<AssetStore> &assetStore, SDL_Renderer *renderer, int level)
{
    //Temporary level to load.
    auto filename = "./assets/tiled/debug.lua";

    //TODO move the sol validation to its own like we have for the loader.
    //Test if the lua file is valid before loading.
    sol::load_result script = lua.load_file(filename);
    if (!script.valid())
    {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }

    //Load the lua file as a function.
    auto loadedLua = lua.load_file(filename);
    //TODO test if it works like above, probably make it into a helper function.
    sol::table luaTable = loadedLua();
    //Get information about the map and tile size.
    ushort mapNumRows = luaTable["width"];
    ushort tileWidth = luaTable["tilewidth"];
    ushort tileHeight = luaTable["tileheight"];

    //Get information about the tileset so we know where to draw from.
    std::string tilesetName = luaTable["tilesets"][1]["name"];
    std::string loadTilesetName = "./assets/tilemaps/" + tilesetName + ".png";
    assetStore->AddTexture(renderer, tilesetName, "./assets/tilemaps/" + tilesetName + ".png");
    auto texture = assetStore->GetTexture(tilesetName);
    //SDL is based on C, so we need to pass references (memory address) to the int for it to modify, so we need to make them first.
    int texX, texY = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &texX, &texY);
    int tileMapNumRows = texX / tileWidth;
    //Load all the layers in the tiled map. Lua starts at index 1, so we need a variable to represent it.
    sol::table layersTable = luaTable["layers"];
    int iLayer = 1;
    while (true)
    {
        //We don't know how big the table is in lua, so test to see if the layer exists, if not, break from the while loop.
        sol::optional<sol::table> hasLayer = layersTable[iLayer];
        if (hasLayer == sol::nullopt)
            break;
        sol::table layer = layersTable[iLayer];
        std::string layerName = layer["name"];
        bool visible = layer["visible"];
        sol::table layerData = layer["data"];
        //Same as above, we need to loop through all the tiles in the layer, and lua starts at index 1.
        int jTileNum = 1;
        while (true)
        {
            sol::optional<int> hasTile = layerData[jTileNum];
            if (hasTile == sol::nullopt)
                break;
            int tileNumber = layerData[jTileNum];
            //tile 0 in tiled is blank
            if (tileNumber == 0)
            {
                jTileNum++;
                continue;
            }
            //Calculate where the tile should go, and where it is on the spritesheet.
            int loc = jTileNum - 1;
            int column = loc != 0 ? (loc / mapNumRows) : 0;
            int row = loc != 0 ? loc % mapNumRows : 0;
            int y = column * tileHeight;
            int x = (row)*tileWidth;
            //We need to subtract 1, as tiled uses 0 as unused, and 0 on our tileset is 0,0 not 0,16 so we need to shift it left on our tileset by 1
            int numberInTileset = tileNumber - 1;
            int srcY = (numberInTileset / tileMapNumRows) * tileWidth;
            int srcX = (numberInTileset % tileMapNumRows) * tileHeight;
            const auto tile = registry.create();
            if (visible)
            {
                registry.emplace<SpriteComponent>(tile, tilesetName, tileWidth, tileHeight, iLayer, false, srcX, srcY);
            }
            if (layerName == "Obstacles")
            {
                SDL_Rect rect;
                rect.x = x;
                rect.y = y;
                rect.w = tileWidth;
                rect.h = tileHeight;
                registry.emplace<BoxColliderComponent>(tile, rect);
            }
            registry.emplace<TransformComponent>(tile, glm::vec2(x, y), glm::vec2(1.0, 1.0), 0.0);

            jTileNum++;
        }
        iLayer++;
    }
    const auto player = registry.create();

    registry.emplace<TransformComponent>(player, glm::vec2(40, 40), glm::vec2(1.0, 1.0), 0);
    registry.emplace<SpriteComponent>(player, tilesetName, tileWidth, tileHeight, 5, false, 0, 64);
    registry.emplace<RigidBodyComponent>(player, glm::vec2(150,-30));
    SDL_Rect player_rect;
    player_rect.h = 12;
    player_rect.w = 12;
    player_rect.x = 16;
    player_rect.y = 16;
    registry.emplace<BoxColliderComponent>(player, player_rect, glm::vec2(2,2));


    //Sort the registry so that it is ordered by z for the sprite components.  This should be done whenever something is added.
    registry.sort<SpriteComponent>([](const auto &lhs, const auto &rhs)
                                   { return lhs.zIndex < rhs.zIndex; });
}