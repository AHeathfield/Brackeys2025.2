#pragma once

#include "../Core/Vector2.h"

struct BoxColliderComponent
{
    // A Copy Constructor
    // BoxColliderComponent(const BoxColliderComponent& other)
    //     : position(other.position),
    //       w(other.w),
    //       h(other.h)
    // {}

    Vector2 position;
    int w;
    int h;
};
