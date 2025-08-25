#pragma once

#include "Biome.h"
#include "../Components/TransformComponent.h"
#include "../Components/TextureComponent.h"
#include "../Components/ScrollComponent.h"

class StartingBiome : public Biome
{
public:
    void Init() override;
    void Grow() override;
    // void Update() override;
};
