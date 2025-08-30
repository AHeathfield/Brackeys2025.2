#include "TitleState.h"
#include "PlayState.h"
#include <SDL3/SDL_rect.h>
#include <string>

extern Coordinator gCoordinator;

void TitleState::Enter()
{
    // Unique state Number
    // if (mStateNumber == -1)
    // {
    //     State::stateNumber++;
    //     mStateNumber = State::stateNumber;
    // }

    // The player
    if (mBackground == MAX_ENTITIES + 1)
    {
        // Comment this out if you just want to use spritesheet
        auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
        std::vector<SDL_Texture*> curtainTextures;
        for (int i = 0; i < 11; i++)
        {
            std::string path = "src/Assets/Animations/TheaterCurtains/Curtain_Animation-" + std::to_string(i+1) + ".png";
            TextureComponent curtainTexture = {
                .texture = nullptr,
                .path = path,
                .depth = 15
            };
            renderSystem->LoadMedia(&curtainTexture);
            curtainTextures.push_back(curtainTexture.texture);
        }

        mBackground = gCoordinator.CreateEntity();
        // SDL_Log(std::to_string(mBackground).c_str());
        // std::unordered_map<std::string, Animation> curtainAnimations;
        // Animation openCurtains = {
        //     .numberOfImages = 11,
        //     .columnsPerRow = 4,
        //     .clipWidth = 1920.f,
        //     .clipHeight = 1080.f,
        //     .animationTime = 0.2f,
        //     .startClipPos = Vector2(),
        //     .isPlaying = false,
        //     .isLooping = false,
        //     // Comment these out if you just want to use spriteSheet
        //     .isImageAnimation = true,
        //     .textures = curtainTextures
        // };
        // curtainAnimations.insert({"Open Curtains", openCurtains});
        gCoordinator.AddComponent(
                mBackground,
                TextureComponent{
                    .texture = nullptr,
                    .spriteClip = SDL_FRect{0.f, 0.f, 1920.f, 1080.f},
                    .path = "src/Assets/CurtainSpriteSheet.png",
                    // .path = "src/Assets/Animations/TheaterCurtains/Curtain_Animation-1.png",
                    .depth = 15
                });
        gCoordinator.AddComponent(
                mBackground,
                TransformComponent{
                    .position = Vector2(),
                    .rotation = Vector2(),
                    .scale = Vector2()
                });
        // gCoordinator.AddComponent(
        //         mBackground, 
        //         AnimationComponent{
        //             .animations = curtainAnimations
        //         });
    }

    mPlayButton = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mPlayButton,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, 517.f, 183.f},
                .path = "src/Assets/playButtonSpriteSheet.png",
                .depth = 16
            });
    gCoordinator.AddComponent(
            mPlayButton,
            TransformComponent{
                .position = Vector2((1920 - 517) / 2.f, 1080 - 350),
                .rotation = Vector2(),
                .scale = Vector2()
            });
    ButtonComponent* button = new StateChangerButton(new PlayState());
    gCoordinator.AddComponent(
            mPlayButton,
            button
            );
    button = nullptr;

    // Setting the Systems Update Order
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<AnimationSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<RenderSystem>());
    mSystemUpdateOrder.push_back(gCoordinator.GetSystem<AudioSystem>());
}

void TitleState::Exit()
{
    // Clear the update order
    mSystemUpdateOrder.clear();

    // auto& animationComponent = gCoordinator.GetComponent<AnimationComponent>(mBackground);
    // animationComponent.Play("Open Curtains");
    // SDL_Log("Leaving TitleState")
    // Destroy the Texture component
    gCoordinator.GetComponent<TextureComponent>(mBackground).destroy();
    gCoordinator.GetComponent<TextureComponent>(mPlayButton).destroy();

    gCoordinator.DestroyEntity(mBackground);
    gCoordinator.DestroyEntity(mPlayButton);
    // SDL_Log("DONE LEAVING TITLE");
    //
}

void TitleState::HandleEvent(SDL_Event* e)
{
    //If mouse event happened
    if( e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP )
    {
        auto renderSystem = gCoordinator.GetSystem<RenderSystem>();

        //Get mouse position
        float x = -1.f, y = -1.f;
        SDL_GetMouseState( &x, &y );
        Vector2 mousePos = renderSystem->GetLogicalMouseCoords(Vector2(x, y));

        //Check if mouse is in button
        auto& texture = gCoordinator.GetComponent<TextureComponent>(mPlayButton);
        const auto& transform = gCoordinator.GetComponent<TransformComponent>(mPlayButton);
        auto& button = gCoordinator.GetComponent<ButtonComponent*>(mPlayButton);

        if (isMouseInside(mousePos, transform.position, texture))
        {
            activateHoverColor(&texture);
            
            if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                activatePressColor(&texture);
            }
            else if (e->type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                disableEffectColor(&texture);
                button->Click();
            }
        }
        else
        {
            disableEffectColor(&texture);
        }
    }


}


// I think put like the systems here in future
void TitleState::Update(float deltaTime)
{
    for (int i = 0; i < mSystemUpdateOrder.size(); i++)
    {
        mSystemUpdateOrder[i]->Update(deltaTime);
    }
}


bool TitleState::isMouseInside(Vector2 mousePos, Vector2 buttonPos, const TextureComponent& texture)
{
    //Mouse is left of the button
    if( mousePos.x < buttonPos.x )
    {
        return false;
    }
    //Mouse is right of the button
    else if( mousePos.x > buttonPos.x + texture.spriteClip.w)
    {
        return false;
    }
    //Mouse above the button
    else if( mousePos.y < buttonPos.y )
    {
        return false;
    }
    //Mouse below the button
    else if( mousePos.y > buttonPos.y + texture.spriteClip.h)
    {
        return false;
    }

    return true;
}

void TitleState::activateHoverColor(TextureComponent* texture)
{
    texture->spriteClip.y = texture->spriteClip.h;
}

void TitleState::activatePressColor(TextureComponent* texture)
{
    texture->spriteClip.y = texture->spriteClip.h * 2;
}

void TitleState::disableEffectColor(TextureComponent* texture)
{
    texture->spriteClip.y = 0.f;
}

