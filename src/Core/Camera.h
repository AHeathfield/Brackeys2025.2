#pragma once

#include "ECS.h"
#include "Vector2.h"
#include "../Components/TransformComponent.h"

class Camera
{
public:
    Camera(const Vector2& position = Vector2(), const Vector2& offset = Vector2());

    void FollowEntity(Entity entity);
    void StopFollowing();
    void SetPosition(const Vector2& pos);
    void SetOffset(const Vector2& offset);
    void Update();

    Vector2 GetPosition();
    Vector2 GetOffset();

private:
    Vector2 mPosition;
    Vector2 mOffset;

    Entity mEntityFollowing;
    bool mIsFollowing = false;
    // Going to add more stuff like deadzones eventually
};
