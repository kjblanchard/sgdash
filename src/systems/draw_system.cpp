#include <systems/draw_system.hpp>

void DrawSystem::Update(entt::registry &reg, SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera)
{
    auto entities = reg.group<SpriteComponent, TransformComponent>();
    entities.each([&assetStore, &renderer, &camera](auto &sprite, auto &transform)
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

}