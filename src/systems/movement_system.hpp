#pragma once

#include <entt/entt.hpp>

class TransformComponent;
class RigidBodyComponent;
class BoxColliderComponent;
class MovementSystem
{
public:
    static void Update(entt::registry &reg, const double &delta_time);

private:
    static void handle_y_movement(double y_step, entt::registry &reg, entt::entity& entity, TransformComponent &transform, RigidBodyComponent &rigid_body, BoxColliderComponent &box);
};
