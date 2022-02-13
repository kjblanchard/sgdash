#include <iostream>

#include <systems/collision_system.hpp>
#include <components/box_collider_component.hpp>
#include <components/transform_component.hpp>
#include <core/logger.hpp>

void CollisionSystem::Update(entt::registry &reg, const double &delta_time)
{
    auto view = reg.view<BoxColliderComponent, TransformComponent>();
    for (auto current = view.begin(); current != view.end(); ++current)
    {
        auto& current_box = view.get<BoxColliderComponent>(*current);
        auto& current_transform = view.get<TransformComponent>(*current);
        auto another = current;
        ++another;

        for(; another != view.end(); ++another){
            auto& another_box = view.get<BoxColliderComponent>(*another);
            auto& another_transform = view.get<TransformComponent>(*another);

            SDL_Rect lhs;
            lhs.x = current_transform.position.x;
            lhs.y = current_transform.position.y;
            lhs.h = current_box.bounding_box.h;
            lhs.w = current_box.bounding_box.w;
            SDL_Rect rhs;
            rhs.x = another_transform.position.x;
            rhs.y = another_transform.position.y;
            rhs.h = another_box.bounding_box.h;
            rhs.w = another_box.bounding_box.w;

            if(CollisionSystem::aabb_collision_check(lhs, rhs))
            {
                Logger::Err("Collision!");
            }
        }
    }

}