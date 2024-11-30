#pragma once

#include "ObjectManager.h"
#include "Input.h"
#include "Camera.h"
#include "Renderer.h"

class Script {
public:
    virtual ~Script() = default;

    void initialize(InputManager* inputManager, ObjectManager* objectManager, Renderer* renderer)
    {
        this->inputManager = inputManager;
        this->objectManager = objectManager;
        this->renderer = renderer;
        onStart();
    }

    virtual void onStart() = 0;
    virtual void onUpdate(double deltaTime) = 0;

protected:
    InputManager* inputManager;
    ObjectManager* objectManager;
    Renderer* renderer;
};