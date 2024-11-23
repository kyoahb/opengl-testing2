#pragma once

#include <vector>
#include <string>
#include "Script.h"

class ScriptManager {
public:
    ~ScriptManager() {
        for (auto& script : scripts) {
            delete script;
        }
    }

    void registerScript(Script* script) {
        scripts.push_back(script);
    }

    void startScripts(InputManager* inputManager, ObjectManager* objectManager, Camera* camera, Renderer* renderer) {
        for (auto& script : scripts) {
            script->initialize(inputManager, objectManager, camera, renderer);
        }
    }

    void updateScripts(double deltaTime) {
        for (auto& script : scripts) {
            script->onUpdate(deltaTime);
        }
    }

private:
    std::vector<Script*> scripts;
};