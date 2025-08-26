#pragma once

#include "../Core/ECS.h"
#include "../States/State.h"
#include "../States/TitleState.h"
#include "../Components/TransformComponent.h"
#include "../Biomes/Biome.h"
#include "../Biomes/StartingBiome.h"
#include "../Biomes/ForestBiome.h"
#include "../Biomes/CityBiome.h"
#include "../Biomes/DesertBiome.h"
#include "../Systems/CollisionSystem.h"
#include <memory>

extern Coordinator gCoordinator;

// Debating if I should just let the physics system handle the biomes entities, we could then just make this like the BiomeSystem
class ScrollSystem : public System
{
    static constexpr float kMaxSpeed = 10000.f;
    static constexpr float kStartSpeed = 5000.f;

public:
    void Init();
    void Update(float deltaTime) override;
    float GetScrollSpeed();
    void SetScrollSpeed(float speed);
    void DecreaseScrollSpeed(float speed);
    void Close();

private:
    void InitBiomes()
    {
        // So the biomes spawn beside eachother
        const float biomeLength = 7680;
        for (int i = 0; i < mNumOfBiomes; i++)
        {
            mBiomes[i]->mBiomePos = Vector2((i * biomeLength), 0.f);
            mBiomes[i]->Init();
        }
    }

private:
    std::vector<Biome*> mBiomes;
    std::shared_ptr<CollisionSystem> mCollisionSystem = gCoordinator.GetSystem<CollisionSystem>();
    int mNumOfBiomes = 4;
    float mScrollSpeed = 1.f; // TODO: Make 0
};
