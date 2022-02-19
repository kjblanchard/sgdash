#pragma once

struct CameraFollowComponent
{
    CameraFollowComponent() : follow_x{true}, follow_y{false} {}
    ~CameraFollowComponent() {}
    bool follow_x;
    bool follow_y;
};