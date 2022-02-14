#include <iostream>
#include <systems/movement_system.hpp>
#include <components/box_collider_component.hpp>
#include <components/rigid_body_component.hpp>
#include <components/transform_component.hpp>
#include <systems/collision_system.hpp>
#include <core/logger.hpp>

void MovementSystem::Update(entt::registry &reg, const double &delta_time)
{
    auto view = reg.view<RigidBodyComponent, TransformComponent, BoxColliderComponent>();

    view.each([&delta_time, &view, &reg](auto entity, auto &rigid_body, auto &transform, auto &box)
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

                  bool x_collision = false;
                  bool y_collision = false;

                  while (!x_collision && x_step > 1)
                  {
                      if (CollisionSystem::check_collision(reg, entity, box, transform))
                      {
                          x_collision = true;
                          Logger::Err("Collision");
                          break;
                      }
                      ++transform.position.x;
                      --x_step;
                  }
                  while (!x_collision && x_step < -1)
                  {
                      if (CollisionSystem::check_collision(reg, entity, box, transform))
                      {
                          x_collision = true;
                          Logger::Err("Collision");
                          break;
                      }
                      --transform.position.x;
                      ++x_step;
                  }
                  while (!y_collision && y_step > 1)
                  {
                      if (CollisionSystem::check_collision(reg, entity, box, transform))
                      {
                          Logger::Err("Collision");
                          y_collision = true;
                          break;
                      }
                      ++transform.position.y;
                      --y_step;
                  }
                  while (!y_collision && y_step < -1)
                  {
                      if (CollisionSystem::check_collision(reg, entity, box, transform))
                      {
                          Logger::Err("Collision");
                          y_collision = true;
                          break;
                      }
                      --transform.position.y;
                      ++y_step;
                  }

                  if (!x_collision)
                      transform.position.x += x_step;
                  if (!y_collision)
                      transform.position.y += y_step;
              });
}