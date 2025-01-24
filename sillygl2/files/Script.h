#pragma once

#include "Manager.h"
#include "spdlog/spdlog.h"

class Script {
public:
    std::string name = "";
    Script(const std::string& scriptName) : name(scriptName) {}
    virtual ~Script() = default;

    void initialize()
    {
        onStart();
    }

    virtual void onStart() = 0;
    virtual void onUpdate(double deltaTime) = 0;

protected:
};