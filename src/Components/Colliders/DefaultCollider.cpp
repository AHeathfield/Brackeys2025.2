#include "DefaultCollider.h"
#include "BoxColliderComponent.h"

extern Coordinator gCoordinator;

DefaultCollider::DefaultCollider(const Vector2& pos, int width, int height)
    : BoxColliderComponent(pos, width, height)
{}

void DefaultCollider::HandleCollision(Entity self, Entity other)
{
    // SDL_Log("Handling tile collision");
}
