#pragma once

#include <SDL2/SDL.h>

class TransformComponent;
class BoxColliderComponent;

class CollisionSystem
{
public:
    static void Update(entt::registry &reg, const double &delta_time);
    static bool check_collision(entt::registry &reg, entt::entity &ent, BoxColliderComponent& box_collider, TransformComponent& transform);

    static inline bool aabb_collision_check(SDL_Rect& lhs, SDL_Rect& rhs){
        return SDL_HasIntersection(&lhs,&rhs);
    }
    static inline SDL_Rect aabb_collision_area(SDL_Rect& lhs, SDL_Rect& rhs){
        SDL_Rect overlap;
        SDL_IntersectRect(&lhs, &rhs, &overlap);
        return overlap;

    }
};