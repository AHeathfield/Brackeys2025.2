#include "StateChangerButton.h"

extern Coordinator gCoordinator;
extern State* gCurrentState;


void StateChangerButton::Click()
{
    // Changes the current state
    // gCurrentState->Exit();
    // delete gCurrentState;
    gCurrentState = mNewState;
    // gCurrentState->Enter();
}
