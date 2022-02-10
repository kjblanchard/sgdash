#pragma once
#include <entt/entt.hpp>
#include <sol/sol.hpp>

class GravitySystem
{
public:
    static void Setup(sol::state &lua);
    static void Update(entt::registry &reg, double &delta_time);

private:
    static int gravity;
    static double friction;

    static const char *lua_physics_string;
    static const char *lua_gravity_string;
    static const char *lua_friction_string;
};