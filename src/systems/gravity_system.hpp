#pragma once

namespace sol
{
    class state;
}
class GravitySystem
{
public:
    static void Setup();
    static void Update(entt::registry &reg, double &delta_time);

private:
    static double keep_gravity_x_in_range(double vel_x, double fric);
    static int gravity;
    static double friction;
    static double min_x;
    static double max_x;

    static constexpr char *lua_physics_string = (char *)"physics_config";
    static constexpr char *lua_gravity_string = (char *)"gravity";
    static constexpr char *lua_friction_string = (char *)"friction";
    static constexpr char *lua_friction_min_string = (char *)"min_friction";
    static constexpr char *lua_friction_max_string = (char *)"max_friction";
};