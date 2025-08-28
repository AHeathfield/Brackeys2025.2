#include "Camera.h"
#include "ECS.h"
#include "Vector2.h"

extern Coordinator gCoordinator;

Camera::Camera(const Vector2& position, const Vector2& offset)
    : mPosition(position),
    mOffset(offset)
{}


void Camera::FollowEntity(Entity entity)
{
    if (gCoordinator.HasComponent<TransformComponent>(entity))
    {
        mEntityFollowing = entity;
        mIsFollowing = true;
    }
    else
    {
        SDL_Log("Cannot follow entity without a TransformComponent!");
    }
}


void Camera::StopFollowing()
{
    mIsFollowing = false;
}


Vector2 Camera::GetPosition()
{
    return mPosition;
}

Vector2 Camera::GetOffset()
{
    return mOffset;
}

void Camera::SetPosition(const Vector2& pos)
{
    mPosition = pos;
}

void Camera::SetOffset(const Vector2& offset)
{
    mOffset = offset;
}



void Camera::Update()
{
    // For now we are just centering camera on entity
    if (mIsFollowing)
    {
        const auto& transform = gCoordinator.GetComponent<TransformComponent>(mEntityFollowing);
        mPosition.x = transform.position.x;
        mPosition.y = transform.position.y;
    }
}
