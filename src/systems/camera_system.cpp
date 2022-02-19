#include <systems/camera_system.hpp>
#include <components/camera_follow_component.hpp>
#include <components/transform_component.hpp>
#include <core/world.hpp>

void CameraSystem::update(entt::registry &reg, SDL_Rect &camera)
{
    auto view = reg.view<CameraFollowComponent>();
    view.each([&reg, &camera](auto entity, auto &follow_entity)
              {
                  auto transform = reg.get<TransformComponent>(entity);
                  auto diff = transform.position.x - camera.x;
                  auto middle_screen_x = World::unscaledWidth / 2;
                  if (diff > middle_screen_x)
                  {
                      auto offset = diff - middle_screen_x;
                      camera.x += offset;
                  }
              });
}