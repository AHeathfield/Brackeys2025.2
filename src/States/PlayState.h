#pragma once

#include "State.h"
#include "TitleState.h"

#include "../Core/Camera.h"

#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BiscuitComponent.h"
#include "../Components/Colliders/BoxColliderComponent.h"
#include "../Components/Colliders/PlayerCollider.h"
#include "../Components/Colliders/DefaultCollider.h"
#include "../Components/Colliders/BiscuitCollider.h"
#include "../Components/ControllerComponent.h"
#include "../Components/KinematicsComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/HurtComponent.h"
#include "../Components/CollectibleComponent.h"
#include "../Components/StateChangerComponent.h"

#include "../Systems/RenderSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/MoveSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/AudioSystem.h"


class PlayState : public State
{
    static constexpr float kPlayerMoveSpeed = 200.f;
public:
    void Enter() override;
    void Exit() override;
    void HandleEvent(SDL_Event* e) override;
    void Update(float deltaTime) override;

private:
    void UpdatePlayer();

private:
    Entity mPlayer;
    Entity mBiscuit;
    Entity mHome;
    Entity mGround;
    Entity mBackground;
    Entity mTestObject;
    Entity mTestObjectAir;
    std::vector<std::shared_ptr<System>> mSystemUpdateOrder;

    // I 
    // std::shared_ptr<CollisionSystem> mCollisionSystem; //= gCoordinator.GetSystem<CollisionSystem>() 
    // std::shared_ptr<ScrollSystem> mScrollSystem; //gCoordinator.GetSystem<ScrollSystem>();

    bool mIsSpaceHeldDown = false;
    bool mIsDownHeldDown = false;
    bool mIsAHeldDown = false;
    bool mIsDHeldDown = false;
    int mSpaceInputCount = 0;

};
