#pragma once

#include <SDL3/SDL_render.h>
#include <map>
#include <unordered_map>
#include <vector>
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/ControllerComponent.h"
#include "../Core/ECS.h"
#include "../Core/Camera.h"
#include "../States/State.h"
// #include "../States/TitleState.h"

// This is for the layering implementation
struct RenderData
{
    TextureComponent* textureComponent = nullptr;
    TransformComponent* transformComponent = nullptr;
    
    std::string texturePath = "";

    // When an entity gets deleted it's components will just point to another entity which is why we need to check it's texture path changed
    bool isNULL()
    {
        if (texturePath != textureComponent->path)
        {
            std::string log = "Old Texture Path: " + texturePath + ", New Texture Path: " + textureComponent->path ;
            SDL_Log(log.c_str());
        }
        return (texturePath != textureComponent->path);
        // return (textureComponent == nullptr || transformComponent == nullptr);
    }

    void destroy()
    {
        textureComponent = nullptr;
        transformComponent = nullptr;
    }
};

class RenderSystem : public System
{
    static constexpr int kLogicalRenderX = 1920;
    static constexpr int kLogicalRenderY = 1080;

public:
    // initializes window
    bool Init();

    bool LoadAllMedia();

    bool LoadMedia(TextureComponent* textureComponent);

    // Things that happen every frame
    void Update(float deltaTime) override;

    // Renders texture
    void Render(TransformComponent transfromComponent, TextureComponent textureComponent, double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipMode = SDL_FLIP_NONE );

    Vector2 GetLogicalMouseCoords(const Vector2& mousePos);

    void EnableTextInput();
    void DisableTextInput();

    void SetCamera(Camera& camera);
    void SetCameraFollowEntity(Entity entity);
    void AdjustCameraOffset(Vector2& adjustment);

    // Closing stuff
    void Close();

private:
    // Loads texture from file
    // We use TextureComponent& when we just want handle the object normally but its really big, we use TextureComponent* when we actually want to change the value of the object being passed in
    bool loadTexture(TextureComponent* textureComponent);
    bool loadFromRenderedText(TextureComponent* textureComponent);
    void addToRenderOrder(Entity newEntity);

private:
    SDL_Renderer* mRenderer = nullptr;
    SDL_Window* mWindow = nullptr;

    Camera mCamera;

    // This is too store any textures we have loaded, <path, texture>
    std::unordered_map<std::string, SDL_Texture*> mTextures;

    // Using the fact that map by nature is sorted using red-black tree, therefore keys will sorted least to greatest
    // The key is the texture depth
    std::map<int, std::vector<RenderData>> mRenderOrder;

    int mOldNumOfEntities = 0;
    int mOldStateNumber = 0;
};
