#include <iostream>
#include <systems/movement_system.hpp>
#include <systems/sound_system.hpp>
#include <components/box_collider_component.hpp>
#include <components/rigid_body_component.hpp>
#include <components/transform_component.hpp>
#include <components/sprite_component.hpp>
#include <systems/collision_system.hpp>
#include <core/logger.hpp>

void MovementSystem::Update(entt::registry &reg, const double &delta_time)
{
    auto view = reg.view<RigidBodyComponent, TransformComponent, BoxColliderComponent>();

    view.each([&delta_time, &view, &reg](auto entity, auto &rigid_body, auto &transform, auto &box)
              {
                  double x_step = rigid_body.velocity.x * delta_time;
                  double y_step = rigid_body.velocity.y * delta_time;
                  //TODO remove this and the death parts in here.
                  auto &sprite = reg.get<SpriteComponent>(entity);

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
                      if (CollisionSystem::check_collision_with_walls(reg, entity, box, transform))
                      {
                          //hit the ground
                          rigid_body.velocity.y = 0;
                          y_collision = true;
                          rigid_body.on_ground = true;
                          transform.rotation = 0;
                          //Move transform back as there was a colission
                          --transform.position.y;
                          break;
                      }
                      //Else subtract from y step
                      --y_step;
                      if (!y_collision)
                      {
                          rigid_body.on_ground = false;
                          transform.rotation += 5;
                      }
                  }
                  while (!y_collision && y_step < -1)
                  {
                      --transform.position.y;
                      if (CollisionSystem::check_collision_with_walls(reg, entity, box, transform))
                      {
                          rigid_body.velocity.y = 0;
                          y_collision = true;
                          rigid_body.on_ground = true;
                          transform.rotation = 0;
                          ++transform.position.y;
                          break;
                      }
                      ++y_step;
                      if (!y_collision)
                      {
                          rigid_body.on_ground = false;
                          transform.rotation += 5;
                      }
                  }

                  //handle X
                  while (!x_collision && x_step > 0)
                  {
                      //TODO copy this to the others when doing refactoring to make sure that it works properly for x < 1 steps
                      auto move_amount = (x_step >= 1) ? 1 : x_step;
                      ++transform.position.x;
                      transform.position.x += move_amount;
                      if (CollisionSystem::check_collision_with_walls_x(reg, entity, box, transform))
                      {
                          Logger::Err("There would be a Collision in X");
                          rigid_body.velocity.x = 0;
                          x_collision = true;
                          //   -transform.position.x;
                          transform.position.x -= move_amount;
                          //Handle death.
                          SoundSystem::stop_music_with_fadeout();
                          SoundSystem::play_sfx_oneshot();
                          sprite.enabled = false;
                          rigid_body.velocity.x = 0;
                          rigid_body.velocity.y = 0;
                          break;
                      }
                      --x_step;
                  }
                  while (!x_collision && x_step < 0)
                  {
                      --transform.position.x;
                      if (CollisionSystem::check_collision_with_walls_x(reg, entity, box, transform))
                      {
                          Logger::Err("There would be a Collision in X");
                          rigid_body.velocity.x = 0;
                          x_collision = true;
                          ++transform.position.x;
                          //Handle death.
                          SoundSystem::stop_music_with_fadeout();
                          SoundSystem::play_sfx_oneshot();
                          sprite.enabled = false;
                          rigid_body.velocity.x = 0;
                          rigid_body.velocity.y = 0;
                          break;
                      }
                      ++x_step;
                  }
                  CollisionSystem::check_collision_with_actors(reg, entity, box, transform);

                  //   if (!x_collision && x_step > 0 && x_step < 1)
                  //       transform.position.x += x_step;
              });
}