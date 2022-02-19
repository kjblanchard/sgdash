#include <iostream>
#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <core/world.hpp>
#include <systems/gravity_system.hpp>
#include <components/rigid_body_component.hpp>
#include <components/transform_component.hpp>
#include <utilities/lualoader.hpp>

//Statics
int GravitySystem::gravity = 0;
double GravitySystem::friction = 0;
double GravitySystem::min_x = 0;
double GravitySystem::max_x = 0;
double GravitySystem::min_y = 0;
double GravitySystem::max_y = 0;

void GravitySystem::Setup()
{
    sol::state lua_temp;
    auto lua_gravity_table = utilities::load_lua_table(lua_temp, World::config_file_string, lua_physics_string);
    GravitySystem::gravity = lua_gravity_table[GravitySystem::lua_gravity_string];
    GravitySystem::min_x = lua_gravity_table[lua_friction_min_string];
    GravitySystem::max_x = lua_gravity_table[lua_friction_max_string];
    GravitySystem::min_y = lua_gravity_table[lua_gravity_min_string];
    GravitySystem::max_y = lua_gravity_table[lua_gravity_max_string];
    std::cout << "Gravity y min/max: " << GravitySystem::min_y << " : " << GravitySystem::max_y << std::endl;
    GravitySystem::friction = lua_gravity_table[GravitySystem::lua_friction_string];
}

void GravitySystem::Update(entt::registry &reg, double &delta_time)
{
    auto view = reg.view<RigidBodyComponent>();
    double gravity_step = gravity * delta_time;
    double friction_step = friction * delta_time;

    //TODOHandle the game loading when the step is huge from loading for now
    if (gravity_step > 30)
        gravity_step = 0;
    if (friction_step > 30)
        friction_step = 0;

    view.each([&delta_time, &gravity_step, &friction_step](auto entity, auto &rigid_body)
              {
                  rigid_body.velocity.x = GravitySystem::keep_gravity_x_in_range(rigid_body.velocity.x, friction_step);
                  rigid_body.velocity.y = GravitySystem::keep_gravity_y_in_range(rigid_body.velocity.y, gravity_step);
                    // rigid_body.velocity.y = (rigid_body.velocity.y + gravity_step > max_y) ? max_y : gravity_step + rigid_body.velocity.y;
              });
}

double GravitySystem::keep_gravity_x_in_range(double vel_x, double fric)
{
    if (vel_x == 0)
        return 0;

    if (vel_x > 0)
    {
        auto step = vel_x - fric;
        if (step > GravitySystem::max_x)
            step = GravitySystem::max_x;
        if (step < GravitySystem::min_x)
            step = 0;
        return step;
    }
    else
    {
        auto step = vel_x + fric;
        if (step < -GravitySystem::max_x)
            step = -GravitySystem::max_x;
        if (step > -GravitySystem::min_x)
            step = 0;
        return step;
    }
}
double GravitySystem::keep_gravity_y_in_range(double vel_y, double gravity)
{

    auto step = vel_y + gravity;
    if (step > GravitySystem::max_y)
        step = GravitySystem::max_y;
    if (step < GravitySystem::min_y)
        step = 0;
    std::cout << step << "Is the step" << std::endl;
    return step;
}