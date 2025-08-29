#pragma once

#include "BoxColliderComponent.h"
#include "../BiscuitComponent.h"
#include "../TextureComponent.h"

// I think this maybe shouldn't be it's own collider but it works I guess
class BiscuitCollider : public BoxColliderComponent
{
public:
    BiscuitCollider(const Vector2& pos, int width, int height);
    void HandleCollision(Entity self, Entity other) override;
};
