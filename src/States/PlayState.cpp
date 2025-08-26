#include "PlayState.h"
#include "State.h"
#include <SDL3/SDL_rect.h>
#include <unordered_map>

extern Coordinator gCoordinator;

void PlayState::Enter()
{
    const float screenHeight = 1080.f;
    const float screenWidth = 1920.f;

    // Initializing the ScrollSystem
    auto mScrollSystem = gCoordinator.GetSystem<ScrollSystem>();
    mScrollSystem->Init();

    // The player
    mPlayer = gCoordinator.CreateEntity();
    std::unordered_map<std::string, Animation> playerAnimations;
    Animation run = {
        .numberOfImages = 12,
        .columnsPerRow = 8,
        .clipWidth = 64.f,
        .clipHeight = 64.f,
        .animationTime = 0.6f,
        .startClipPos = Vector2(),
        .isPlaying = true,
        .isLooping = true
    };
    playerAnimations.insert({"Run", run});

    gCoordinator.AddComponent(
            mPlayer,
            TransformComponent{
                .position = Vector2((screenWidth / 2) - 32.f, screenHeight - 334.f),
            });
    gCoordinator.AddComponent(
            mPlayer,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 64.f, 64.f},
                .path = "src/Assets/RunSpriteSheetTEST.png",
                .depth = 10
            });
    gCoordinator.AddComponent(
            mPlayer,
            BoxColliderComponent{
                .position = Vector2((screenWidth / 2) - 32.f, screenHeight - 334.f),
                .w = 64,
                .h = 64
            });
    gCoordinator.AddComponent(
            mPlayer,
            KinematicsComponent{
            });
    gCoordinator.AddComponent(
            mPlayer,
            AnimationComponent{
                .animations = playerAnimations
            });
    gCoordinator.AddComponent(
            mPlayer,
            ControllerComponent{
            });



    mBackground = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBackground,
            TransformComponent{
                .position = Vector2(),
            });
    gCoordinator.AddComponent(
            mBackground,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/Background.png",
                .depth = -1
            });


    mGroundCollider = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mGroundCollider,
            TransformComponent{
                .position = Vector2((screenWidth / 2) - 135.f, screenHeight - 270.f),
            });
    // FOR TESTING
    // gCoordinator.AddComponent(
    //         mGroundCollider,
    //         TextureComponent{
    //             .texture = nullptr,
    //             .spriteClip = SDL_FRect{0.f, 0.f, 270.f, 270.f},
    //             .path = "src/Assets/debug.png",
    //         });
    gCoordinator.AddComponent(
            mGroundCollider,
            BoxColliderComponent{
                .position = Vector2((screenWidth / 2) - 135.f, screenHeight - 270.f),
                .w = 270,
                .h = 1000 // For good measure :)
            });

    // The player
    mSun = gCoordinator.CreateEntity();
    std::unordered_map<std::string, Animation> sunAnimations;
    Animation smile = {
        .numberOfImages = 360,
        .columnsPerRow = 18,
        .clipWidth = 240.f,
        .clipHeight = 240.f,
        .animationTime = 30.f,
        .startClipPos = Vector2(),
        .isPlaying = true,
        .isLooping = true
    };
    sunAnimations.insert({"Smile", smile});

    gCoordinator.AddComponent(
            mSun,
            TransformComponent{
                .position = Vector2((screenWidth / 2) - (270 / 2),  334.f),
            });
    gCoordinator.AddComponent(
            mSun,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 240.f, 240.f},
                .path = "src/Assets/SunSmileSpriteSheet_240x240_12FPS.png",
                .depth = 9
            });
    gCoordinator.AddComponent(
            mSun,
            AnimationComponent{
                .animations = sunAnimations
            });

    // Setting up update order
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<ScrollSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<PhysicsSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<CollisionSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<AnimationSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<RenderSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<AudioSystem>());
}

void PlayState::Exit()
{
    // Clear the update order
    mSystemUpdateOrder.clear();

    // Close the Scroll System
    auto mScrollSystem = gCoordinator.GetSystem<ScrollSystem>();
    auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
    mScrollSystem->Close();

    // Destroy the Texture components
    auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mPlayer);
    textureComponent.destroy();

    // textureComponent = gCoordinator.GetComponent<TextureComponent>(mBackground);
    // textureComponent.destroy();

    gCoordinator.DestroyEntity(mPlayer);
    gCoordinator.DestroyEntity(mGroundCollider);
    // gCoordinator.DestroyEntity(mBackground);
}


void PlayState::HandleEvent(SDL_Event* e)
{
    if (e->type == SDL_EVENT_KEY_DOWN)
    {
        if (e->key.key == SDLK_SPACE)
        {
            // std::string log = "Count: " + std::to_string(mSpaceInputCount);
            // SDL_Log(log.c_str());

            // Initial Jump
            // Should be and is on Ground
            if (mSpaceInputCount == 0)
            {
                // NOTE: positive y is downwards
                auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(mPlayer);

                SDL_Log("Initial Jump");
                kinematics.velocity.y = -1250.f;
                kinematics.acceleration.y = PhysicsSystem::kGravity;

                auto collisionSystem = gCoordinator.GetSystem<CollisionSystem>();
                collisionSystem->SetPlayerIsOnGround(false);
            }

            // So it only recognizes 10 extra inputs
            if (mSpaceInputCount < 10)
            {
                mSpaceInputCount++;
            }
            mIsSpaceHeldDown = true;
        }

        // FAST FALL
        if (!mIsDownHeldDown && e->key.key == SDLK_DOWN)
        {
            mIsDownHeldDown = true;
            auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(mPlayer);
            kinematics.velocity.y = 2000.f;
        }

        auto scrollSystem = gCoordinator.GetSystem<ScrollSystem>();
        // For debugging
        if (e->key.key == SDLK_LEFT)
        {
            scrollSystem->SetScrollSpeed(200.f);
        }
        if (e->key.key == SDLK_RIGHT)
        {
            scrollSystem->SetScrollSpeed(2500.f);
        }

    }

    // No key is down
    else if (e->type == SDL_EVENT_KEY_UP)
    {
        if (e->key.key == SDLK_SPACE)
        {
            mIsSpaceHeldDown = false;
        }
        if (e->key.key == SDLK_DOWN)
        {
            mIsDownHeldDown = false;
        }
    }


    // Updating Player
    // UpdatePlayer();
}


// In future I think it should call Update for the necessary systems
void PlayState::Update(float deltaTime)
{
    auto collisionSystem = gCoordinator.GetSystem<CollisionSystem>();
    // To make jump go higher when space is held
    if (mIsSpaceHeldDown && mSpaceInputCount > 0 && mSpaceInputCount < 10)
    {
        auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(mPlayer);
        
        // Only apply on the way up not when falling
        if (kinematics.velocity.y < 0.f)
        {
            kinematics.velocity.y -= 10.f;
        }
    }

    // When jump is complete and when your back on the ground
    else if (!mIsSpaceHeldDown && mSpaceInputCount > 0 && collisionSystem->IsPlayerOnGround())
    {
        mSpaceInputCount = 0;
    }

    // If player is not on ground
    if (!collisionSystem->IsPlayerOnGround())
    {
        auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(mPlayer);
        kinematics.acceleration.y = PhysicsSystem::kGravity;
    }

    // If player hit a wall
    if (collisionSystem->didPlayerHitWall() == true)
    {
        SDL_Log("Slowing player down");
        auto scrollSystem = gCoordinator.GetSystem<ScrollSystem>();
        float scrollSpeed = scrollSystem->GetScrollSpeed();
        // It halfs it plus a lil more
        scrollSystem->DecreaseScrollSpeed((scrollSpeed / 2) + kDecreaseSpeedAmount);
    }


    // Updating systems
    for (int i = 0; i < mSystemUpdateOrder.size(); i++)
    {
        mSystemUpdateOrder[i]->Update(deltaTime);
    }
}
