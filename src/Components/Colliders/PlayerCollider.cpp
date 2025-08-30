#include "PlayerCollider.h"
#include "BoxColliderComponent.h"
#include <string>
// #include "BoxColliderComponent.h"

extern Coordinator gCoordinator;
extern State* gCurrentState;

PlayerCollider::PlayerCollider(const Vector2& pos, int width, int height)
    : BoxColliderComponent(pos, width, height)
{}

void PlayerCollider::HandleCollision(Entity self, Entity other)
{
    // SDL_Log("Handling player collision");
    auto& playerKinematics = gCoordinator.GetComponent<KinematicsComponent>(self);
    const auto& otherCollider = gCoordinator.GetComponent<BoxColliderComponent*>(other);

    if (gCoordinator.HasComponent<SolidComponent>(other))
    {
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
                        // this->position.y = this->position.y + 5.f;
                        playerKinematics.acceleration.y = PhysicsSystem::kGravity;
                    }
                }
                break;
        }
    }

    else if (gCoordinator.HasComponent<StateChangerComponent>(other) && this->collisionSide != CollisionSide::NONE)
    {
        // Probably run some animation, have a timer or something then change state, but for now it will just instant change state
        auto& stateChanger = gCoordinator.GetComponent<StateChangerComponent>(other);
        
        if (stateChanger.newState != nullptr)
        {
            gCurrentState = stateChanger.newState;
        }
        else
        {
            SDL_Log("Cannot change to null state");
        }
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
