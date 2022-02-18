#include <iostream>
#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <systems/gravity_system.hpp>
#include <components/rigid_body_component.hpp>
#include <components/transform_component.hpp>
#include <utilities/lualoader.hpp>

//Statics
int GravitySystem::gravity = 0;
double GravitySystem::friction = 0;
const char *GravitySystem::lua_physics_string = "physics_config";
const char *GravitySystem::lua_gravity_string = "gravity";
const char *GravitySystem::lua_friction_string = "friction";

void GravitySystem::Setup(sol::state &lua)
{
    auto lua_gravity_table = lua[GravitySystem::lua_physics_string];
    GravitySystem::gravity = lua_gravity_table[GravitySystem::lua_gravity_string];
    GravitySystem::friction = lua_gravity_table[GravitySystem::lua_friction_string];
}

void GravitySystem::Update(entt::registry &reg, double &delta_time)
{
    auto view = reg.view<RigidBodyComponent>();
    double gravity_step = gravity * delta_time;

    //TODOHandle the game loading when the step is huge from loading for now
    if (gravity_step > 30)
        gravity_step = 0;

    view.each([&delta_time, &gravity_step](auto entity, auto &rigid_body)
              {
                  double friction_step = rigid_body.velocity.x * friction * delta_time;
                  if (friction_step > 30)
                      friction_step = 0;
                  std::cout << "Friction step is " << friction_step << std::endl;
                  std::cout << "Gravity Step is step is " << gravity_step << std::endl;
                  rigid_body.velocity.y = (rigid_body.velocity.y + gravity_step > RigidBodyComponent::max_y_speed) ? RigidBodyComponent::max_y_speed : gravity_step + rigid_body.velocity.y;
              });
}