#include "PlayState.h"
#include "State.h"
#include "TitleState.h"
#include <SDL3/SDL_rect.h>
#include <string>
#include <unordered_map>

extern Coordinator gCoordinator;
extern Vector2 gScreenSize;

void PlayState::Enter()
{
    const float screenHeight = 1080.f;
    const float screenWidth = 1920.f;

    BoxColliderComponent* boxCollider = nullptr;

    // Initializing the ScrollSystem
    // auto mScrollSystem = gCoordinator.GetSystem<ScrollSystem>();
    // mScrollSystem->Init();

    // The player
    mPlayer = gCoordinator.CreateEntity();
    // SDL_Log(std::to_string(mPlayer).c_str());
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
                .position = Vector2(500.f, 500.f),
            });
    gCoordinator.AddComponent(
            mPlayer,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 64.f, 64.f},
                .path = "src/Assets/RunSpriteSheetTEST.png",
                .depth = 10
            });
    boxCollider = new PlayerCollider(
        Vector2(500.f, 500.f), //screenHeight - 334.f),
        64,
        64
    );
    gCoordinator.AddComponent(
            mPlayer,
            boxCollider
            );
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
    gCoordinator.AddComponent(
            mPlayer,
            BiscuitComponent{
            });

    mBiscuit = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mBiscuit,
            TransformComponent{
                .position = Vector2(1400.f, 500.f),
            });
    gCoordinator.AddComponent(
            mBiscuit,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 40.f, 40.f},
                .path = "src/Assets/yellowBlock.png",
                .depth = 10
            });
    boxCollider = new BiscuitCollider(
        Vector2(1400.f, 500.f), //screenHeight - 334.f),
        40,
        40
    );
    gCoordinator.AddComponent(
            mBiscuit,
            boxCollider
            );


    mHome = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mHome,
            TransformComponent{
                .position = Vector2(200.f, screenHeight - 270.f - 80.f),
            });
    gCoordinator.AddComponent(
            mHome,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 80.f, 80.f},
                .path = "src/Assets/CitySpriteSheet.png",
                .depth = 10
            });
    boxCollider = new DefaultCollider(
        Vector2(200.f, screenHeight - 270.f - 80.f),
        80,
        80
    );
    gCoordinator.AddComponent(
            mHome,
            boxCollider
            );
    gCoordinator.AddComponent(
            mHome, 
            StateChangerComponent{
                .newState = new TitleState()
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
    boxCollider = new DefaultCollider(
        Vector2(0.f, screenHeight - 270.f),
        7680,
        270
    );
    gCoordinator.AddComponent(
            mGround,
            boxCollider
            );
    gCoordinator.AddComponent(
            mGround,
            SolidComponent{
            });
    // gCoordinator.AddComponent(
    //         mGround,
    //         BoxColliderComponent{
    //             .position = Vector2(0.f, screenHeight - 270.f),
    //             .w = 7680,
    //             .h = 270
    //         });

    // Adding some test objs for level
    mTestObject = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mTestObject,
            TransformComponent{
                .position = Vector2(1300.f, screenHeight - (270.f + 120.f)),
            });
    gCoordinator.AddComponent(
            mTestObject,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 120.f, 120.f},
                .path = "src/Assets/SandStoneSpriteSheet.png",
            });
    boxCollider = new DefaultCollider(
            Vector2(1300.f, screenHeight - (270.f + 120.f)),
            120,
            120
    );
    gCoordinator.AddComponent(
            mTestObject,
            boxCollider
            );
    gCoordinator.AddComponent(
            mTestObject,
            SolidComponent{
            });

    mTestObjectAir = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mTestObjectAir,
            TransformComponent{
                .position = Vector2(1000.f, 400.f),
            });
    gCoordinator.AddComponent(
            mTestObjectAir,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 120.f, 120.f},
                .path = "src/Assets/SandStoneSpriteSheet.png",
            });
    boxCollider = new DefaultCollider(
            Vector2(1000.f, 400.f),
            120,
            120
    );
    gCoordinator.AddComponent(
            mTestObjectAir,
            boxCollider
            );
    gCoordinator.AddComponent(
            mTestObjectAir,
            SolidComponent{
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
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<MoveSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<AnimationSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<RenderSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<AudioSystem>());

    boxCollider = nullptr;
}

void PlayState::Exit()
{
    // Clear the update order
    mSystemUpdateOrder.clear();

    // Destroy the Texture components
    gCoordinator.GetComponent<TextureComponent>(mPlayer).destroy();
    gCoordinator.GetComponent<TextureComponent>(mHome).destroy();
    gCoordinator.GetComponent<TextureComponent>(mBiscuit).destroy();
    gCoordinator.GetComponent<TextureComponent>(mBackground).destroy();
    gCoordinator.GetComponent<TextureComponent>(mGround).destroy();
    gCoordinator.GetComponent<TextureComponent>(mTestObject).destroy();
    gCoordinator.GetComponent<TextureComponent>(mTestObjectAir).destroy();

    // Destroying entities
    gCoordinator.DestroyEntity(mPlayer);
    gCoordinator.DestroyEntity(mBiscuit);
    gCoordinator.DestroyEntity(mHome);
    gCoordinator.DestroyEntity(mGround);
    gCoordinator.DestroyEntity(mBackground);
    gCoordinator.DestroyEntity(mTestObject);
    gCoordinator.DestroyEntity(mTestObjectAir);
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
                kinematics.velocity.y = -900.f;
                kinematics.acceleration.y = PhysicsSystem::kGravity;

                // auto collisionSystem = gCoordinator.GetSystem<CollisionSystem>();
                // collisionSystem->SetPlayerIsOnGround(false);
                auto& temp = gCoordinator.GetComponent<BoxColliderComponent*>(mPlayer);
                PlayerCollider* playerCollider = static_cast<PlayerCollider*>(temp);
                playerCollider->SetPlayerIsOnGround(false);
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
    // DEBUG
    // const auto& transform = gCoordinator.GetComponent<TransformComponent>(mPlayer);
    // SDL_Log(transform.position.PrintPosition().c_str());

    // auto collisionSystem = gCoordinator.GetSystem<CollisionSystem>();

    if (gCoordinator.HasComponent<BoxColliderComponent*>(mPlayer))
    {
        auto& temp = gCoordinator.GetComponent<BoxColliderComponent*>(mPlayer);
        PlayerCollider* playerCollider = static_cast<PlayerCollider*>(temp);
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
        else if (!mIsSpaceHeldDown && mSpaceInputCount > 0 && playerCollider->IsPlayerOnGround())
        {
            mSpaceInputCount = 0;
        }

        // If player is not on ground
        if (!playerCollider->IsPlayerOnGround())
        {
            auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(mPlayer);
            kinematics.acceleration.y = PhysicsSystem::kGravity;
        }

        // If player hit a wall (maybe for wall jump??)
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

            // Going to handle any collisions that occur
            // if (mSystemUpdateOrder[i].get() == collisionSystem.get())
            // {
            //     auto& playerCollider = gCoordinator.GetComponent<BoxColliderComponent*>(mPlayer);
            //
            //     if (playerCollider->isColliding())
            //     {
            //         SDL_Log
            //     }
            // }
        }
    }
}
