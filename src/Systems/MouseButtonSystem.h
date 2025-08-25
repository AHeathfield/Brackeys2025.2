#pragma once

#include <SDL3/SDL.h>

#include "../Core/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/TextureComponent.h"
#include "../Components/Buttons/ButtonComponent.h"

class MouseButtonSystem : public System
{
public:
    void HandleEvent(SDL_Event* e);
    bool isMouseInside(Vector2 mousePos, Vector2 buttonPos, const TextureComponent& texture);

    void activateHoverColor(TextureComponent* texture);
    void activatePressColor(TextureComponent* texture);
    void disableEffectColor(TextureComponent* texture);
};
