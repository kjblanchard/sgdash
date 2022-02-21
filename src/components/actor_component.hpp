#pragma once
struct ActorComponent
{
    ActorComponent(int actor_num) : actor_type{actor_num} {}
    int actor_type;
};