#pragma once

#include "State.h"
// #include "../Core/ECS.h"
// #include "../Core/Vector2.h"
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/Buttons/ButtonComponent.h"
#include "../Components/Buttons/StateChangerButton.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/AudioSystem.h"

class TitleState : public State
{
public:
    void Enter() override;
    void Exit() override;
    void HandleEvent(SDL_Event* e) override;
    void Update(float deltaTime) override;
    // int GetStateNumber() override;

private:
    void activateHoverColor(TextureComponent* texture);
    void activatePressColor(TextureComponent* texture);
    void disableEffectColor(TextureComponent* texture);
    bool isMouseInside(Vector2 mousePos, Vector2 buttonPos, const TextureComponent& texture);


private:
    // Entities
    Entity mBackground = MAX_ENTITIES + 1; // NULL
    Entity mPlayButton;
    std::vector<std::shared_ptr<System>> mSystemUpdateOrder;
    // int mStateNumber = -1; // Default value
};
