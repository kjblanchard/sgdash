#include <iostream>
#include <systems/draw_system.hpp>
#include <components/rigid_body_component.hpp>
#include <components/box_collider_component.hpp>
#include <components/transform_component.hpp>
#include <components/sprite_component.hpp>
#include <core/asset_store.hpp>
#include <core/world.hpp>


void DrawSystem::Update(entt::registry &reg, SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera)
{
    auto entities = reg.view<SpriteComponent, TransformComponent>();
    entities.each([&assetStore, &renderer, &camera, &reg](auto entity, auto &sprite, auto &transform)
                  {
                      SDL_Rect srcRect = sprite.srcRect;
                      SDL_Rect dstRect = {
                          static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                          static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                          static_cast<int>(sprite.width * transform.scale.x),
                          static_cast<int>(sprite.height * transform.scale.y)};

                      SDL_RenderCopyEx(
                          renderer,
                          assetStore->GetTexture(sprite.assetId),
                          &srcRect,
                          &dstRect,
                          transform.rotation,
                          NULL,
                          sprite.flip);
                  });

    if (World::isDebug)
    {
        auto debug_entities = reg.view<BoxColliderComponent, TransformComponent>();
        debug_entities.each([&renderer, &camera, &reg](auto entity, auto &box, auto &transform)
                            {
                                SDL_Rect lhs;
                                lhs.x = static_cast<int>(transform.position.x + box.offset.x);
                                lhs.y = static_cast<int>(transform.position.y + box.offset.y);
                                lhs.w = static_cast<int>(box.bounding_box.w);
                                lhs.h = static_cast<int>(box.bounding_box.h);
                                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                                SDL_RenderDrawRect(renderer, &lhs);
                            });
    }
}