#pragma once

#include "Biome.h"
#include "../Core/Utility.h"
#include "../Components/TransformComponent.h"
#include "../Components/TextureComponent.h"
#include <set>
#include <string>

struct Pyramid
{
    std::vector<int> positions;
    int baseWidth = 0;
    int currentLeftIndex = 0;
    int currentRightIndex = 0;
    int currentLevel = 0;
    int maxLevel = 0;

    void AdjustIndexes()
    {
        currentLeftIndex = currentLevel;
        currentRightIndex = baseWidth - currentLevel - 1;
    }
    // void SetMaxLevel()
    // {
    //     maxLevel = (baseWidth + 1) / 2;
    // }
};

class DesertBiome : public Biome
{
    static constexpr int kSandStoneWidth = 120;
    static constexpr int kMaxBaseStones = 7;
    // static constexpr int kSapplingSize = 50;

public:
    void Init() override;
    void Grow() override;

private:
    void growPyramid(Pyramid& pyramid);
    void placeSandStone(int posX);
    void growStone(Entity entity); // like place another on top
    bool isStoneAtPos(int posX);
    // void fillLeft(int currentPosX);
    // void fillRight(int currentPosX);
    bool canGrow(const Pyramid& pyramid);

private:
    std::vector<Pyramid> mPyramids;

    // First int represents its pos along along the biome
    std::unordered_map<int, Entity> mStones;
    std::set<int> mGrowStonesRight;
    std::set<int> mGrowStonesLeft;
    std::vector<int> mNewStonePositions;
};
