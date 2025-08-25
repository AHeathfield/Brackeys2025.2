#pragma once

#include "../Core/ECS.h"
#include <SDL3/SDL_events.h>
#include <memory>

// #include "../Components/BoxColliderComponent.h"
#include "../Components/ControllerComponent.h"
#include "../Components/KinematicsComponent.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/ScrollSystem.h"

extern Coordinator gCoordinator;

class PlayerMovementSystem : public System
{
    static constexpr float kDecreaseSpeedAmount = 75.f;

public:
    void Update();
    void HandleEvent(SDL_Event e);

private:
    std::shared_ptr<CollisionSystem> collisionSystem = gCoordinator.GetSystem<CollisionSystem>();
    std::shared_ptr<ScrollSystem> scrollSystem = gCoordinator.GetSystem<ScrollSystem>();

    bool mIsSpaceHeldDown = false;
    bool mIsDownHeldDown = false;
    int mSpaceInputCount = 0;
};
