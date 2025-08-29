#include "PlayerCollider.h"
#include <string>
// #include "BoxColliderComponent.h"

extern Coordinator gCoordinator;

PlayerCollider::PlayerCollider(const Vector2& pos, int width, int height, Entity player)
    : BoxColliderComponent(pos, width, height),
    mPlayer(player)
{}

void PlayerCollider::HandleCollision(Entity other)
{
    // SDL_Log("Handling player collision");
    auto& playerKinematics = gCoordinator.GetComponent<KinematicsComponent>(mPlayer);
    const auto& otherCollider = gCoordinator.GetComponent<BoxColliderComponent*>(other);

    switch (collisionSide)
    {
        case CollisionSide::BOTTOM:
            mCurrentGround = gCoordinator.GetComponent<BoxColliderComponent*>(other);
            mIsOnGround = true;
            playerKinematics.velocity.y = 0.f;
            playerKinematics.acceleration.y = 0.f;
            this->position.y = otherCollider->position.y - this->h;
            break;
        case CollisionSide::TOP:
            playerKinematics.velocity.y = 0.f;
            this->position.y = otherCollider->position.y + otherCollider->h;
            break;
        case CollisionSide::LEFT:
            this->position.x = otherCollider->position.x + otherCollider->w;
            break;
        case CollisionSide::RIGHT:
            this->position.x = otherCollider->position.x - this->w;
            break;
        case CollisionSide::NONE:
            // Case when you fall off edge
            if (mIsOnGround)
            {
                // Checking to see if player is still above platform with some tolerance
                const float tolerance = 2.0f;
                float platformLeft = mCurrentGround->position.x;
                float platformRight = mCurrentGround->position.x + mCurrentGround->w;
                float playerLeft = this->position.x;
                float playerRight = this->position.x + this->w;

                bool stillOnPlatform = (playerRight > platformLeft + tolerance) && (playerLeft < platformRight - tolerance);
                if (!stillOnPlatform)
                {
                    SDL_Log("Test");
                    mIsOnGround = false;

                    // Adding a little so they don't get stuck
                    // this->position.y = this->position.y + 1.f;
                    playerKinematics.acceleration.y = PhysicsSystem::kGravity;
                }
            }
            break;
    }
}


bool PlayerCollider::IsPlayerOnGround()
{
    return mIsOnGround;
}


void PlayerCollider::SetPlayerIsOnGround(bool isOnGround)
{
    mIsOnGround = isOnGround;
}
