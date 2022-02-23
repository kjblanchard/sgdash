#pragma once
struct JumpComponent
{
    JumpComponent()
    {
    }
    ~JumpComponent() {}
    int jump_vel = 200;
    bool is_currently_jumping = false;
};