#pragma once

#include "../Core/ECS.h"
#include "../Core/Vector2.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/RenderSystem.h"
#include <vector>

extern Coordinator gCoordinator;

class Biome
{
public:
    virtual void Init() = 0;
    // virtual void Update() = 0;
    virtual void Grow() = 0;

    // Moves the entire biome, will need to move colliders too!!
    void MoveBiome(float x, float y)
    {
        mBiomePos = mBiomePos + Vector2(x, y);

        for (const auto& entity : mBiomeEntities)
        {
            // Updates all the colliders, the transforms update based on the colliders after a collision check
            if (gCoordinator.HasComponent<BoxColliderComponent>(entity))
            {
                auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
                collider.position.x += x;
                collider.position.y += y;
            }
            // When there is no collider like for the ground, we still want to move them so we just update transform
            else
            {
                auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
                transform.position.x += x;
                transform.position.y += y;
            }
        }
    }

    void SetBiomePosition(const Vector2& newPos)
    {
        Vector2 moveVector = newPos - mBiomePos;
        MoveBiome(moveVector.x, moveVector.y);
    }


    std::shared_ptr<RenderSystem> mRenderSystem = gCoordinator.GetSystem<RenderSystem>();   // Used for growing
    Vector2 mBiomePos = Vector2();   // This is the reference of the biome
    std::vector<Entity> mBiomeEntities;
    int mBiomeLength = 7680; // TODO: If we make the biomes different sizes, we should make virtual method called GetLength()
};
