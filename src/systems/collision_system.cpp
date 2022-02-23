#include <iostream>
#include <entt/entt.hpp>
#include <systems/collision_system.hpp>
#include <components/box_collider_component.hpp>
#include <components/transform_component.hpp>
#include <components/wall_component.hpp>
#include <components/actor_component.hpp>
#include <core/logger.hpp>

std::map<std::pair<int, int>, void (*)(entt::registry &, const entt::entity &, const entt::entity &)> CollisionSystem::collision_type_to_func_map;

bool CollisionSystem::check_collision_with_walls(entt::registry &reg, entt::entity &ent, BoxColliderComponent &box_collider, glm::vec2 desired_transform)
{
    auto view = reg.view<BoxColliderComponent, TransformComponent, WallComponent>();
    SDL_Rect lhs;
    lhs.x = static_cast<int>(desired_transform.x + box_collider.offset.x);
    lhs.y = static_cast<int>(desired_transform.y + box_collider.offset.y);
    lhs.w = box_collider.bounding_box.w;
    lhs.h = box_collider.bounding_box.h;

    for (auto current = view.begin(); current != view.end(); ++current)
    {
        auto &current_box = view.get<BoxColliderComponent>(*current);
        auto &current_transform = view.get<TransformComponent>(*current);
        if (ent != *current)
        {
            SDL_Rect rhs;
            rhs.x = static_cast<int>(current_transform.position.x);
            rhs.y = static_cast<int>(current_transform.position.y);
            rhs.w = current_box.bounding_box.w;
            rhs.h = current_box.bounding_box.h;
            if (aabb_collision_check(lhs, rhs))
            {
                return true;
            }
        }
    }
    return false;
}
bool CollisionSystem::check_collision_with_walls_x(entt::registry &reg, entt::entity &ent, BoxColliderComponent &box_collider, TransformComponent &transform)
{
    auto view = reg.view<BoxColliderComponent, TransformComponent, WallComponent>();
    SDL_Rect lhs;
    lhs.x = transform.position.x + box_collider.offset.x;
    lhs.y = transform.position.y + box_collider.offset.y;
    lhs.w = box_collider.bounding_box.w;
    lhs.h = box_collider.bounding_box.h;
    // auto actor = view.get<WallComponent>(ent);
    // int actor_type_num = actor.actor_type;

    for (auto current = view.begin(); current != view.end(); ++current)
    {
        auto &current_box = view.get<BoxColliderComponent>(*current);
        auto &current_transform = view.get<TransformComponent>(*current);
        if (ent != *current)
        {
            SDL_Rect rhs;
            rhs.x = current_transform.position.x;
            rhs.y = current_transform.position.y;
            rhs.w = current_box.bounding_box.w;
            rhs.h = current_box.bounding_box.h;
            // auto rhs_actor = view.get<ActorComponent>(*current);
            // int rhs_actor_type_num = rhs_actor.actor_type;

            if (aabb_collision_check(lhs, rhs))
            {
                return true;
                // auto lookup_key = std::make_pair(actor_type_num, rhs_actor_type_num);
                // auto collision_func = collision_type_to_func_map.find(lookup_key);
                // if (collision_func == collision_type_to_func_map.end())
                //     continue;
                // collision_type_to_func_map[lookup_key](reg, ent, *current);
            }
        }
    }
    return false;
}
bool CollisionSystem::check_collision_with_actors(entt::registry &reg, entt::entity &ent, BoxColliderComponent &box_collider, TransformComponent &transform)
{
    auto view = reg.view<BoxColliderComponent, TransformComponent, ActorComponent>();
    SDL_Rect lhs;
    lhs.x = transform.position.x + box_collider.offset.x;
    lhs.y = transform.position.y + box_collider.offset.y;
    lhs.w = box_collider.bounding_box.w;
    lhs.h = box_collider.bounding_box.h;
    auto actor = view.get<ActorComponent>(ent);
    int actor_type_num = actor.actor_type;

    for (auto current = view.begin(); current != view.end(); ++current)
    {
        auto &current_box = view.get<BoxColliderComponent>(*current);
        auto &current_transform = view.get<TransformComponent>(*current);
        if (ent != *current)
        {
            SDL_Rect rhs;
            rhs.x = current_transform.position.x;
            rhs.y = current_transform.position.y;
            rhs.w = current_box.bounding_box.w;
            rhs.h = current_box.bounding_box.h;
            auto rhs_actor = view.get<ActorComponent>(*current);
            int rhs_actor_type_num = rhs_actor.actor_type;

            if (aabb_collision_check(lhs, rhs))
            {
                auto lookup_key = std::make_pair(actor_type_num, rhs_actor_type_num);
                auto collision_func = collision_type_to_func_map.find(lookup_key);
                if (collision_func == collision_type_to_func_map.end())
                    continue;
                collision_type_to_func_map[lookup_key](reg, ent, *current);
            }
        }
    }
    return false;
}
