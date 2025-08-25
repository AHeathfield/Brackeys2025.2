#include "StartingBiome.h"

extern Coordinator gCoordinator;

void StartingBiome::Init()
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
                .path = "src/Assets/Starting.png",
                .depth = 1,
                .width = 7680,
                .height = 270
            });
    // gCoordinator.AddComponent(
    //         ground,
    //         ScrollComponent{
    //         });
    mBiomeEntities.push_back(ground);
}

void StartingBiome::Grow()
{

}
