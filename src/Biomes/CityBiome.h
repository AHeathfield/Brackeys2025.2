#pragma once

#include "Biome.h"
#include "../Components/TransformComponent.h"
#include "../Components/TextureComponent.h"
#include "../Components/ScrollComponent.h"
#include <unordered_map>

class CityBiome : public Biome
{
    static constexpr int kSmallBuildingWidth = 80;
    static constexpr int kMediumBuildingWidth = 160;
    static constexpr int kLargeBuildingWidth = 320;
public:
    void Init() override;
    void Grow() override;
    // void Update() override;

private:
    bool canBuildSmall(int buildingPosX);
    bool canBuildMedium(int buildingPosX);
    bool canBuildLarge(int buildingPosX);
    bool isBuildingAtPos(int pos);
    void growSmallBuilding(Entity entity);
    void growMediumBuilding(Entity entity);
    void growLargeBuilding(Entity entity);

private:
    // First int represents its pos along along the biome
    std::unordered_map<int, Entity> mBuildings;
};
