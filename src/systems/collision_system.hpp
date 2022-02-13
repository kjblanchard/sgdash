#pragma once

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

class CollisionSystem
{
public:
    static void Update(entt::registry &reg, const double &delta_time);
    static inline bool aabb_collision_check(SDL_Rect& lhs, SDL_Rect& rhs){
        return SDL_HasIntersection(&lhs,&rhs);
    }
    static inline SDL_Rect aabb_collision_area(SDL_Rect& lhs, SDL_Rect& rhs){
        SDL_Rect overlap;
        SDL_IntersectRect(&lhs, &rhs, &overlap);
        return overlap;

    }
};