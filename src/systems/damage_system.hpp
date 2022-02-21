#pragma once
#include <iostream>
#include <entt/entt.hpp>

#include <systems/collision_system.hpp>
#include <systems/sound_system.hpp>
#include <components/sprite_component.hpp>
#include <components/rigid_body_component.hpp>
#include <primitives/actor_list.hpp>

class DamageSystem
{
public:
    static void setup()
    {
        CollisionSystem::collision_type_to_func_map[std::make_pair((int)COLLISION_LIST::PLAYER, (int)COLLISION_LIST::DAMAGE)] = cause_damage;
    }
    static inline void cause_damage(entt::registry &reg, const entt::entity &lhs, const entt::entity &rhs)
    {
        auto &sprite = reg.get<SpriteComponent>(lhs);
        auto &rb = reg.get<RigidBodyComponent>(lhs);
        if (sprite.enabled)
        {
            SoundSystem::stop_music_with_fadeout();
            SoundSystem::play_sfx_oneshot();
            sprite.enabled = false;
            rb.velocity.x = 0;
            rb.velocity.y = 0;
            std::cout << "You should be dead!" << std::endl;
        }
    }
};