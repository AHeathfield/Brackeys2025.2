#include "PlayerMovementSystem.h"
#include "PhysicsSystem.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <string>

extern Coordinator gCoordinator;

void PlayerMovementSystem::HandleEvent(SDL_Event e)
{
    if (e.type == SDL_EVENT_KEY_DOWN)
    {
        if (e.key.key == SDLK_SPACE)
        {
            // std::string log = "Count: " + std::to_string(mSpaceInputCount);
            // SDL_Log(log.c_str());

            // Initial Jump
            // Should be and is on Ground
            if (mSpaceInputCount == 0)
            {
                for (const auto& entity : mEntities)
                {
                    // NOTE: positive y is downwards
                    auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(entity);

                    SDL_Log("Initial Jump");
                    kinematics.velocity.y = -1250.f;
                    kinematics.acceleration.y = PhysicsSystem::kGravity;
                    collisionSystem->SetPlayerIsOnGround(false);
                }
            }

            // So it only recognizes 10 extra inputs
            if (mSpaceInputCount < 10)
            {
                mSpaceInputCount++;
            }
            mIsSpaceHeldDown = true;
        }

        // FAST FALL
        if (!mIsDownHeldDown && e.key.key == SDLK_DOWN)
        {
            mIsDownHeldDown = true;
            for (const auto& entity : mEntities)
            {
                auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(entity);
                kinematics.velocity.y = 2000.f;
            }
        }

        if (e.key.key == SDLK_LEFT)
        {
            scrollSystem->SetScrollSpeed(200.f);
        }
        if (e.key.key == SDLK_RIGHT)
        {
            scrollSystem->SetScrollSpeed(2500.f);
        }

    }

    // No key is down
    else if (e.type == SDL_EVENT_KEY_UP)
    {
        if (e.key.key == SDLK_SPACE)
        {
            mIsSpaceHeldDown = false;
        }
        if (e.key.key == SDLK_DOWN)
        {
            mIsDownHeldDown = false;
        }
    }
}


void PlayerMovementSystem::Update(float deltaTime)
{
    // To make jump go higher when space is held
    if (mIsSpaceHeldDown && mSpaceInputCount > 0 && mSpaceInputCount < 10)
    {
        for (const auto& entity : mEntities)
        {
            auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(entity);
            
            // Only apply on the way up not when falling
            if (kinematics.velocity.y < 0.f)
            {
                kinematics.velocity.y -= 10.f;
            }
        }
    }

    // When jump is complete and when your back on the ground
    else if (!mIsSpaceHeldDown && mSpaceInputCount > 0 && collisionSystem->IsPlayerOnGround())
    {
        mSpaceInputCount = 0;
    }

    // If player is not on ground
    if (!collisionSystem->IsPlayerOnGround())
    {
        for (const auto& entity : mEntities)
        {
            auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(entity);
            kinematics.acceleration.y = PhysicsSystem::kGravity;
        }
    }

    // If player hit a wall
    if (collisionSystem->didPlayerHitWall() == true)
    {
        float scrollSpeed = scrollSystem->GetScrollSpeed();
        // It halfs it plus a lil more
        scrollSystem->DecreaseScrollSpeed((scrollSpeed / 2) + kDecreaseSpeedAmount);
    }
}
