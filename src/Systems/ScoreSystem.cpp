#include "ScoreSystem.h"
// #include "Shape.h"
#include <cmath>
#include <string>

extern Coordinator gCoordinator;

// Helper function
unsigned getNumberOfDigits (unsigned i)
{
    if (i > 0)
    {
        return ((int) log10((double) i) + 1);
    }
    else
    {
        return 1;
    }
}

void ScoreSystem::Init()
{
    // The scores
    score = 0;
    level = 0;
    lines = 0;

    // The entities
    mScore = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mScore,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/8bit16.ttf",
                .isText = true,
                .fontSize = 40,
                .text = "00000000",
            });
    gCoordinator.AddComponent(
            mScore, 
            TransformComponent{
                .position = Vector2(360.f, 600.f)
            });
    gCoordinator.AddComponent(
            mScore, 
            ScoreComponent{
            });


    mLevel = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mLevel,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/8bit16.ttf",
                .isText = true,
                .fontSize = 40,
                .text = "00000000",
            });
    gCoordinator.AddComponent(
            mLevel, 
            TransformComponent{
                .position = Vector2(360.f, 720.f)
            });
    gCoordinator.AddComponent(
            mLevel, 
            ScoreComponent{
            });


    mLines = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(
            mLines,
            TextureComponent{
                .texture = nullptr,
                .path = "src/Assets/8bit16.ttf",
                .isText = true,
                .fontSize = 40,
                .text = "00000000",
            });
    gCoordinator.AddComponent(
            mLines, 
            TransformComponent{
                .position = Vector2(360.f, 840.f)
            });
    gCoordinator.AddComponent(
            mLines, 
            ScoreComponent{
            });

}

// Helper funciton
std::string getNumberString(int digits, int gameNumber)
{
    std::string numberStr = "";
    for (int i = 0; i < 8 - digits; i++)
    {
        numberStr += "0";
    }

    return numberStr + std::to_string(gameNumber);

}

void ScoreSystem::Update(float deltaTime)
{
    int digitsScore = getNumberOfDigits(score);
    int digitsLevel = getNumberOfDigits(level);
    int digitsLines = getNumberOfDigits(lines);

    auto& scoreText = gCoordinator.GetComponent<TextureComponent>(mScore);
    auto& levelText = gCoordinator.GetComponent<TextureComponent>(mLevel);
    auto& linesText = gCoordinator.GetComponent<TextureComponent>(mLines);

    // Getting the actual string
    scoreText.text = getNumberString(digitsScore, score);
    levelText.text = getNumberString(digitsLevel, level);
    linesText.text = getNumberString(digitsLines, lines);
    // SDL_Log(mScoreText.text.c_str());

    // Might need to call loadmedia on rendersystem
    auto renderSystem = gCoordinator.GetSystem<RenderSystem>();
    renderSystem->LoadMedia(&scoreText);
    renderSystem->LoadMedia(&levelText);
    renderSystem->LoadMedia(&linesText);
}
