#pragma once

// Need to think about how an Entity will have a specific type of ButtonComponent...
class ButtonComponent
{
public:
    virtual void Click() = 0;
};
