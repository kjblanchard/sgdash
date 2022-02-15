#pragma once
#include <glm/glm.hpp>

struct RigidBodyComponent
{
    RigidBodyComponent()
        : velocity(glm::vec2(100,0))
    {
    }
    ~RigidBodyComponent(){};
    glm::vec2 velocity;
    static const int max_y_speed = 150;
};
