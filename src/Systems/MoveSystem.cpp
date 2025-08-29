#include "MoveSystem.h"

extern Coordinator gCoordinator;

void MoveSystem::Update(float deltaTime)
{
    for (const auto& entity : mEntities)
    {
        auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
        auto& collider = gCoordinator.GetComponent<BoxColliderComponent*>(entity);

        if (collider->isColliding())
        {
            // collider->handleCollision()
        }

        transform.position = collider->position;
    }
}
