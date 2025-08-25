#pragma once

#include "../../Core/ECS.h"
#include "../../States/State.h"
#include "../../States/PlayState.h"
#include "ButtonComponent.h"

class StateChangerButton : public ButtonComponent
{
public:
    StateChangerButton(State* newState)
        : mNewState(newState)
    {}

    void Click() override;

private: 
    State* mNewState;
};

