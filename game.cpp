// External Libs
#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>
#include <typeinfo>

// ECS
// Components
#include "src/Core/ECS.h"
#include "src/Components/TextureComponent.h"
#include "src/Components/TransformComponent.h"
#include "src/Components/Colliders/BoxColliderComponent.h"
#include "src/Components/ScoreComponent.h"
#include "src/Components/ControllerComponent.h"
#include "src/Components/KinematicsComponent.h"
#include "src/Components/AnimationComponent.h"
#include "src/Components/AudioComponent.h"
#include "src/Components/HurtComponent.h"
#include "src/Components/CollectibleComponent.h" // I don't think we need to use this, I'm just going to do biscuitcomponent instead, may be good in the future though...
#include "src/Components/BiscuitComponent.h"
#include "src/Components/SolidComponent.h"
#include "src/Components/StateChangerComponent.h"

// Systems
#include "src/Core/Vector2.h"
#include "src/Systems/RenderSystem.h"
#include "src/Systems/PhysicsSystem.h"
#include "src/Systems/CollisionSystem.h"
#include "src/Systems/ScoreSystem.h"
#include "src/Systems/AnimationSystem.h"
#include "src/Systems/AudioSystem.h"
#include "src/Systems/MoveSystem.h"

// States
#include "src/States/State.h"
#include "src/States/TitleState.h"
#include "src/States/PlayState.h"

// Core
#include "src/Core/Timer.h"
#include "src/Core/Utility.h"


Coordinator gCoordinator;
State* gCurrentState;
Vector2 gScreenSize;

/* Constants */
constexpr int kScreenFps{ 60 };


/* Function Prototypes */
//Starts up SDL and creates window
bool init();

//Loads media handled by RenderSystem
// bool loadMedia();

//Frees media and shuts down SDL
void close();

//Check collision between two AABBs
bool checkCollision(SDL_Rect a, SDL_Rect b);


/* Function Implementations */
bool init()
{
    //Initialization flag
    bool success{ true };

    //Initialize SDL with audio
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == false )
    {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Initialize the font loading
        if( TTF_Init() == false )
        {
            SDL_Log( "SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError() );
            success = false;
        }
    }



    return success;
}


void close()
{
    //Quit SDL subsystems
    // Mix_Quit();
    // TTF_Quit();
    // SDL_Quit();
}


int main( int argc, char* args[] )
{
    // utility::panicAndAbort("Test", "Test text");

    // Final exit code
    int exitCode{ 0 };

    // Timer used to track how long it took to run the frame
    Timer frameTimer = Timer();

    // Timer to cap frame rate
    Timer capTimer = Timer();
    Uint64 renderingNS = 0; // Time spent rendering

    // Setting Initial State and next State
    gCurrentState = new TitleState();
    State* prevState = gCurrentState;

    // Initalizing ECS Stuff
    gCoordinator.Init();
    bool successInit = init();

    // Registering Components
    gCoordinator.RegisterComponent<TextureComponent>();
    gCoordinator.RegisterComponent<TransformComponent>();
    gCoordinator.RegisterComponent<ScoreComponent>();
    gCoordinator.RegisterComponent<ControllerComponent>();
    gCoordinator.RegisterComponent<KinematicsComponent>();
    gCoordinator.RegisterComponent<AnimationComponent>();
    gCoordinator.RegisterComponent<AudioComponent>();
    gCoordinator.RegisterComponent<HurtComponent>();
    gCoordinator.RegisterComponent<CollectibleComponent>();
    gCoordinator.RegisterComponent<BiscuitComponent>();
    gCoordinator.RegisterComponent<SolidComponent>();
    gCoordinator.RegisterComponent<StateChangerComponent>();
    gCoordinator.RegisterComponent<ButtonComponent*>();
    gCoordinator.RegisterComponent<BoxColliderComponent*>();

    // Registering Systems
    // Render System
    auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<TextureComponent>());
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        gCoordinator.SetSystemSignature<RenderSystem>(signature);
    }
    renderSystem->Init();

    auto audioSystem = gCoordinator.RegisterSystem<AudioSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<AudioComponent>());
        gCoordinator.SetSystemSignature<AudioSystem>(signature);
    }
    audioSystem->Init();

    // Mouse Button System
    // auto mouseButtonSystem = gCoordinator.RegisterSystem<MouseButtonSystem>();
    // {
    //     Signature signature;
    //     signature.set(gCoordinator.GetComponentType<TextureComponent>());
    //     signature.set(gCoordinator.GetComponentType<TransformComponent>());
    //     signature.set(gCoordinator.GetComponentType<ButtonComponent*>());
    //     gCoordinator.SetSystemSignature<MouseButtonSystem>(signature);
    // }

    // Physics System
    auto physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<BoxColliderComponent*>());
        signature.set(gCoordinator.GetComponentType<KinematicsComponent>());
        gCoordinator.SetSystemSignature<PhysicsSystem>(signature);
    }

    // Animation System
    auto animationSystem = gCoordinator.RegisterSystem<AnimationSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<AnimationComponent>());
        signature.set(gCoordinator.GetComponentType<TextureComponent>());
        gCoordinator.SetSystemSignature<AnimationSystem>(signature);
    }

    // Score System
    auto scoreSystem = gCoordinator.RegisterSystem<ScoreSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<ScoreComponent>());
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        signature.set(gCoordinator.GetComponentType<TextureComponent>());
        gCoordinator.SetSystemSignature<ScoreSystem>(signature);
    }

    // Collision System
    auto collisionSystem = gCoordinator.RegisterSystem<CollisionSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        signature.set(gCoordinator.GetComponentType<BoxColliderComponent*>());
        gCoordinator.SetSystemSignature<CollisionSystem>(signature);
    }

    // Move System
    auto moveSystem = gCoordinator.RegisterSystem<MoveSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<TransformComponent>());
        signature.set(gCoordinator.GetComponentType<BoxColliderComponent*>());
        signature.set(gCoordinator.GetComponentType<KinematicsComponent>());
        gCoordinator.SetSystemSignature<MoveSystem>(signature);
    }

    // Other systems...


    // TODO: Currently RenderSystem intializes window and renderer
    //Initialize
    if( successInit == false )
    {
        SDL_Log( "Unable to initialize program!\n" );
        exitCode = 1;
    }
    else
    {
        //The quit flag
        bool quit{ false };
        bool stateChanged = false;
        int moveCount = 0;
        bool canMove = true;

        //The event data
        SDL_Event e;
        SDL_zero( e );

        // SETTING ENTITIES
        // When you do AddComponent, it automatically updates which systems should have it
        // State->Enter() calls gCoordinator.AddComponent
        gCurrentState->Enter();

        // DO THIS AFTER LOADING TEXTURES!!!
        if (renderSystem->LoadAllMedia() == false)
        {
            SDL_Log( "Unable to load media!\n" );
            exitCode = 2;
        }


        // updateTimer.start();

        float deltaTime = 0.015f; // Avg deltaTime for 60 fps apparently
        // =================== Main Loop ========================
        while( quit == false )
        {
            // Start deltaTime tracking
            frameTimer.start();
            //Start frame time
            capTimer.start();

            // Setting new state
            if (gCurrentState != prevState)
            {
                // SDL_Log("TEST");
                prevState->Exit();
                delete prevState;
                gCurrentState->Enter();
                renderSystem->LoadAllMedia();
                // renderSystem->Update();
                prevState = gCurrentState;
            }



            //Get event data
            while( SDL_PollEvent( &e ) == true )
            {
                //If event is quit type
                if( e.type == SDL_EVENT_QUIT )
                {
                    //End the main loop
                    quit = true;
                }

                // NEW, now state handles events!
                // FIXME: HERE
                gCurrentState->HandleEvent(&e);
                // if (typeid(*gCurrentState) == typeid(TitleState))
                // {
                //     SDL_Log("Title Handle");
                // }
                // else
                // {
                //     SDL_Log("Play Handle");
                // }
            }

            gCurrentState->Update(deltaTime);

            // If time remaining in frame
            constexpr Uint64 nsPerFrame = 1000000000 / kScreenFps; 
            Uint64 frameNs = capTimer.getTicksNS();
            if ( frameNs < nsPerFrame)
            {
                SDL_DelayNS(nsPerFrame - frameNs);
            }

            // Updating deltaTime
            deltaTime = frameTimer.getTimeS();
            // std::string log = std::to_string(deltaTime);
            // SDL_Log(log.c_str());
            frameTimer.stop();

            // if (typeid(*gCurrentState) == typeid(TitleState))
            // {
            //     SDL_Log("Title");
            // }
            // else
            // {
            //     SDL_Log("Play");
            // }
        }
        // =======================================================
        gCurrentState->Exit();
    }

    //Clean up
    delete gCurrentState;
    gCurrentState = nullptr;
    prevState = nullptr;

    renderSystem->Close();
    audioSystem->Close();
    close();

    return exitCode;
}
