#include <systems/jump_system.hpp>
#include <systems/player_controller_system.hpp>
#include <components/jump_component.hpp>
#include <components/rigid_body_component.hpp>
#include <components/player_controller_component.hpp>
#include <components/transform_component.hpp>
#include <iostream>

void JumpSystem::update(entt::registry &reg)
{
    auto view = reg.view<JumpComponent, RigidBodyComponent, TransformComponent>();
    view.each([](auto entity, auto &jump, auto &rb, auto &transform)
              {
                  //Handle if we are on the ground, to cancel out the jumping flag. And set rotation back to 0
                  if (jump.is_currently_jumping)
                  {
                      if (rb.on_ground)
                      {
                          jump.is_currently_jumping = false;
                          transform.rotation = 0;
                      }
                  }
                  //If the button is pressed, then Jump.
                  auto button = PlayerControllerSystem::player_button_lookup(Joystick::Joystick_Buttons::CONFIRM);
                  if (PlayerControllerSystem::KeyJustPressed(button) || PlayerControllerSystem::IsKeyHeldDown(button))
                  {
                      if (rb.on_ground)
                      {
                          rb.velocity.y -= jump.jump_vel;
                          jump.is_currently_jumping = true;
                      }
                  }

                  //Rotate if we are currently jumping.
                  if (jump.is_currently_jumping)
                  {
                      transform.rotation += 3;
                  }
              });
}