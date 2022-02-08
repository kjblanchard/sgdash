#pragma once

#include <entt/entt.hpp>

class MovementSystem
{
public:
    static void Update(entt::registry &reg, const double &delta_time);
};
