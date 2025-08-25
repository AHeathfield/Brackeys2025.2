#include "ScrollSystem.h"
#include <SDL3/SDL_log.h>
#include <cstdlib>
#include <string>

extern Coordinator gCoordinator;
extern State* gCurrentState;

void ScrollSystem::Init()
{
    // Setting the start speed
    mScrollSpeed = kStartSpeed;

    // Creating the array of biomes
    int biomeNum = 0;
    mBiomes.push_back(new StartingBiome); // Starting Biome is just a run way
    while (mBiomes.size() < mNumOfBiomes)
    {
        // SDL_Log("Creating Biome");
        // SHOULD BE RANDOM BUT FOR TESTING IM HARDCODING IT
        // biomeNum = rand() % numOfTypesOfBiomes;

        // TODO: Remove this
        biomeNum = mBiomes.size() - 1;

        switch (biomeNum)
        {
            case 0:
                mBiomes.push_back(new ForestBiome);
                break;
            case 1:
                mBiomes.push_back(new CityBiome);
                break;
            case 2:
                mBiomes.push_back(new DesertBiome);
                break;
        }
    }

    InitBiomes();
}

void ScrollSystem::Update(float deltaTime)
{
    // Check if player loss
    if (mScrollSpeed == 0.f)
    {
        SDL_Log("YOU DIED");
        gCurrentState = new TitleState();
    }

    // Increases speed when player is on ground
    if (mCollisionSystem->IsPlayerOnGround())
    {
        if (mScrollSpeed < kMaxSpeed)
        {
            mScrollSpeed += 1.f;
        }
        // std::string log = "Current Speed: " + std::to_string(mScrollSpeed);
        // SDL_Log(log.c_str());
    }

    // Moving Biomes to the left
    for (const auto& biome : mBiomes)
    {
        if (biome->mBiomePos.x <= (biome->mBiomeLength * -1))
        {
            float newXPos = (mNumOfBiomes - 1) * biome->mBiomeLength + (biome->mBiomePos.x - biome->mBiomeLength * -1);
            Vector2 lastBiomePos = Vector2(newXPos, 0.f);
            biome->SetBiomePosition(lastBiomePos);

            // Then we want to update the biome by growing it
            biome->Grow();
        }
        biome->MoveBiome(mScrollSpeed * -1 * deltaTime, 0.f);
    }
}

void ScrollSystem::Close()
{
    // I guess this is where we should play curtains animation

    // Deleting all entities in biomes
    for (const auto& biome : mBiomes)
    {
        for (const auto& entity : biome->mBiomeEntities)
        {
            if (gCoordinator.HasComponent<TextureComponent>(entity))
            {
                auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);
                textureComponent.destroy();
            }
            gCoordinator.DestroyEntity(entity);
        }
    }
    
    // Removing all the Biomes
    mBiomes.clear();

    // SDL_Log("Done closing scroll");
}

float ScrollSystem::GetScrollSpeed()
{
    return mScrollSpeed;
}

void ScrollSystem::SetScrollSpeed(float speed)
{
    mScrollSpeed = speed;
}

void ScrollSystem::DecreaseScrollSpeed(float speed)
{
    mScrollSpeed -= speed;
    if (mScrollSpeed < 0.f)
    {
        mScrollSpeed = 0.f;
    }
}
