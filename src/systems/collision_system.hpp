#pragma once

#include <SDL2/SDL.h>
#include <typeinfo>
#include <map>
#include <functional>
#include <iostream>

class TransformComponent;
class BoxColliderComponent;

class CollisionSystem
{
public:
    static bool check_collision_with_walls(entt::registry &reg, entt::entity &ent, BoxColliderComponent &box_collider, TransformComponent &transform);
    static bool check_collision_with_actors(entt::registry &reg, entt::entity &ent, BoxColliderComponent &box_collider, TransformComponent &transform);
    static std::map<std::pair<int, int>, void (*)(entt::registry &reg, const entt::entity&, const entt::entity&)> collision_type_to_func_map;

    static inline bool aabb_collision_check(SDL_Rect &lhs, SDL_Rect &rhs)
    {
        return SDL_HasIntersection(&lhs, &rhs);
    }
    static inline SDL_Rect aabb_collision_area(SDL_Rect &lhs, SDL_Rect &rhs)
    {
        SDL_Rect overlap;
        SDL_IntersectRect(&lhs, &rhs, &overlap);
        return overlap;
    }
};