#include <systems/jump_system.hpp>
#include <systems/player_controller_system.hpp>
#include <components/jump_component.hpp>
#include <components/rigid_body_component.hpp>
#include <components/player_controller_component.hpp>
#include <iostream>

void JumpSystem::update(entt::registry &reg)
{
    auto view = reg.view<JumpComponent, RigidBodyComponent>();
    view.each([](auto entity, auto &jump, auto &rb)
              {
                  auto button = PlayerControllerSystem::player_button_lookup(Joystick::Joystick_Buttons::UP);
                  if (PlayerControllerSystem::KeyJustPressed(button))
                  {
                      if (rb.on_ground)
                          rb.velocity.y -= jump.jump_vel;
                  }
              });
}