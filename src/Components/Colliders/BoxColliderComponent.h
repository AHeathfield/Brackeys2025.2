#pragma once

#include "../../Core/ECS.h"
#include "../../Core/Vector2.h"

enum class CollisionSide
{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    NONE
};

struct BoxColliderComponent
{
    Vector2 position;
    int w;
    int h;


    CollisionSide collisionSide = CollisionSide::NONE;
    // bool isBottomColliding = false;
    // bool isTopColliding = false;
    // bool isRightColliding = false;
    // bool isLeftColliding = false;

    virtual ~BoxColliderComponent() = default;
    virtual void HandleCollision(Entity other) = 0;

    // Constructor which is used by the classes that inherit this
    BoxColliderComponent(const Vector2& pos, int width, int height)
        : position(pos),
        w(width),
        h(height)
    {}

    void resetCollisionSide()
    {
        collisionSide = CollisionSide::NONE;
        // isBottomColliding = false;
        // isLeftColliding = false;
        // isRightColliding = false;
        // isTopColliding = false;
    }

    bool isColliding()
    {
        return (collisionSide != CollisionSide::NONE);
        // return (isBottomColliding || isTopColliding || isRightColliding || isLeftColliding);
    }
};
