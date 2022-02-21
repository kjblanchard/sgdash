#include <core/level.hpp>
#include <core/world.hpp>
#include <core/levelloader.hpp>
#include <sol/sol.hpp>

entt::registry Level::level_registry;
void Level::load_level()
{
    sol::state level_lua;
    auto loader = LevelLoader();
    loader.LoadTiledLevel(level_lua, level_registry, World::assetStore, World::renderer, 1);
}
void Level::restart()
{
    sol::state level_lua;
    auto loader = LevelLoader();
    level_registry.clear();
    loader.LoadTiledLevel(level_lua, level_registry, World::assetStore, World::renderer, 1);

}