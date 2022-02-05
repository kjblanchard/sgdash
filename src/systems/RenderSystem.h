#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <algorithm>
#include <ecs/ECS.h>
#include <components/TransformComponent.h>
#include <components/SpriteComponent.h>
#include <essential/asset_store.hpp>
#include <SDL2/SDL.h>

class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera)
    {
        // Create a vector with both Sprite and Transform component of all entities
        struct RenderableEntity
        {
            TransformComponent transformComponent;
            SpriteComponent spriteComponent;
        };
        auto entities = GetSystemEntities();
        std::vector<RenderableEntity> renderableEntities;
        for (const auto& entity : entities)
        {
            RenderableEntity renderableEntity;
            renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
            renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();

            bool isEntityOutsideCameraView = (
                renderableEntity.transformComponent.position.x  + (renderableEntity.transformComponent.scale.x * renderableEntity.spriteComponent.width) < camera.x ||
                renderableEntity.transformComponent.position.x > camera.x + camera.w ||
                renderableEntity.transformComponent.position.y  + (renderableEntity.transformComponent.scale.y * renderableEntity.spriteComponent.height) < camera.y ||
                renderableEntity.transformComponent.position.y > camera.y + camera.h
            );

            //Bypass rendering entities if they are outside camera view
            if (isEntityOutsideCameraView && !renderableEntity.spriteComponent.isFixed)
                continue;
            renderableEntities.emplace_back(renderableEntity);
        }

        // Sort the vector by the z-index value
        std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity &a, const RenderableEntity &b)
                  { return a.spriteComponent.zIndex < b.spriteComponent.zIndex; });

        // Loop all entities that the system is interested in
        for (auto entity : renderableEntities)
        {
            const auto transform = entity.transformComponent;
            const auto sprite = entity.spriteComponent;

            // Set the source rectangle of our original sprite texture
            SDL_Rect srcRect = sprite.srcRect;

            // Set the destination rectangle with the x,y position to be rendered
            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)};

            // Draw the texture on the destination renderer
            SDL_RenderCopyEx(
                renderer,
                assetStore->GetTexture(sprite.assetId),
                &srcRect,
                &dstRect,
                transform.rotation,
                NULL,
                sprite.flip);
        }
    }
};

#endif
