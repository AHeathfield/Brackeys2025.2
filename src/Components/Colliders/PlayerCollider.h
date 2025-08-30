#pragma once

#include "BoxColliderComponent.h"
#include "../KinematicsComponent.h"
#include "../SolidComponent.h"
#include "../StateChangerComponent.h"
#include "../../Systems/PhysicsSystem.h"

class PlayerCollider : public BoxColliderComponent
{
public:
    void HandleCollision(Entity self, Entity other) override;

    PlayerCollider(const Vector2& pos, int width, int height);
    bool IsPlayerOnGround();
    void SetPlayerIsOnGround(bool isOnGround);

private:
    // Entity mPlayer; // Probably don't need this since we have self, if any problems occur it's an easy change
    BoxColliderComponent* mCurrentGround;
    bool mIsOnGround = false;
};
