#include "CityBiome.h"
#include <SDL3/SDL_rect.h>
#include <cstdlib>
#include <unordered_map>

extern Coordinator gCoordinator;

// Helper method that randomly returns true or false
bool randomBool()
{
    int num = rand() % 2;
    if (num == 0)
    {
        return false;
    }
    return true;
}

void CityBiome::Init()
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
                .path = "src/Assets/CityGround.png",
                .depth = 1,
                .width = 7680,
                .height = 270
            });
    gCoordinator.AddComponent(
            ground,
            ScrollComponent{
            });
    mBiomeEntities.push_back(ground);


    // Initial Huts
    const int maxCityWidth = 320;
    const int groundPosY = screenHeight - 270.f;
    int hutPos = 0;
    int prevHutPos = 0;
    Entity hut;
    for (int i = 0; i < mBiomeLength / maxCityWidth; i++)
    {
        if (randomBool() == true)
        {
            // Setting the hut pos
            hutPos = (rand() % 4) + 1;
            if (hutPos < prevHutPos)
            {
                hutPos = prevHutPos;
            }

            // This makes sure that the last city doesn't bleed into the next biome
            if (i == (mBiomeLength / maxCityWidth) - 1)
            {
                if (prevHutPos != 0)
                {
                    break;
                }
            }

            // Creating the entity
            hut = gCoordinator.CreateEntity();
            gCoordinator.AddComponent(
                    hut,
                    TransformComponent{
                        .position = mBiomePos + Vector2(i * 320.f + (hutPos * 80.f), groundPosY - 80.f),
                    });
            gCoordinator.AddComponent(
                    hut,
                    TextureComponent{
                        .texture = nullptr,
                        .spriteClip = SDL_FRect{0.f, 0.f, 80.f, 80.f},
                        .path = "src/Assets/CitySpriteSheet.png",
                        .depth = 5,
                        .width = 80,
                        .height = 80
                    });
            gCoordinator.AddComponent(
                    hut,
                    BoxColliderComponent{
                        .position = mBiomePos + Vector2(i * 320.f + (hutPos * 80.f), groundPosY - 80.f),
                        .w = 80,
                        .h = 80
                    });

            mBiomeEntities.push_back(hut);
            mBuildings.insert({(i * 320 + hutPos * 80), hut});
            prevHutPos = hutPos;
        }
    }
}


// Private Helper method to see if an unorder map contains a key
bool CityBiome::isBuildingAtPos(int pos)
{
    return (mBuildings.find(pos) != mBuildings.end());
}

// Private Helper method to check if there is any other huts in range
bool CityBiome::canBuildSmall(int buildingPosX)
{
    return (isBuildingAtPos(buildingPosX + 320)); //|| isBuildingAtPos(buildingPosX - 320));
}
bool CityBiome::canBuildMedium(int buildingPosX)
{
    if (isBuildingAtPos(buildingPosX + 320))
    {
        Entity next = mBuildings[buildingPosX + 320];
        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(next);
        return (collider.w == kMediumBuildingWidth);
    }
    return false;
}
bool CityBiome::canBuildLarge(int buildingPosX)
{
    if (isBuildingAtPos(buildingPosX + 320))
    {
        Entity current = mBuildings[buildingPosX];
        Entity next = mBuildings[buildingPosX + 320];

        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(current);
        const auto& colliderNext = gCoordinator.GetComponent<BoxColliderComponent>(next);
        return ((colliderNext.w == kLargeBuildingWidth) && (collider.h == kLargeBuildingWidth));
    }
    return false;
}


// THESE PRIVATE HELPER MEHTODS ADJUST THE COMPONENTS TO GROW THE BUILDING
void CityBiome::growSmallBuilding(Entity entity)
{
    auto& transfrom = gCoordinator.GetComponent<TransformComponent>(entity);
    auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
    auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);

    // Change this if we want the dimensions diff
    transfrom.position.y -= 80.f;
    collider.position.y -= 80.f;
    collider.w = kMediumBuildingWidth;
    collider.h = kMediumBuildingWidth;
    texture.spriteClip = SDL_FRect{(float) kSmallBuildingWidth, 0.f, (float) kMediumBuildingWidth, (float) kMediumBuildingWidth};
}
void CityBiome::growMediumBuilding(Entity entity)
{
    auto& transfrom = gCoordinator.GetComponent<TransformComponent>(entity);
    auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
    auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);

    // Change this if we want the dimensions diff
    transfrom.position.y -= 160.f;
    collider.position.y -= 160.f;
    collider.w = kLargeBuildingWidth;
    collider.h = kLargeBuildingWidth;
    texture.spriteClip = SDL_FRect{(float) (kMediumBuildingWidth + kMediumBuildingWidth), 0.f, (float) kLargeBuildingWidth, (float) kLargeBuildingWidth};

}
void CityBiome::growLargeBuilding(Entity entity)
{
    auto& transfrom = gCoordinator.GetComponent<TransformComponent>(entity);
    auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);
    auto& texture = gCoordinator.GetComponent<TextureComponent>(entity);

    // Change this if we want the dimensions diff
    transfrom.position.y -= 320.f;
    collider.position.y -= 320.f;
    collider.w = kLargeBuildingWidth;
    collider.h = kLargeBuildingWidth * 2.f;
    texture.spriteClip = SDL_FRect{(float) (kMediumBuildingWidth + kMediumBuildingWidth), 0.f, (float) kLargeBuildingWidth, (float) kLargeBuildingWidth * 2.f};
}

void CityBiome::Grow()
{
    for (auto building = mBuildings.begin(); building != mBuildings.end(); building++)
    {
        int pos = building->first;
        Entity entity = building->second;
        const auto& collider = gCoordinator.GetComponent<BoxColliderComponent>(entity);

        switch (collider.w)
        {
            case kSmallBuildingWidth:
                if (canBuildSmall(pos))
                {
                    growSmallBuilding(entity);
                }
                break;
            case kMediumBuildingWidth:
                if (canBuildMedium(pos))
                {
                    growMediumBuilding(entity);
                }
                break;
            case kLargeBuildingWidth:
                if (canBuildLarge(pos))
                {
                    growLargeBuilding(entity);
                }
                break;
        }
    }
}
