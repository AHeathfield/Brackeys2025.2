#pragma once

#include "../Core/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/TextureComponent.h"
#include <queue>
#include <set>


class AnimationSystem : public System
{
public:
    void Update(float deltaTime) override;

private:
    void UpdateAnimation(Animation& animation, TextureComponent& textureComponent);
    void UpdateImageAnimation(Animation& animation, TextureComponent& textureComponent);
};
