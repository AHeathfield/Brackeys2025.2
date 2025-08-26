#include "AnimationSystem.h"
#include <SDL3/SDL_rect.h>

extern Coordinator gCoordinator;

void AnimationSystem::Update(float deltaTime)
{
    for (const auto& entity : mEntities)
    {
        auto& animationComponent = gCoordinator.GetComponent<AnimationComponent>(entity);

        for (auto it = animationComponent.animations.begin(); it != animationComponent.animations.end(); it++)
        {
            auto& animation = it->second;

            if (animation.isPlaying)
            {
                auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);
                if (animation.isImageAnimation)
                {
                    UpdateImageAnimation(animation, textureComponent);
                }
                else
                {
                    UpdateAnimation(animation, textureComponent);
                }
            }
        }
    }
}


// Moves the animation to the next frame
void AnimationSystem::UpdateAnimation(Animation& animation, TextureComponent& textureComponent)
{
    // Maybe make it a member variable, although it might make it hard to change animation speeds if so.
    float interval = animation.animationTime / animation.numberOfImages;

    // This is so it switches the animation at the correct time
    if (animation.timer.isStarted() && !animation.timer.isPaused() && animation.timer.getTimeS() >= interval)
    {
        // If it was the last frame stop playing
        if (animation.currentImage == animation.numberOfImages)
        {
            animation.currentImage = 0;
            
            if (!animation.isLooping)
            {
                animation.isPlaying = false;
                return;
            }
            else
            {
                textureComponent.spriteClip = SDL_FRect{
                    .x = animation.startClipPos.x,
                    .y = animation.startClipPos.y,
                    .w = animation.clipWidth,
                    .h = animation.clipHeight
                };
                animation.currentImage++;
            }
        }
        else
        {
            // Getting current column and row
            int currentRow = (animation.currentImage / animation.columnsPerRow);
            int currentColumn = animation.currentImage - animation.columnsPerRow * (currentRow);

            textureComponent.spriteClip = SDL_FRect{
                .x = animation.startClipPos.x + (currentColumn * animation.clipWidth),
                .y = animation.startClipPos.y + (currentRow * animation.clipHeight),
                .w = animation.clipWidth,
                .h = animation.clipHeight
            };
            animation.currentImage++;
        }

        // Resets the timer for the next interval
        animation.timer.reset();
    }
    // Reset the timer (if it hasnt started it will start it)
    else if (!animation.timer.isStarted())
    {
        animation.timer.reset();
    }
}

void AnimationSystem::UpdateImageAnimation(Animation& animation, TextureComponent& textureComponent)
{
    // Maybe make it a member variable, although it might make it hard to change animation speeds if so.
    float interval = animation.animationTime / animation.numberOfImages;

    // This is so it switches the animation at the correct time
    if (animation.timer.isStarted() && !animation.timer.isPaused() && animation.timer.getTimeS() >= interval)
    {
        // If it was the last frame stop playing
        if (animation.currentImage == animation.numberOfImages)
        {
            animation.currentImage = 0;
            if (!animation.isLooping)
            {
                animation.isPlaying = false;
                return;
            }
            else
            {
                textureComponent.texture = animation.textures[animation.currentImage];
                animation.currentImage++;
            }
        }
        else
        {
            textureComponent.texture = animation.textures[animation.currentImage];
            animation.currentImage++;
        }

        // Resets the timer for the next interval
        animation.timer.reset();
    }
    // Reset the timer (if it hasnt started it will start it)
    else if (!animation.timer.isStarted())
    {
        animation.timer.reset();
    }
}
