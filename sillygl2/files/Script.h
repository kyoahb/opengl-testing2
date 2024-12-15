#pragma once

#include "Manager.h"

class Script {
public:
    virtual ~Script() = default;

    void initialize()
    {
        onStart();
    }

    virtual void onStart() = 0;
    virtual void onUpdate(double deltaTime) = 0;

protected:
};