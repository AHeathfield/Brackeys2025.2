#pragma once

#include "../Core/Vector2.h"

struct TransformComponent
{
    Vector2 position = Vector2();
    Vector2 rotation = Vector2();
    Vector2 scale = Vector2();
};
