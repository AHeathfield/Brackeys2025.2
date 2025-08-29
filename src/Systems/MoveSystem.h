#pragma once

#include "../Core/ECS.h"
// #include "../Components/MoveComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/Colliders/BoxColliderComponent.h"

// This will basically update the transforms of all entities with colliders and transform that can move (have kinematics)
class MoveSystem : public System
{
public:
    void Update(float deltaTime) override;
};
