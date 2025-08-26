#pragma once

#include "State.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ControllerComponent.h"
#include "../Components/KinematicsComponent.h"
#include "../Components/AnimationComponent.h"
// #include "../Systems/ScoreSystem.h"
#include "../Systems/ScrollSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/CollisionSystem.h"


class PlayState : public State
{
public:
    void Enter() override;
    void Exit() override;
    void HandleEvent(SDL_Event* e) override;
    void Update(float deltaTime) override;

private:
    void UpdatePlayer();

private:
    static constexpr float kDecreaseSpeedAmount = 75.f;
    Entity mPlayer;
    Entity mGroundCollider;
    Entity mBackground;
    Entity mSun;

    // std::shared_ptr<CollisionSystem> mCollisionSystem; //= gCoordinator.GetSystem<CollisionSystem>();
    // std::shared_ptr<ScrollSystem> mScrollSystem; //gCoordinator.GetSystem<ScrollSystem>();

    bool mIsSpaceHeldDown = false;
    bool mIsDownHeldDown = false;
    int mSpaceInputCount = 0;

};
