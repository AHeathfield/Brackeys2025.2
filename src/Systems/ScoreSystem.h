#pragma once

#include "../Core/ECS.h"
#include "../Components/ScoreComponent.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "RenderSystem.h"

class ScoreSystem : public System
{
public:
    void Init();
    void Update(float deltaTime);

    int score;
    int level;
    int lines;

private:
    Entity mScore, mLevel, mLines;
    TextureComponent mScoreText, mLevelText, mLinesText;
};
