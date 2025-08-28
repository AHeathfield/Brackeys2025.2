#pragma once

#include "../Core/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/ControllerComponent.h"
#include "../Components/KinematicsComponent.h"
#include "PhysicsSystem.h"

// The collision system will update the entities transform
class CollisionSystem : public System
{
    // Invisibility frames
    static constexpr int kMaxIFrames = 60;

public:
    void Update(float deltaTime) override;
    void UpdateCollisions();
    void UpdateTransforms();

    void SetPlayerIsOnGround(bool isOnGround);
    bool IsPlayerOnGround();
    bool didPlayerHitWall();

private:
    void checkCollisionSide(const Vector2& aPrevPos, const BoxColliderComponent& aCollider, const BoxColliderComponent& bCollider);
    void checkEntityCollision(Entity entityA, BoxColliderComponent& colliderA);
    bool isCurrentGroundDefined();

private:
    bool mStopMoving = false;
    bool mIsOnGround = true;
    bool mCollisionRight = false;
    bool mCollisionLeft = false;
    bool mCollisionBottom = false;
    bool mCollisionTop = false;
    bool mCollisionGround = false;
    bool mIsInvincible = false;
    int mCurrentIFrames = 0;
    Vector2 mMoveMoves = Vector2();
    Entity mCurrentGround = MAX_ENTITIES + 1; // This is too indicate a null value
};
