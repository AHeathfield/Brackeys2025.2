#include "PlayState.h"
#include "State.h"
#include <SDL3/SDL_rect.h>
#include <unordered_map>

extern Coordinator gCoordinator;
extern Vector2 gScreenSize;

void PlayState::Enter()
{
    const float screenHeight = 1080.f;
    const float screenWidth = 1920.f;

    // Initializing the ScrollSystem
    // auto mScrollSystem = gCoordinator.GetSystem<ScrollSystem>();
    // mScrollSystem->Init();

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


    mGround = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mGround,
            TransformComponent{
                .position = Vector2(0.f, screenHeight - 270.f),
            });
    gCoordinator.AddComponent(
            mGround,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/Starting.png",
            });
    gCoordinator.AddComponent(
            mGround,
            BoxColliderComponent{
                .position = Vector2(0.f, screenHeight - 270.f),
                .w = 7680,
                .h = 270
            });

    // Adding some test objs for level
    mTestObject = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mTestObject,
            TransformComponent{
                .position = Vector2(800.f, screenHeight - (270.f + 120.f)),
            });
    gCoordinator.AddComponent(
            mTestObject,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 120.f, 120.f},
                .path = "src/Assets/SandStoneSpriteSheet.png",
            });
    gCoordinator.AddComponent(
            mTestObject,
            BoxColliderComponent{
                .position = Vector2(800.f, screenHeight - (270.f + 120.f)),
                .w = 120,
                .h = 120
            });

    // Setting Render camera to follow player
    Camera playerCam = Camera();
    playerCam.FollowEntity(mPlayer);
    playerCam.SetOffset(Vector2(gScreenSize.x / 2, gScreenSize.y / 2));
    gCoordinator.GetSystem<RenderSystem>()->SetCamera(playerCam);
    // gCoordinator.GetSystem<RenderSystem>()->SetCameraFollowEntity(mPlayer);

    // Setting up update order
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

    // Destroy the Texture components
    gCoordinator.GetComponent<TextureComponent>(mPlayer).destroy();
    gCoordinator.GetComponent<TextureComponent>(mBackground).destroy();
    gCoordinator.GetComponent<TextureComponent>(mGround).destroy();
    gCoordinator.GetComponent<TextureComponent>(mTestObject).destroy();

    // Destroying entities
    gCoordinator.DestroyEntity(mPlayer);
    gCoordinator.DestroyEntity(mGround);
    gCoordinator.DestroyEntity(mBackground);
    gCoordinator.DestroyEntity(mTestObject);
}


void PlayState::HandleEvent(SDL_Event* e)
{
    // Resetting x movement
    auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(mPlayer);


    if (e->type == SDL_EVENT_KEY_DOWN)
    {
        // Movements
        // JUMP
        if (e->key.key == SDLK_SPACE)
        {
            // Initial Jump
            // Should be and is on Ground
            if (mSpaceInputCount == 0)
            {
                // NOTE: positive y is downwards
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
        // if (!mIsDownHeldDown && e->key.key == SDLK_DOWN)
        // {
        //     mIsDownHeldDown = true;
        //     auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(mPlayer);
        //     kinematics.velocity.y = 2000.f;
        // }

        // LEFT
        if (e->key.key == SDLK_A)
        {
            kinematics.velocity.x = -1 * kPlayerMoveSpeed;
            mIsAHeldDown = true;
        }
        // RIGHT
        if (e->key.key == SDLK_D)
        {
            kinematics.velocity.x = kPlayerMoveSpeed;
            mIsDHeldDown = true;
        }

    }

    // No key is down
    else if (e->type == SDL_EVENT_KEY_UP)
    {
        if (e->key.key == SDLK_SPACE)
        {
            mIsSpaceHeldDown = false;
        }
        if (e->key.key == SDLK_A)
        {
            mIsAHeldDown = false;
        }
        if (e->key.key == SDLK_D)
        {
            mIsDHeldDown = false;
        }
    }


    // Handling side to side movement cases
    if (!mIsAHeldDown && !mIsDHeldDown)
    {
        kinematics.velocity.x = 0.f;
    }
    else if (!mIsAHeldDown && mIsDHeldDown)
    {
        kinematics.velocity.x = kPlayerMoveSpeed;
    }
    else if (mIsAHeldDown && !mIsDHeldDown)
    {
        kinematics.velocity.x = -1 * kPlayerMoveSpeed;
    }
}


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
    // if (collisionSystem->didPlayerHitWall() == true)
    // {
    //     SDL_Log("Slowing player down");
    //     auto scrollSystem = gCoordinator.GetSystem<ScrollSystem>();
    //     float scrollSpeed = scrollSystem->GetScrollSpeed();
    //     // It halfs it plus a lil more
    //     scrollSystem->DecreaseScrollSpeed((scrollSpeed / 2) + kDecreaseSpeedAmount);
    // }


    // Updating systems
    for (int i = 0; i < mSystemUpdateOrder.size(); i++)
    {
        mSystemUpdateOrder[i]->Update(deltaTime);
    }
}
