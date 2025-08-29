#include "CollisionSystem.h"
// #include "PhysicsSystem.h"
// #include "PlayerMovementSystem.h"
// #include "ScrollSystem.h"
#include <SDL3/SDL_log.h>
#include <cstdlib>
#include <string>

extern Coordinator gCoordinator;

void CollisionSystem::Update(float deltaTime)
{
    UpdateCollisions();
    // UpdateTransforms(); REPLACING with MoveSystem which will update the transforms of all entities that can move
}

void CollisionSystem::UpdateCollisions()
{
    std::set<Entity> playerEntities;
    mMoveMoves = Vector2();
    mStopMoving = false;
    mCollisionBottom = false;
    mCollisionTop = false;
    mCollisionRight = false;
    mCollisionLeft = false;

    // Handling IFrames
    // if (mIsInvincible && mCurrentIFrames < kMaxIFrames)
    // {
    //     mCurrentIFrames++;
    // }
    // else
    // {
    //     mCurrentIFrames = 0;
    //     mIsInvincible = false;
    // }

    for (const auto& entityA : mEntities)
    {
        // This is to check for the player, we know player entity has the controller component (currently it is the only entity
        // with it, if this changes we can just make a PlayerComponent or smt)
        if (gCoordinator.HasComponent<ControllerComponent>(entityA))
        {
            playerEntities.insert(entityA);

            auto& colliderA = gCoordinator.GetComponent<BoxColliderComponent*>(entityA);
            checkEntityCollision(entityA, colliderA);
        }
    }

    // Update the components
    // NOTE: playerEntities for now will just be the player entity however this could change if lets say he grabs a pogo stick or smt...
    // for (const auto& entity : playerEntities)
    // {
    //     auto& collider = gCoordinator.GetComponent<BoxColliderComponent*>(entity);
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
    // mCollisionGround = false;
}

// void CollisionSystem::UpdateTransforms()
// {
//     for (const auto& entity : mEntities)
//     {
//         auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
//         const auto& collider = gCoordinator.GetComponent<BoxColliderComponent*>(entity);
//         transform.position = collider->position;
//     }
// }

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
void CollisionSystem::checkCollisionSide(const Vector2& aPrevPos, const BoxColliderComponent* aCollider, const BoxColliderComponent* bCollider)
{
    // TODO: GET RID OF THIS, I DON'T THINK WE NEED ANYMORE
    // Check to see if it's ground collision
    // const int screenHeight = 1080;
    // const int groundHeight = 270;
    // if (bCollider->position.y == (screenHeight - groundHeight))
    // {
    //     mCollisionBottom = true;
    //     mCollisionGround = true;
    // }

    // Obstacle collisions
    // else
    // {
    // Swept AABB detection
    float overlapX = std::min(aCollider->position.x + aCollider->w, bCollider->position.x + bCollider->w) - std::max(aCollider->position.x, bCollider->position.x);
    float overlapY = std::min(aCollider->position.y + aCollider->h, bCollider->position.y + bCollider->h) - std::max(aCollider->position.y, bCollider->position.y);

    // Reslove axis with smallest overlap
    if (overlapX < overlapY)
    {
        if (aCollider->position.x < bCollider->position.x)
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
        if (aCollider->position.y < bCollider->position.y)
        {
            mCollisionBottom = true;
        }
        else
        {
            mCollisionTop = true;
        }
    }
    // }
}

// Helper function
bool checkCollision(const BoxColliderComponent* a, const BoxColliderComponent* b)
{
    // To help with inaccuracies
    // float spacer = 2.f;

    //Calculate the sides of rect A
    int aMinX = a->position.x;
    int aMaxX = a->position.x + a->w;
    int aMinY = a->position.y;
    int aMaxY = a->position.y + a->h;

    //Calculate the sides of rect B
    int bMinX = b->position.x;
    int bMaxX = b->position.x + b->w;
    int bMinY = b->position.y;
    int bMaxY = b->position.y + b->h;

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


void CollisionSystem::checkEntityCollision(Entity entityA, BoxColliderComponent* colliderA)
{
    auto& transformA = gCoordinator.GetComponent<TransformComponent>(entityA);
    auto& kinematicsA = gCoordinator.GetComponent<KinematicsComponent>(entityA);

    for (const auto& entityB : mEntities)
    {
        colliderA->resetCollisionSide();
        if (entityA != entityB)
        {
            auto& colliderB = gCoordinator.GetComponent<BoxColliderComponent*>(entityB);
            colliderB->resetCollisionSide();
            
            if (checkCollision(colliderA, colliderB))
            {
                // SDL_Log("COLLISION");
                // entityA has to have kinematics I think...
                checkCollisionSide(transformA.position, colliderA, colliderB);

                if (mCollisionBottom)
                {
                    colliderA->collisionSide = CollisionSide::BOTTOM;
                    colliderB->collisionSide = CollisionSide::TOP;
                    // mCurrentGround = entityB;
                    // mIsOnGround = true;

                    // This is to help with the teleport up bug
                    // int leverage = 40.f;
                    // if (colliderA->position.y + colliderA->h < colliderB->position.y + leverage)
                    // {
                    //     mCurrentGround = entityB;
                    //     mIsOnGround = true;
                    //     // kinematicsA.velocity.y = 0.f;
                    //     // kinematicsA.acceleration.y = 0.f;
                    //     // colliderA->position.y = colliderB->position.y - colliderA->h;
                    // }
                }
                else if (mCollisionTop)
                {
                    colliderA->collisionSide = CollisionSide::TOP;
                    colliderB->collisionSide = CollisionSide::BOTTOM;
                }

                // Side collisions
                else if (mCollisionRight)
                {
                    colliderA->collisionSide = CollisionSide::RIGHT;
                    colliderB->collisionSide = CollisionSide::LEFT;
                }
                else if (mCollisionLeft)
                {
                    colliderA->collisionSide = CollisionSide::LEFT;
                    colliderB->collisionSide = CollisionSide::RIGHT;
                }

            }

            // Handling The Collisions for both entities (even if none)
            colliderA->HandleCollision(entityB);
            colliderB->HandleCollision(entityA);

            // No collision and in the air (scenario when you fall off an edge)
            // else if (mIsOnGround && isCurrentGroundDefined())
            // {
            //     // SDL_Log("No Collision");
            //     // When we restart game we need to reset this
            //     if (gCoordinator.HasComponent<BoxColliderComponent*>(mCurrentGround))
            //     {
            //         const auto& groundCollider = gCoordinator.GetComponent<BoxColliderComponent*>(mCurrentGround);
            //         if (groundCollider->position.x + groundCollider->w < colliderA->position.x || groundCollider->position.x > colliderA->position.x + colliderA->w)
            //         {
            //             mIsOnGround = false;
            //             mMoveMoves.y = -1.f;
            //
            //             kinematicsA.acceleration.y = PhysicsSystem::kGravity;
            //         }
            //     }
            //     else
            //     {
            //         mCurrentGround = MAX_ENTITIES + 1;
            //     }
            // }
        }
    }
}

