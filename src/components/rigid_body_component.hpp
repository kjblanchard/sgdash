#pragma once
#include <glm/glm.hpp>

struct RigidBodyComponent
{
    RigidBodyComponent(glm::vec2 vel = glm::vec2(0,0))
        : velocity(vel)
    {
    }
    ~RigidBodyComponent(){};
    glm::vec2 velocity;
    inline void apply_force(glm::vec2 force){
        velocity += force;
    }
};
