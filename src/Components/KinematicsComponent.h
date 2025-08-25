#pragma once

#include "../Core/Vector2.h"
#include <cassert>
#include <cmath>

struct KinematicsComponent
{
    Vector2 velocity = Vector2();
    Vector2 acceleration = Vector2();
    float damping = 1.0f;

    void integrate(float duration)
    {
        if (duration > 0.0)
        {
            // Update linear velocity from the acceleration
            velocity.addScaledVector(acceleration, duration);

            // Impose drag to make movement feel nicer
            velocity = velocity * powf(damping, duration);
        }
    }
};
