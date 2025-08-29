#include "PhysicsSystem.h"

extern Coordinator gCoordinator;

void PhysicsSystem::Update(float deltaTime)
{
    for (const auto& entity : mEntities)
    {
        // SDL_Log("Applying physics");
        auto& collider = gCoordinator.GetComponent<BoxColliderComponent*>(entity);
        auto& kinematics = gCoordinator.GetComponent<KinematicsComponent>(entity);

        // Move the collider based on velocity
        collider->position.addScaledVector(kinematics.velocity, deltaTime);

        // adjust the vel based on acceleration
        kinematics.integrate(deltaTime);
    }
}
