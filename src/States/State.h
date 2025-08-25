#pragma once

#include "../Core/ECS.h"
#include <SDL3/SDL_events.h>

/*
 * State will contain the entities for that state.
 * - It will need to first initialize all entities
 * - It will also need to delete all entities when switching states
 */
class State
{
public:
    virtual ~State() = default; // So we can delete the reference
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void HandleEvent(SDL_Event* e) = 0;
    virtual void Update() = 0;
    // virtual int GetStateNumber() = 0;

    // static int stateNumber;
};
