#include "TileCollider.h"
#include "BoxColliderComponent.h"

extern Coordinator gCoordinator;

TileCollider::TileCollider(const Vector2& pos, int width, int height)
    : BoxColliderComponent(pos, width, height)
{}

void TileCollider::HandleCollision(Entity other)
{
    // SDL_Log("Handling tile collision");
}
