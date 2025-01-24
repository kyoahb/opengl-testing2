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

    void startScripts() {
        for (auto& script : scripts) {
			spdlog::info("Starting script: {}", script->name);
            script->initialize();
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