#pragma once

#include "BoxColliderComponent.h"

class TileCollider : public BoxColliderComponent
{
public:
    TileCollider(const Vector2& pos, int width, int height);
    void HandleCollision(Entity other) override;
};
