#include "RenderSystem.h"
#include "../Core/Utility.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <string>

extern Coordinator gCoordinator;
extern State* gCurrentState;
extern Vector2 gScreenSize;

bool RenderSystem::Init()
{
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode* DM = SDL_GetCurrentDisplayMode(displayID);
    if (DM == nullptr)
    {
        SDL_Log( "Failed to get Display Mode! SDL error: %s\n", SDL_GetError() );
        return false;
    }
    gScreenSize.x = DM->w;
    gScreenSize.y = DM->h;

    // int screenWidth = 1920;
    // int screenHeight = 1080;

    // TODO: Don't hardcode this...
    mCamera.x = 0.f;
    mCamera.y = 0.f;
    mCamera.w = gScreenSize.x;
    mCamera.h = gScreenSize.y;

    // Framerate Stuff
    // mScreenFps = 60;
    // capTimer = 
 

    // Initializing Window and Renderer
    if (SDL_CreateWindowAndRenderer( "NastyTetris", gScreenSize.x, gScreenSize.y, 0, &mWindow, &mRenderer ) == false )
    {
        SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
        return false;
    }

    // Setting Fullscreen option (true if you want full screen)
    // TODO: In a settings window/state, have an option to turn on fullscreen
    if (SDL_SetWindowFullscreen(mWindow, true) == false)
    {
        SDL_Log( "Fullscreen cannot be set! SDL error: %s\n", SDL_GetError() );
        return false;
    }

    // This scales the render output properly, so the game was made for 1920x1080 but lets say my laptop
    // aspect ratio doesn't fit it well, it will scale it properly depending on the mode
    // I currently have LETTERBOX which creates those blackbars, I found STRETCH and INTEGER_SCALE to also be good
    // https://wiki.libsdl.org/SDL3/SDL_SetRenderLogicalPresentation
    if (SDL_SetRenderLogicalPresentation(mRenderer, kLogicalRenderX, kLogicalRenderY, SDL_LOGICAL_PRESENTATION_LETTERBOX) == false)
    {

        SDL_Log( "Cannot scale renderer! SDL error: %s\n", SDL_GetError() );
        return false;
    }


    // Initializing the render order
    // createRenderOrder();
    mOldNumOfEntities = mEntities.size();
    // mOldStateNumber = gCurrentState->GetStateNumber();

    // Everything initialize good
    return true;
}


void RenderSystem::Update()
{
    // Updating window size if it changed
    int windowW, windowH;
    SDL_GetWindowSize(mWindow, &windowW, &windowH);
    gScreenSize.x = windowW;
    gScreenSize.y = windowH;

    // TODO: Bound camera

    // Fills the background
    SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF,  0xFF );
    SDL_RenderClear( mRenderer );

    // New layering system
    if (mOldNumOfEntities < mEntities.size())
    {
        auto it = mEntities.end(); // Not actually the newest entity, but the it after it
        Entity newEntity;

        // This is because their might be more than 1 add
        for (int i = 0; i < mEntities.size() - (mOldNumOfEntities - 1); i++)
        {
            // Getting the new entity
            it = prev(it);
            newEntity = *it;
            auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(newEntity);
            auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(newEntity);

            // creates the texture for the texture component since it's a new entity
            LoadMedia(&textureComponent);

            RenderData renderData = {
                .textureComponent = &textureComponent,
                .transformComponent = &transformComponent,
                .texturePath = textureComponent.path
            };

            // If there already entitys with same depth
            if (mRenderOrder.find(textureComponent.depth) != mRenderOrder.end())
            {
                mRenderOrder[textureComponent.depth].push_back(renderData);
            }
            else
            {
                std::vector<RenderData> tempArray = {renderData};
                mRenderOrder.insert({textureComponent.depth, tempArray});

            }
        }
        // it = nullptr;
        mOldNumOfEntities = mEntities.size();
    }

    // Rendering in correct order
    for (auto it = mRenderOrder.begin(); it != mRenderOrder.end(); it++)
    {
        // Logging what depth is currently being rendered
        // std::string log = "Depth: " + std::to_string(it->first);
        // SDL_Log(log.c_str());
        // Copy of the array since we might be deleting some renders
        for (int i = 0; i < it->second.size();)
        {
            if (it->second[i].isNULL())
            {
                SDL_Log("erasing!");
                // Erasing from the actual and not copy
                it->second[i].destroy(); // Just in case 1 of the components still exist
                it->second.erase(it->second.begin() + i);
            }
            else
            {
                Render(*it->second[i].transformComponent, *it->second[i].textureComponent);

                // Only increment here since not erasing
                i++;
            }
        }
    }

    // TODO: THIS DOESN'T WORK, TRY TO GET IT TOO WORK!!
    // Recreate the render order since an entity was deleted
    // if (mOldStateNumber != gCurrentState->GetStateNumber() || mOldNumOfEntities > mEntities.size())
    // {
    //     // SDL_Log("Recreating renderer");
    //     createRenderOrder();
    // }
    // // Update the render order since their is new entity
    // else if (mOldNumOfEntities < mEntities.size())
    // {
    //     // SDL_Log("Updating Render");
    //     auto it = mEntities.rbegin(); // Latest add
    //     Entity newEntity;
    //
    //     for (int i = 0; i < mEntities.size() - mOldNumOfEntities; i++)
    //     {
    //         // Getting the new entity
    //         if (i != 0)
    //         {
    //             it = prev(it);
    //         }
    //         newEntity = *it;
    //
    //         // Putting it in render order map
    //         const auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(newEntity);
    //         // If there is a texture with that depth size already
    //         if (mRenderOrder.find(textureComponent.depth) != mRenderOrder.end())
    //         {
    //             mRenderOrder[textureComponent.depth].push_back(newEntity);
    //         }
    //         else
    //         {
    //             std::vector<Entity> tempArray = {newEntity};
    //             mRenderOrder.insert({textureComponent.depth, tempArray});
    //             mDepths.push_back(textureComponent.depth);
    //         }
    //     }
    //     std::sort(mDepths.begin(), mDepths.end());
    // }
    // mOldStateNumber = gCurrentState->GetStateNumber();
    // mOldNumOfEntities = mEntities.size();
    //
    // std::string log = "Diff Depths: " + std::to_string(mRenderOrder.size());
    // SDL_Log(log.c_str());

    // Render Textures 
    // NOTE: Greater depth means it will be render on top
    // for (int i = 0; i < mDepths.size(); i++)
    // {
    //     std::vector<Entity> entities = mRenderOrder.at(mDepths[i]);
    //     for (const auto& entity : entities)
    //     {
    //         const auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
    //         const auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);
    //
    //         Render(transformComponent, textureComponent);
    //     }
    // }
    // for (auto it = mRenderOrder.begin(); it != mRenderOrder.end(); it++)
    // {
    //     std::vector<Entity> entities = it->second;
    //     // std::string log = "Depth: " + std::to_string(it->first) + ", Entities: " + std::to_string(entities.size());
    //     // SDL_Log(log.c_str());
    //
    // }p
    // if (mBg != MAX_ENTITIES + 1)
    // {
    //     const auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(mBg);
    //     const auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mBg);
    //     Render(transformComponent, textureComponent);
    // }
    //
    //
    // for (const auto& entity : mEntities)
    // {
    //     if (entity == mPlayer || entity == mBg || entity == mFg)
    //     {
    //         continue;
    //     }
    //
    //     const auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
    //     const auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);
    //     Render(transformComponent, textureComponent);
    // }
    //
    // if (mPlayer != MAX_ENTITIES + 1)
    // {
    //     const auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(mPlayer);
    //     const auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mPlayer);
    //     Render(transformComponent, textureComponent);
    // }
    //
    // if (mFg != MAX_ENTITIES + 1)
    // {
    //     const auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(mFg);
    //     const auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mFg);
    //     Render(transformComponent, textureComponent);
    // }
    //
    // if (mCurtain != MAX_ENTITIES + 1)
    // {
    //     const auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(mCurtain);
    //     const auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(mCurtain);
    //     Render(transformComponent, textureComponent);
    // }

    // Update Screen
    SDL_RenderPresent(mRenderer);
 
    // TODO: Do the framerate stuff
}

bool RenderSystem::LoadAllMedia()
{
    // Initializing Textures
    for (const auto& entity : mEntities)
    {
        auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);

        if (LoadMedia(&textureComponent) == false)
        {
            return false;
        }
    }
    return true;
}

// I don't think this has to be a bool with the way renderSystem works lol
bool RenderSystem::LoadMedia(TextureComponent* textureComponent)
{
    if (textureComponent->isText)
    {
        // Loading the font
        if (textureComponent->font == nullptr)
        {
            if (textureComponent->loadFont() == false)
            {
                SDL_Log( "Could not load %s! SDL_ttf Error: %s\n", textureComponent->path.c_str(), SDL_GetError() );
                return false;
            }
        }
        
        // loading text
        if (loadFromRenderedText(textureComponent) == false)
        {
            SDL_Log("Unable to load text texture.\n");
            return false;
        }
    }
    // Image
    else if (loadTexture(textureComponent) == false)
    {
        SDL_Log("Unable to load a texture (put better error msg here lol).\n");
        return false;
    }

    return true;
}


void RenderSystem::Render(TransformComponent transfromComponent, TextureComponent textureComponent, double degrees, SDL_FPoint* center, SDL_FlipMode flipMode)
{
    SDL_FRect dstRect{ transfromComponent.position.x, transfromComponent.position.y, static_cast<float>(textureComponent.width), static_cast<float>(textureComponent.height) };

    //Default to clip dimensions if clip is given
    if (textureComponent.isSpriteClip())
    {
        dstRect.w = textureComponent.spriteClip.w;
        dstRect.h = textureComponent.spriteClip.h;

        // Render Texture on screen
        SDL_RenderTextureRotated(mRenderer, textureComponent.texture, &textureComponent.spriteClip, &dstRect, degrees, center, flipMode );
    }
    else
    {
        // Render Texture on screen
        SDL_RenderTextureRotated(mRenderer, textureComponent.texture, nullptr, &dstRect, degrees, center, flipMode );
    }

}


// This is for getting proper mouse coordinates with scaled renderers
Vector2 RenderSystem::GetLogicalMouseCoords(const Vector2& mousePos)
{
    // NOTE: Assumes we are using LETTERBOX OLD
    float scale = std::min(
        gScreenSize.x / kLogicalRenderX,
        gScreenSize.y / kLogicalRenderY
    );

    float offsetX = (gScreenSize.x - (kLogicalRenderX * scale)) / 2;
    float offsetY = (gScreenSize.y - (kLogicalRenderY * scale)) / 2;

    return Vector2(
        (mousePos.x - offsetX) / scale,
        (mousePos.y - offsetY) / scale
    );
}

void RenderSystem::EnableTextInput()
{
    SDL_StartTextInput(mWindow);
}

void RenderSystem::DisableTextInput()
{
    SDL_StopTextInput(mWindow);
}

void RenderSystem::Close()
{
    // Destroys all textures
    for (const auto& entity : mEntities)
    {
        auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);
        textureComponent.destroy();

        // If it has a font it will destroy it
        textureComponent.destroyFont();
    }

    // Destroys window
    SDL_DestroyRenderer(mRenderer);
    mRenderer = nullptr;
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
}


// PRIVATE METHODS:
// Returns true if texture is loaded
bool RenderSystem::loadTexture(TextureComponent* textureComponent)
{
    // Checking if texture has been loaded before
    if (mTextures.find(textureComponent->path) != mTextures.end())
    {
        textureComponent->texture = mTextures[textureComponent->path];
        return true;
    }

    // TODO: Clean up texture if already exists
    textureComponent->destroy();
    // SDL_Log(textureComponent.path.c_str());

    std::string path = utility::getProjectPath(textureComponent->path);

    //Load surface
    if( SDL_Surface* loadedSurface = IMG_Load( path.c_str() ); loadedSurface == nullptr )
    {
        SDL_Log( "Unable to load image %s! SDL_image error: %s\n", textureComponent->path.c_str(), SDL_GetError() );
    }
    else
    {
        // SDL_Log("Image loaded correctly!");
        //Color key image
        if( SDL_SetSurfaceColorKey( loadedSurface, true, SDL_MapSurfaceRGB( loadedSurface, 0x00, 0xFF, 0xFF ) ) == false )
        {
            SDL_Log( "Unable to color key! SDL error: %s", SDL_GetError() );
        }
        else
        {
            //Create texture from surface
            if( textureComponent->texture = SDL_CreateTextureFromSurface( mRenderer, loadedSurface ); textureComponent->texture == nullptr )
            {
                SDL_Log( "Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError() );
            }
            else
            {
                //Get image dimensions
                textureComponent->width = loadedSurface->w;
                textureComponent->height = loadedSurface->h;
            }
        }
        
        //Clean up loaded surface
        SDL_DestroySurface( loadedSurface );
    }

    // Adding new texture to the collection
    if (textureComponent->texture != nullptr)
    {
        mTextures.insert({textureComponent->path, textureComponent->texture});
        return true;
    }

    return false;
}

bool RenderSystem::loadFromRenderedText(TextureComponent* textureComponent)
{
    // Checking if texture has been loaded before
    if (mTextures.find(textureComponent->path) != mTextures.end())
    {
        textureComponent->texture = mTextures[textureComponent->path];
        return true;
    }


    //Clean up existing texture
    textureComponent->destroy();

    //Load text surface
    if( SDL_Surface* textSurface = TTF_RenderText_Blended( textureComponent->font, textureComponent->text.c_str(), 0, textureComponent->textColor ); textSurface == nullptr )
    {
        SDL_Log( "Unable to render text surface! SDL_ttf Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create texture from surface
        if( textureComponent->texture = SDL_CreateTextureFromSurface( mRenderer, textSurface ); textureComponent->texture == nullptr )
        {
            SDL_Log( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            textureComponent->width = textSurface->w;
            textureComponent->height = textSurface->h;
        }

        //Free temp surface
        SDL_DestroySurface( textSurface );
    }

    // Adding new texture to the collection
    if (textureComponent->texture != nullptr)
    {
        mTextures.insert({textureComponent->path, textureComponent->texture});
        return true;
    }

    return false;

    //Return success if texture loaded
    // return textureComponent->texture != nullptr;
}

void RenderSystem::createRenderOrder()
{
    mRenderOrder.clear();
    // mDepths.clear();
    
    // Fills in the render order map
    for (const auto& entity : mEntities)
    {
        auto& textureComponent = gCoordinator.GetComponent<TextureComponent>(entity);
        auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
        RenderData renderData = {
            .textureComponent = &textureComponent,
            .transformComponent = &transformComponent
        };

        // If there is a texture with that depth size already
        if (mRenderOrder.find(textureComponent.depth) != mRenderOrder.end())
        {
            mRenderOrder[textureComponent.depth].push_back(renderData);
        }
        else
        {
            std::vector<RenderData> tempArray = {renderData};
            mRenderOrder.insert({textureComponent.depth, tempArray});
            // mDepths.push_back(textureComponent.depth);
        }
    }
}
