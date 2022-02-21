#pragma once
#include <entt/entt.hpp>

class Level
{
public:
    static entt::registry level_registry;
    static void load_level();
    static void restart();
};
