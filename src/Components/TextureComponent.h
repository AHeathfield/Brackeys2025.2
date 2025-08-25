#pragma once

#include <SDL3/SDL.h>
#include "../Core/Vector2.h"
#include "../Core/Utility.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

struct TextureComponent
{
    // Contains texture data
    SDL_Texture* texture;
    SDL_FRect spriteClip{-1.f, -1.f, -1.f, -1.f}; // Default values
    std::string path;
    int depth = 0;

    // For text
    bool isText = false;
    TTF_Font* font = nullptr;
    int fontSize = 32;
    std::string text = "";
    SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF};

    // Texture Dimensions
    int width = 0;
    int height = 0;

    bool isSpriteClip()
    {
        return !(spriteClip.x < 0 || spriteClip.y < 0 || spriteClip.h < 0 || spriteClip.w < 0);
    }

    bool loadFont()
    {
        font = TTF_OpenFont(utility::getProjectPath(path).c_str(), fontSize);
        return font != nullptr;
    }

    // Function to destroy the texture properly
    void destroy()
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        width = 0;
        height = 0;
    }

    // For text
    void destroyFont()
    {
        if (isText)
        {
            TTF_CloseFont(font);
            font = nullptr;
        }
    }
};
