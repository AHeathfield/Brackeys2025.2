#pragma once

#include <SDL3/SDL_render.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "../Core/Vector2.h"
#include "../Core/Timer.h"

struct Animation
{
    // int numberOfFrames;
    int numberOfImages = 1;
    int currentImage = 1;

    // Sprite Sheet Stuff
    int columnsPerRow = 1;
    // int rows = 1; // May not be necessary
    float clipWidth;
    float clipHeight;
    float animationTime = 0.f;

    Timer timer = Timer();
    Vector2 startClipPos = Vector2();

    bool isPlaying = false;
    bool isLooping = false;

    // This is actually recommended for large images instead of sprite sheet
    bool isImageAnimation = false;
    std::vector<SDL_Texture*> textures;
};


struct AnimationComponent
{
    std::unordered_map<std::string, Animation> animations;

    void Play(std::string animationName)
    {
        animations[animationName].isPlaying = true;
    }

    void Stop(std::string animationName)
    {
        animations[animationName].isPlaying = false;
    }
};
