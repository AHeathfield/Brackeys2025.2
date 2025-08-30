#pragma once

#include "../States/State.h"

struct StateChangerComponent
{
    State* newState = nullptr;
};
