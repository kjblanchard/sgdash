#pragma once
#include <entt/entt.hpp>

class GravitySystem
{
public:
    static void Update(entt::registry &reg, double &delta_time);
};