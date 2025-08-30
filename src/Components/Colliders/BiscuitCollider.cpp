#include "BiscuitCollider.h"
#include "BoxColliderComponent.h"
// #include "BoxColliderComponent.h"

extern Coordinator gCoordinator;

BiscuitCollider::BiscuitCollider(const Vector2& pos, int width, int height)
    : BoxColliderComponent(pos, width, height)
{}

void BiscuitCollider::HandleCollision(Entity self, Entity other)
{
    // Player will have biscuit component
    if (this->collisionSide != CollisionSide::NONE)
    {
        if (gCoordinator.HasComponent<BiscuitComponent>(other))
        {
            gCoordinator.GetComponent<BiscuitComponent>(other).biscuitsCollected += 1;
            
            // Disabling since when someone dies I don't want to reload the entire scene atleast for now
            gCoordinator.RemoveComponent<TextureComponent>(self);
            this->isActive = false;
        }
    }
}
