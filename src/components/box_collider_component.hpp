#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct BoxColliderComponent
{

    BoxColliderComponent( SDL_Rect bounds, glm::vec2 offset = glm::vec2()) : bounding_box{bounds}, offset{offset}
    {
    }
    ~BoxColliderComponent(){}

    SDL_Rect bounding_box;
    glm::vec2 offset;
};