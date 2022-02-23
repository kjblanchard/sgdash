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

                  //handle y movement
                  if (y_step != 0)
                  {
                      handle_y_movement(y_step, reg, entity, transform, rigid_body, box);
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

                  std::cout << "The current y value (greater than or = 0 is: " << rigid_body.velocity.y << " and the onground status is " << rigid_body.on_ground << std::endl;
              });
}
void MovementSystem::handle_y_movement(double y_step, entt::registry &reg, entt::entity &entity, TransformComponent &transform, RigidBodyComponent &rigid_body, BoxColliderComponent &box)
{
    bool y_collision = false;
    if (y_step > 0)
    {
        while (!y_collision && y_step > 0)
        {
            //falling down logic
            //I think the logic that's happening is that when it first hits the ground, the step is potentially large, and breaks out, then the next iteration it's smaller so it moves closer down, and there is no collision until it hits the ground.

            auto move_amount = (y_step >= 1) ? 1 : y_step;
            auto desired_x = transform.position.x;
            auto desired_y = transform.position.y + move_amount;
            auto desired_position = glm::vec2{desired_x, desired_y};
            if (CollisionSystem::check_collision_with_walls(reg, entity, box, desired_position))
            {
                auto y_coord_intersect = static_cast<int>(desired_y);
                //reduce the integer but be very close
                transform.position.y = y_coord_intersect - 0.0001;
                std::cout << "Desired Y is " << desired_y << " and actual Y is " << transform.position.y << std::endl;
                y_collision = true;
                rigid_body.velocity.y = 0;
                rigid_body.on_ground = true;
                return;
            }
            transform.position.y = desired_position.y;
            y_step -= move_amount;
        }
        rigid_body.on_ground = false;
    }
    else
    {
        while (!y_collision && y_step < 0)
        {
            auto move_amount = (y_step <= 1) ? -1 : y_step;
            auto desired_position = glm::vec2{transform.position.x, transform.position.y += move_amount};
            if (CollisionSystem::check_collision_with_walls(reg, entity, box, desired_position))
            {
                y_collision = true;
                transform.rotation = 0;
                rigid_body.velocity.y = 0;
                rigid_body.on_ground = true;
                break;
            }
            y_step -= move_amount;
        }
        rigid_body.on_ground = false;
    }
}