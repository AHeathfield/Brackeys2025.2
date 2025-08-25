#pragma once

#include "Biome.h"
#include "../Core/Utility.h"
#include "../Components/TransformComponent.h"
#include "../Components/TextureComponent.h"
#include "../Systems/RenderSystem.h"

class ForestBiome : public Biome
{
    static constexpr float kTreeTextureWidth = 100;
    static constexpr int kTreeWidth = 80;
    static constexpr int kSapplingSize = 50;
    static constexpr int kMaxTreeSegments = 6;

public:
    void Init() override;
    void Grow() override;

private:
    void placeSappling(int sapplingPosX);
    void growTree(Entity entity);
    bool isTreeAtPos(int pos);
    bool canTreeSpreadLeft(int treePosX);
    bool canTreeSpreadRight(int treePosX);
    bool canGrow(Entity entity);

private:
    // First int represents its pos along along the biome
    std::unordered_map<int, Entity> mTrees;
};
