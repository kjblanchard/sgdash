#include <iostream>
#include <systems/movement_system.hpp>
#include <components/rigid_body_component.hpp>
#include <components/transform_component.hpp>

void MovementSystem::Update(entt::registry &reg, const double &delta_time)
{
    auto view = reg.view<RigidBodyComponent, TransformComponent>();

    view.each([&delta_time](auto entity, auto &rigid_body, auto &transform)
              {
                  std::cout << rigid_body.velocity.y << "This is the y velocity " << std::endl;
                  std::cout << rigid_body.velocity.x << "This is the x velocity " << std::endl;

                  double x_step = rigid_body.velocity.x * delta_time;
                  double y_step = rigid_body.velocity.y * delta_time;

                  //Handle the game loading for testing so that you don't move after a load.
                  if (x_step > 10)
                      x_step = 0;
                  if (y_step > 10)
                      y_step = 0;

                  transform.position.x += x_step;
                  transform.position.y += y_step;
              });
}