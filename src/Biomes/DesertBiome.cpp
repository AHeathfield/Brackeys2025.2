#include "DesertBiome.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <string>
#include <vector>

extern Coordinator gCoordinator;

void DesertBiome::Init()
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
                .path = "src/Assets/Desert.png",
                .depth = 1,
                .width = 7680,
                .height = 270
            });
    mBiomeEntities.push_back(ground);


    // Creating the Tree texture (mini asset manager)
    // TextureComponent sandStoneTextureComponent = {
    //     .texture = nullptr,
    //     .path = "src/Assets/SandStoneSpriteSheet.png"
    // };
    // mRenderSystem->LoadMedia(&sandStoneTextureComponent);
    // mSandStoneTexture = sandStoneTextureComponent.texture;


    // Initial stones + Grow Stones
    // We want max base to equal 10 stones
    // Possibly we may want base to be an even number of stones too
    int posX = 0;
    int tempStartX = 0;
    int tempEndX = 0;
    int gap = 0;
    int base = 0;
    int count = 0;
    for (int i = 0; i < mBiomeLength / kSandStoneWidth; i++)
    {
        // Forcing base length to be 3, 5, 7...
        // base++;
        if ((utility::getTrueOneXChance(8) == true && base > 1 && utility::isIntEven(base)) || (base == kMaxBaseStones - 1) || (base == 0 && gap > 3))
        {
            gap = 0;
            count++;
            posX = i * kSandStoneWidth;
            placeSandStone(posX);

            if (base == 0)
            {
                base = 1;
            }
            else
            {
                base++;
            }

            if (count == 1)
            {
                tempStartX = posX;
            }
            else if (count == 2)
            {
                tempEndX = posX;
            }
        }
        else
        {
            if (base != 0)
            {
                base++;
            }
            gap++;
        }

        // Create the pyramid
        if (count == 2)
        {
            count = 0;
            std::vector<int> tempPositions;
            utility::initVectorInt(tempPositions, base, -1);
            tempPositions.front() = tempStartX;
            tempPositions.back() = tempEndX;

            Pyramid newPyramid = {
                .positions = tempPositions,
                .baseWidth = base,
                .currentLeftIndex = 1,
                .currentRightIndex = base - 2,
                .currentLevel = 0,
                .maxLevel = (base + 1) / 2
            };

            mPyramids.push_back(newPyramid);
            // std::string log = "Base: " + std::to_string(base) + ", ArraySize: " + std::to_string(tempPositions.size());
            // SDL_Log(log.c_str());
            base = 0;
        }
    }
}

void DesertBiome::Grow()
{
    for (auto& pyramid : mPyramids)
    {
        if (canGrow(pyramid))
        {
            growPyramid(pyramid);
        }
    }
}


// Private methods
// CODE HERE IS GARBAGE BUT WE LOW ON TIME LOOOL
void DesertBiome::growPyramid(Pyramid& pyramid)
{
    int startPosX = pyramid.positions.front();
    int leftIndex = pyramid.currentLeftIndex;
    int rightIndex = pyramid.currentRightIndex;
    // std::string log = "LeftIndex: " + std::to_string(leftIndex) + ", RightIndex: " + std::to_string(rightIndex);
    // SDL_Log(log.c_str());

    if (leftIndex == rightIndex)
    {
        // SDL_Log("Placing middle block!");
        if (pyramid.positions[leftIndex] == -1)
        {
            int newPosX = startPosX + (kSandStoneWidth * leftIndex );
            pyramid.positions[leftIndex] = newPosX;
            placeSandStone(newPosX);
        }
        else
        {
            Entity stone = mStones[pyramid.positions[leftIndex]];
            growStone(stone);
        }

        // Putting the indexes in the correct place
        pyramid.currentLevel++;
        pyramid.AdjustIndexes();
    }

    else
    {
        // LEFT SIDE
        if (pyramid.positions[leftIndex] == -1)
        {
            int newPosX = startPosX + (kSandStoneWidth * leftIndex);
            pyramid.positions[leftIndex] = newPosX;
            placeSandStone(newPosX);
        }
        else
        {
            Entity stone = mStones[pyramid.positions[leftIndex]];
            growStone(stone);
        }
        pyramid.currentLeftIndex++;


        // RIGHT SIDE
        if (pyramid.positions[rightIndex] == -1)
        {
            int newPosX = startPosX + (kSandStoneWidth * rightIndex);
            pyramid.positions[rightIndex] = newPosX;
            placeSandStone(newPosX);
        }
        else
        {
            Entity stone = mStones[pyramid.positions[rightIndex]];
            growStone(stone);
        }
        pyramid.currentRightIndex--;
    }
}

bool DesertBiome::isStoneAtPos(int posX)
{
    return (mStones.find(posX) != mStones.end());
}


// Places sapplings to left or right if it can
void DesertBiome::placeSandStone(int posX)
{
    const float screenHeight = 1080.f;
    const float groundPosY = screenHeight - 270.f;

    Entity sandStone = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            sandStone,
            TransformComponent{
                .position = mBiomePos + Vector2(posX, groundPosY - kSandStoneWidth),
            });
    gCoordinator.AddComponent(
            sandStone,
            TextureComponent{
                .texture = nullptr,
                .spriteClip = SDL_FRect{0.f, 0.f, kSandStoneWidth, kSandStoneWidth},
                .path = "src/Assets/SandStoneSpriteSheet.png",
                .depth = 5
            });
    gCoordinator.AddComponent(
            sandStone,
            BoxColliderComponent{
                .position = mBiomePos + Vector2(posX, groundPosY - kSandStoneWidth),
                .w = kSandStoneWidth,
                .h = kSandStoneWidth
            });

    mBiomeEntities.push_back(sandStone);
    mStones.insert({posX, sandStone});
}


bool DesertBiome::canGrow(const Pyramid& pyramid)
{
    return (pyramid.currentLevel < pyramid.maxLevel);
}

void DesertBiome::growStone(Entity entity)
{
    auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
    auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);
    auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);


    transform.position.y -= kSandStoneWidth;
    collider.position.y -= kSandStoneWidth;
    collider.h += kSandStoneWidth;
    texture.spriteClip = SDL_FRect{0.f, 0.f, (float) kSandStoneWidth, (float) collider.h};
}
