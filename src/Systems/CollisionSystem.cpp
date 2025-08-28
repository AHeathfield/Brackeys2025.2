#include "CollisionSystem.h"
// #include "PhysicsSystem.h"
// #include "PlayerMovementSystem.h"
// #include "ScrollSystem.h"
#include <SDL3/SDL_log.h>
#include <string>

extern Coordinator gCoordinator;

void CollisionSystem::Update(float deltaTime)
{
    UpdateCollisions();
    UpdateTransforms();
}

void CollisionSystem::UpdateCollisions()
{
    std::set<Entity> playerEntities;
    mMoveMoves = Vector2();
    mStopMoving = false;
    mCollisionBottom = false;
    mCollisionTop = false;
    mCollisionRight = false;

    // Handling IFrames
    if (mIsInvincible && mCurrentIFrames < kMaxIFrames)
    {
        mCurrentIFrames++;
    }
    else
    {
        mCurrentIFrames = 0;
        mIsInvincible = false;
    }

    for (const auto& entityA : mEntities)
    {
        // This is to check for the player, we know player entity has the controller component (currently it is the only entity
        // with it, if this changes we can just make a PlayerComponent or smt)
        if (gCoordinator.HasComponent<ControllerComponent>(entityA))
        {
            playerEntities.insert(entityA);

            auto& colliderA = gCoordinator.GetComponent<BoxColliderComponent>(entityA);
            checkEntityCollision(entityA, colliderA);
        }
    }

    // Update the components
    // NOTE: playerEntities for now will just be the player entity however this could change if lets say he grabs a pogo stick or smt...
    // for (const auto& entity : playerEntities)
    // {
    //     auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
    //
    //     // YES THIS IS HARDCODED TO SOLVE A BUG LOL
    //     if (mCollisionGround)
    //     {
    //         // 40 is player height
    //         collider.position.y = 1080 - 270 - collider.h;
    //     }
    //     else
    //     {
    //         collider.position = collider.position + mMoveMoves;
    //         if (mStopMoving)
    //         {
    //             gCoordinator.RemoveComponent<ControllerComponent>(entity);
    //         }
    //     }
    // }
    mCollisionGround = false;
}

void CollisionSystem::UpdateTransforms()
{
    for (const auto& entity : mEntities)
    {
        auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
        transform.position = collider.position;
    }
}

bool CollisionSystem::IsPlayerOnGround()
{
    return mIsOnGround;
}

void CollisionSystem::SetPlayerIsOnGround(bool isOnGround)
{
    mIsOnGround = isOnGround;
}

bool CollisionSystem::didPlayerHitWall()
{
    return mCollisionRight || mCollisionLeft;
}


// Private methods + Helper Functions

// Going to use swept AABB
void CollisionSystem::checkCollisionSide(const Vector2& aPrevPos, const BoxColliderComponent& aCollider, const BoxColliderComponent& bCollider)
{
    // Check to see if it's ground collision
    const int screenHeight = 1080;
    const int groundHeight = 270;
    if (bCollider.position.y == (screenHeight - groundHeight))
    {
        mCollisionBottom = true;
        mCollisionGround = true;
    }

    // Obstacle collisions
    else
    {
        // Old collision
        // const float leverageY = 50.f; // Used to make collisions easier
        // const float leverageX = 5.f;
        // if (aPrevPos.y + aCollider.h <= bCollider.position.y && aCollider.position.x + leverageY >= bCollider.position.x)
        // {
        //     mCollisionBottom = true;
        // }
        // else if (aPrevPos.y >= bCollider.position.y + bCollider.h && aCollider.position.x >= bCollider.position.x)
        // {
        //     mCollisionTop = true;
        // }
        // if (aPrevPos.x + aCollider.w <= bCollider.position.x + leverageX && aCollider.position.y + aCollider.h >= bCollider.position.y)
        // {
        //     mCollisionRight = true;
        // }

        // Swept AABB detection
        float overlapX = std::min(aCollider.position.x + aCollider.w, bCollider.position.x + bCollider.w) - std::max(aCollider.position.x, bCollider.position.x);
        float overlapY = std::min(aCollider.position.y + aCollider.h, bCollider.position.y + bCollider.h) - std::max(aCollider.position.y, bCollider.position.y);

        // Reslove axis with smallest overlap
        if (overlapX < overlapY)
        {
            if (aCollider.position.x < bCollider.position.x)
            {
                mCollisionRight = true;
            }
            else
            {
                mCollisionLeft = true;
            }
        }
        else
        {
            if (aCollider.position.y < bCollider.position.y)
            {
                mCollisionBottom = true;
            }
            else
            {
                mCollisionTop = true;
            }
        }
    }
}

// Helper function
bool checkCollision(const BoxColliderComponent& a, const BoxColliderComponent& b)
{
    // To help with inaccuracies
    // float spacer = 2.f;

    //Calculate the sides of rect A
    int aMinX = a.position.x;
    int aMaxX = a.position.x + a.w;
    int aMinY = a.position.y;
    int aMaxY = a.position.y + a.h;

    //Calculate the sides of rect B
    int bMinX = b.position.x;
    int bMaxX = b.position.x + b.w;
    int bMinY = b.position.y;
    int bMaxY = b.position.y + b.h;

    //If left side of A is the right of B
    if( aMinX >= bMaxX )
    {
        return false;
    }

    //If the right side of A to the left of B
    if( aMaxX <= bMinX )
    {
        return false;
    }

    //If the top side of A is below bottom of B
    if( aMinY >= bMaxY)
    {
        return false;
    }

    //If the bottom side of A is above top of B
    if( aMaxY <= bMinY)
    {
        return false;
    }

    return true;
}

// Private method to see if mCurrentGround is actually defined
bool CollisionSystem::isCurrentGroundDefined()
{
    return mCurrentGround <= MAX_ENTITIES;
}


bool isColliderBClose(const BoxColliderComponent& colliderB)
{
    return (colliderB.position.x + colliderB.w >= 0.f) && (colliderB.position.x <= (1920.f / 2) + 100.f);
}


void CollisionSystem::checkEntityCollision(Entity entityA, BoxColliderComponent& colliderA)
{
    auto& transformA = gCoordinator.GetComponent<TransformComponent>(entityA);
    auto& kinematicsA = gCoordinator.GetComponent<KinematicsComponent>(entityA);

    for (const auto& entityB : mEntities)
    {
        if (entityA != entityB)
        {
            const auto& colliderB = gCoordinator.GetComponent<BoxColliderComponent>(entityB);
            
            if (isColliderBClose(colliderB))
            {
                if (checkCollision(colliderA, colliderB))
                {
                    // SDL_Log("COLLISION");
                    // entityA has to have kinematics I think...
                    checkCollisionSide(transformA.position, colliderA, colliderB);

                    // TODO: Need to add the logic for like slowing down
                    if (mCollisionBottom)
                    {
                        // SDL_Log("BOTTOM COLLISION");

                        // This is to help with the teleport up bug
                        int leverage = 40.f;
                        if (colliderA.position.y + colliderA.h < colliderB.position.y + leverage)
                        {
                            mCurrentGround = entityB;
                            mIsOnGround = true;
                            // mMoveMoves.y = -1 * (colliderA.position.y + colliderA.h - colliderB.position.y) - 1.f;
                            kinematicsA.velocity.y = 0.f;
                            kinematicsA.acceleration.y = 0.f;
                            colliderA.position.y = colliderB.position.y - colliderA.h;
                        }
                    }
                    else if (mCollisionTop)
                    {
                        // SDL_Log("TOP COLLISION");
                        // mMoveMoves.y = ((colliderB.position.y + colliderB.h) - colliderA.position.y) + 1.f;
                        kinematicsA.velocity.y = 0.f;
                        colliderA.position.y = colliderB.position.y + colliderB.h;
                    }

                    // Side collisions
                    else if (mCollisionRight)
                    {
                        mIsInvincible = true;
                        // mMoveMoves.x = -1 * (colliderA.position.x + colliderA.w - colliderB.position.x) - 1.f;
                        colliderA.position.x = colliderB.position.x - colliderA.w;
                    }
                    else if (mCollisionLeft)
                    {
                        // mMoveMoves.x = (colliderB.position.x + colliderB.w - colliderA.position.x) + 1.f;
                        colliderA.position.x = colliderB.position.x + colliderB.w;
                    }
                }
                // No collision and in the air (scenario when you fall off an edge)
                else if (mIsOnGround && isCurrentGroundDefined())
                {
                    // SDL_Log("No Collision");
                    // When we restart game we need to reset this
                    if (gCoordinator.HasComponent<BoxColliderComponent>(mCurrentGround))
                    {
                        const auto& groundCollider = gCoordinator.GetComponent<BoxColliderComponent>(mCurrentGround);
                        if (groundCollider.position.x + groundCollider.w < colliderA.position.x || groundCollider.position.x > colliderA.position.x + colliderA.w)
                        {
                            mIsOnGround = false;
                            mMoveMoves.y = -1.f;

                            kinematicsA.acceleration.y = PhysicsSystem::kGravity;
                        }
                    }
                    else
                    {
                        mCurrentGround = MAX_ENTITIES + 1;
                    }
                }
            }
        }
    }
}

