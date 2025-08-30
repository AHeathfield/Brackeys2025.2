#pragma once

#include "BoxColliderComponent.h"

// This collider is just a default, it doesn't do any special handling
// It's good for when objects have certain components we want to check against when colliding
// I'm still not sure if it's better to create a bunch of colliders or rely on components, they both have their ups and downs
class DefaultCollider : public BoxColliderComponent
{
public:
    DefaultCollider(const Vector2& pos, int width, int height);
    void HandleCollision(Entity self, Entity other) override;
};
