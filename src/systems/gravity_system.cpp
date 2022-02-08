#include <iostream>
#include <systems/gravity_system.hpp>
#include <components/rigid_body_component.hpp>
#include <components/transform_component.hpp>

void GravitySystem::Update(entt::registry &reg, double &delta_time)
{
    auto view = reg.view<RigidBodyComponent>();

    int gravity = 200;
    double friction = .90;
    auto gravity_step = gravity * delta_time;
    //Handle the game loading when the step is huge from loading for now
    if (gravity_step > 30)
        gravity_step = 0;

    view.each([&delta_time, &gravity_step, &friction](auto entity, auto &rigid_body)
              {
                  auto friction_step = rigid_body.velocity.x * friction * delta_time;
                  if (friction_step > 30)
                      friction_step = 0;
                      std::cout << "Friction step is " << friction_step << std::endl;
                  rigid_body.velocity.y = (rigid_body.velocity.y + gravity_step > RigidBodyComponent::max_y_speed) ? RigidBodyComponent::max_y_speed : gravity_step + rigid_body.velocity.y;
                  rigid_body.velocity.x -= friction_step;
              });
}