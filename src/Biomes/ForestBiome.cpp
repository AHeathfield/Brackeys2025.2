#include "ForestBiome.h"
#include <SDL3/SDL_rect.h>
#include <string>
#include <vector>

extern Coordinator gCoordinator;


void ForestBiome::Init()
{
    const float screenHeight = 1080.f;
    Entity ground = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            ground,
            TransformComponent{
                .position = mBiomePos + Vector2(0.f, screenHeight - 270.f),
            });
    gCoordinator.AddComponent(
            ground,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/ForestGround.png",
                .depth = 1,
                .width = 7680,
                .height = 270
            });
    mBiomeEntities.push_back(ground);

    Entity foreground = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            foreground,
            TransformComponent{
                .position = mBiomePos + Vector2(0.f, screenHeight - 250.f),
            });
    gCoordinator.AddComponent(
            foreground,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/ForestForeGround.png",
                .depth = 2,
                .width = 7680,
                .height = 360 
            });
    mBiomeEntities.push_back(foreground);


    // Creating the Tree texture (mini asset manager)
    // TextureComponent treeTextureComponent = {
    //     .texture = nullptr,
    //     .path = "src/Assets/TreeSpriteSheet.png"
    // };
    // mRenderSystem->LoadMedia(&treeTextureComponent);
    // mTreeTexture = treeTextureComponent.texture;


    // Initial sapplings
    const int sapplingSize = 50;
    const int groundPosY = screenHeight - 270.f;
    for (int i = 0; i < mBiomeLength / kTreeWidth; i++)
    {
        if (utility::getTrueOneXChance(4) == true)
        {
            placeSappling(i * kTreeWidth);
        }
    }
}

void ForestBiome::Grow()
{
    std::vector<int> sapplingPositions;
    for (auto tree = mTrees.begin(); tree != mTrees.end(); tree++)
    {
        int pos = tree->first;
        Entity entity = tree->second;

        if (canGrow(entity))
        {
            growTree(entity);
        }

        if (canTreeSpreadLeft(pos))
        {
            sapplingPositions.push_back(pos - kTreeWidth);
            // placeSappling(pos - kTreeWidth);
        }
        if (canTreeSpreadRight(pos))
        {
            sapplingPositions.push_back(pos + kTreeWidth);
            // placeSappling(pos + kTreeWidth);
        }
    }

    // DOing this after so you can see the new sapplings
    for (const int pos : sapplingPositions)
    {
        // SDL_Log("Placing new sapplings");
        placeSappling(pos);
    }
}


// Private methods
bool ForestBiome::isTreeAtPos(int pos)
{
    return (mTrees.find(pos) != mTrees.end());
}

// Needs to be at least 4 empty spaces to left or right to spread more
bool ForestBiome::canTreeSpreadLeft(int treePosX)
{
    for (int i = 1; i <= 4; i++)
    {
        int newPosX = treePosX - i * kTreeWidth;
        if (newPosX <= 0 || isTreeAtPos(newPosX))
        {
            return false;
        }
    }
    return true;
}
bool ForestBiome::canTreeSpreadRight(int treePosX)
{
    for (int i = 1; i <= 4; i++)
    {
        int newPosX = treePosX + i * kTreeWidth;
        if (newPosX >= mBiomeLength || isTreeAtPos(newPosX))
        {
            return false;
        }
    }
    return true;
}


// Places sapplings to left or right if it can
void ForestBiome::placeSappling(int sapplingPosX)
{
    const float screenHeight = 1080.f;
    const float groundPosY = screenHeight - 270.f;

    Entity sappling = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            sappling,
            TransformComponent{
                .position = mBiomePos + Vector2(sapplingPosX, groundPosY - kSapplingSize),
            });
    gCoordinator.AddComponent(
            sappling,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, kSapplingSize, kSapplingSize},
                .path = "src/Assets/TreeSpriteSheet.png",
                .depth = 5
            });

    mBiomeEntities.push_back(sappling);
    mTrees.insert({sapplingPosX, sappling});
}


bool ForestBiome::canGrow(Entity entity)
{
    if (gCoordinator.HasComponent<BoxColliderComponent>(entity))
    {
        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);

        return collider.h <= kTreeWidth * kMaxTreeSegments;
    }

    return true;
}

void ForestBiome::growTree(Entity entity)
{
    auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
    auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);

    // if not sappling
    if (gCoordinator.HasComponent<BoxColliderComponent>(entity))
    {
        auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);

        transform.position.y -= kTreeWidth;
        collider.position.y -= kTreeWidth;
        collider.h += kTreeWidth;
        texture.spriteClip = SDL_FRect{kTreeTextureWidth * ((collider.h / kTreeWidth) - 1), 480.f - collider.h, kTreeTextureWidth, (float) collider.h};
    }
    // If sappling
    else
    {
        transform.position.y -= kTreeWidth - kSapplingSize;
        gCoordinator.AddComponent(
                entity, 
                BoxColliderComponent{
                    .position = transform.position + Vector2(0.f, -80.f),
                    .w = (int) kTreeTextureWidth,
                    .h = kTreeWidth * 2
                });
        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);

        texture.spriteClip = SDL_FRect{kTreeTextureWidth, 480.f - collider.h, kTreeTextureWidth, (float) collider.h};
    }
}
