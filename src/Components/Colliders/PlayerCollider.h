#pragma once

#include "BoxColliderComponent.h"
#include "../KinematicsComponent.h"
#include "../SolidComponent.h"
#include "../../Systems/PhysicsSystem.h"

class PlayerCollider : public BoxColliderComponent
{
public:
    void HandleCollision(Entity self, Entity other) override;

    PlayerCollider(const Vector2& pos, int width, int height, Entity player);
    bool IsPlayerOnGround();
    void SetPlayerIsOnGround(bool isOnGround);

private:
    Entity mPlayer;
    BoxColliderComponent* mCurrentGround;
    bool mIsOnGround = false;
};
