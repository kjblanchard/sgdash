#include <core/LevelLoader.h>
#include <core/world.hpp>
#include <components/TransformComponent.h>
#include <components/SpriteComponent.h>
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
    sol::load_result script = lua.load_file(filename);
    if (!script.valid())
    {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }

    //Load the lua file
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
            registry.emplace<TransformComponent>(tile, glm::vec2(x, y), glm::vec2(1.0, 1.0), 0.0);
            registry.emplace<SpriteComponent>(tile, tilesetName, tileWidth, tileHeight, iLayer, false, srcX, srcY);

            jTileNum++;
        }
        iLayer++;
    }
    registry.sort<SpriteComponent>([](const auto &lhs, const auto &rhs)
                                   { return lhs.zIndex < rhs.zIndex; });
}

// void LevelLoader::LoadLevel(sol::state &lua, const std::unique_ptr<Registry> &registry, const std::unique_ptr<AssetStore> &assetStore, SDL_Renderer *renderer, int levelNumber)
// {
//     // This checks the syntax of our script, but it does not execute the script
//     sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");
//     if (!script.valid())
//     {
//         sol::error err = script;
//         std::string errorMessage = err.what();
//         Logger::Err("Error loading the lua script: " + errorMessage);
//         return;
//     }

//     // Executes the script using the Sol state
//     lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

//     // Read the big table for the current level
//     sol::table level = lua["Level"];

//     ////////////////////////////////////////////////////////////////////////////
//     // Read the level assets
//     ////////////////////////////////////////////////////////////////////////////
//     sol::table assets = level["assets"];

//     int i = 0;
//     while (true)
//     {
//         sol::optional<sol::table> hasAsset = assets[i];
//         if (hasAsset == sol::nullopt)
//         {
//             break;
//         }
//         sol::table asset = assets[i];
//         std::string assetType = asset["type"];
//         std::string assetId = asset["id"];
//         if (assetType == "texture")
//         {
//             assetStore->AddTexture(renderer, assetId, asset["file"]);
//             Logger::Log("A new texture asset was added to the asset store, id: " + assetId);
//         }
//         if (assetType == "font")
//         {
//             assetStore->AddFont(assetId, asset["file"], asset["font_size"]);
//             Logger::Log("A new font asset was added to the asset store, id: " + assetId);
//         }
//         i++;
//     }

//     ////////////////////////////////////////////////////////////////////////////
//     // Read the level tilemap information
//     ////////////////////////////////////////////////////////////////////////////
//     sol::table map = level["tilemap"];
//     std::string mapFilePath = map["map_file"];
//     std::string mapTextureAssetId = map["texture_asset_id"];
//     int mapNumRows = map["num_rows"];
//     int mapNumCols = map["num_cols"];
//     int tileSize = map["tile_size"];
//     double mapScale = map["scale"];
//     std::fstream mapFile;
//     mapFile.open(mapFilePath);
//     for (int y = 0; y < mapNumRows; y++)
//     {
//         for (int x = 0; x < mapNumCols; x++)
//         {
//             char ch;
//             mapFile.get(ch);
//             int srcRectY = std::atoi(&ch) * tileSize;
//             mapFile.get(ch);
//             int srcRectX = std::atoi(&ch) * tileSize;
//             mapFile.ignore();

//             Entity tile = registry->CreateEntity();
//             tile.AddComponent<TransformComponent>(glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize)), glm::vec2(mapScale, mapScale), 0.0);
//             tile.AddComponent<SpriteComponent>(mapTextureAssetId, tileSize, tileSize, 0, false, srcRectX, srcRectY);
//         }
//     }
//     mapFile.close();
//     World::mapWidth = mapNumCols * tileSize * mapScale;
//     World::mapHeight = mapNumRows * tileSize * mapScale;

//     ////////////////////////////////////////////////////////////////////////////
//     // Read the level entities and their components
//     ////////////////////////////////////////////////////////////////////////////
//     sol::table entities = level["entities"];
//     i = 0;
//     while (true)
//     {
//         sol::optional<sol::table> hasEntity = entities[i];
//         if (hasEntity == sol::nullopt)
//         {
//             break;
//         }

//         sol::table entity = entities[i];

//         Entity newEntity = registry->CreateEntity();

//         // Tag
//         sol::optional<std::string> tag = entity["tag"];
//         if (tag != sol::nullopt)
//         {
//             newEntity.Tag(entity["tag"]);
//         }

//         // Group
//         sol::optional<std::string> group = entity["group"];
//         if (group != sol::nullopt)
//         {
//             newEntity.Group(entity["group"]);
//         }

//         // Components
//         sol::optional<sol::table> hasComponents = entity["components"];
//         if (hasComponents != sol::nullopt)
//         {
//             // Transform
//             sol::optional<sol::table> transform = entity["components"]["transform"];
//             if (transform != sol::nullopt)
//             {
//                 newEntity.AddComponent<TransformComponent>(
//                     glm::vec2(
//                         entity["components"]["transform"]["position"]["x"],
//                         entity["components"]["transform"]["position"]["y"]),
//                     glm::vec2(
//                         entity["components"]["transform"]["scale"]["x"].get_or(1.0),
//                         entity["components"]["transform"]["scale"]["y"].get_or(1.0)),
//                     entity["components"]["transform"]["rotation"].get_or(0.0));
//             }

//             // RigidBody
//             sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
//             if (rigidbody != sol::nullopt)
//             {
//                 newEntity.AddComponent<RigidBodyComponent>(
//                     glm::vec2(
//                         entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
//                         entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)));
//             }

//             // Sprite
//             sol::optional<sol::table> sprite = entity["components"]["sprite"];
//             if (sprite != sol::nullopt)
//             {
//                 newEntity.AddComponent<SpriteComponent>(
//                     entity["components"]["sprite"]["texture_asset_id"],
//                     entity["components"]["sprite"]["width"],
//                     entity["components"]["sprite"]["height"],
//                     entity["components"]["sprite"]["z_index"].get_or(1),
//                     entity["components"]["sprite"]["fixed"].get_or(false),
//                     entity["components"]["sprite"]["src_rect_x"].get_or(0),
//                     entity["components"]["sprite"]["src_rect_y"].get_or(0));
//             }

//             // Animation
//             sol::optional<sol::table> animation = entity["components"]["animation"];
//             if (animation != sol::nullopt)
//             {
//                 newEntity.AddComponent<AnimationComponent>(
//                     entity["components"]["animation"]["num_frames"].get_or(1),
//                     entity["components"]["animation"]["speed_rate"].get_or(1));
//             }

//             // BoxCollider
//             sol::optional<sol::table> collider = entity["components"]["boxcollider"];
//             if (collider != sol::nullopt)
//             {
//                 newEntity.AddComponent<BoxColliderComponent>(
//                     entity["components"]["boxcollider"]["width"],
//                     entity["components"]["boxcollider"]["height"],
//                     glm::vec2(
//                         entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
//                         entity["components"]["boxcollider"]["offset"]["y"].get_or(0)));
//             }

//             // Health
//             sol::optional<sol::table> health = entity["components"]["health"];
//             if (health != sol::nullopt)
//             {
//                 newEntity.AddComponent<HealthComponent>(
//                     static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100)));
//             }

//             // ProjectileEmitter
//             sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
//             if (projectileEmitter != sol::nullopt)
//             {
//                 newEntity.AddComponent<ProjectileEmitterComponent>(
//                     glm::vec2(
//                         entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
//                         entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]),
//                     static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
//                     static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
//                     static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
//                     entity["components"]["projectile_emitter"]["friendly"].get_or(false));
//             }

//             // CameraFollow
//             sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
//             if (cameraFollow != sol::nullopt)
//             {
//                 newEntity.AddComponent<CameraFollowComponent>();
//             }

//             // KeyboardControlled
//             sol::optional<sol::table> keyboardControlled = entity["components"]["keyboard_controller"];
//             if (keyboardControlled != sol::nullopt)
//             {
//                 newEntity.AddComponent<KeyboardControlledComponent>(
//                     glm::vec2(
//                         entity["components"]["keyboard_controller"]["up_velocity"]["x"],
//                         entity["components"]["keyboard_controller"]["up_velocity"]["y"]),
//                     glm::vec2(
//                         entity["components"]["keyboard_controller"]["right_velocity"]["x"],
//                         entity["components"]["keyboard_controller"]["right_velocity"]["y"]),
//                     glm::vec2(
//                         entity["components"]["keyboard_controller"]["down_velocity"]["x"],
//                         entity["components"]["keyboard_controller"]["down_velocity"]["y"]),
//                     glm::vec2(
//                         entity["components"]["keyboard_controller"]["left_velocity"]["x"],
//                         entity["components"]["keyboard_controller"]["left_velocity"]["y"]));
//             }
//             sol::optional<sol::table> script = entity["components"]["on_update_script"];
//             if (script != sol::nullopt)
//             {
//                 sol::function func = entity["components"]["on_update_script"][0];
//                 newEntity.AddComponent<ScriptComponent>(func);
//             }
//         }
//         i++;
//     }
// }
