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
                  double x_step = rigid_body.velocity.x * delta_time;
                  double y_step = rigid_body.velocity.y * delta_time;


                  //Handle the game loading for testing so that you don't move after a load.
                  if (x_step > 10)
                      x_step = 0;
                  if (y_step > 10)
                      y_step = 0;

                  bool x_collision = false;
                  bool y_collision = false;

                  //handle y movement
                  while (!y_collision && y_step > 1)
                  {
                      //Add to y to test if the next movement will collide
                      ++transform.position.y;
                      if (CollisionSystem::check_collision(reg, entity, box, transform))
                      {
                          //hit the ground
                          rigid_body.velocity.y = 0;
                          y_collision = true;
                          //Move transform back as there was a colission
                          --transform.position.y;
                          break;
                      }
                      //Else subtract from y step
                      --y_step;
                  }
                  while (!y_collision && y_step < -1)
                  {
                      --transform.position.y;
                      if (CollisionSystem::check_collision(reg, entity, box, transform))
                      {
                          rigid_body.velocity.y = 0;
                          y_collision = true;
                          ++transform.position.y;
                          break;
                      }
                      ++y_step;
                  }

                  //handle X
                  while (!x_collision && x_step > 1)
                  {
                      ++transform.position.x;
                      if (CollisionSystem::check_collision(reg, entity, box, transform))
                      {
                          Logger::Err("There would be a Collision in X");
                          rigid_body.velocity.x = 0;
                          x_collision = true;
                          --transform.position.x;
                          break;
                      }
                      --x_step;
                  }
                  while (!x_collision && x_step < -1)
                  {
                      --transform.position.x;
                      if (CollisionSystem::check_collision(reg, entity, box, transform))
                      {
                          Logger::Err("There would be a Collision in X");
                          rigid_body.velocity.x = 0;
                          x_collision = true;
                          ++transform.position.x;
                          break;
                      }
                      ++x_step;
                  }

                  if (!x_collision && x_step > 0 && x_step < 1)
                      transform.position.x += x_step;
              });
}