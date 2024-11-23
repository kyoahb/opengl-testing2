#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "Script.h"
#include <iostream>

class ExampleScript : public Script {
public:
    void onStart() override {
        std::cout << "ExampleScript started!" << std::endl;
        std::cout << objectManager->getObjects()->size() << std::endl;
    }

    void onUpdate(double deltaTime) override {
        //std::cout << "ExampleScript updating. Delta time: " << deltaTime << std::endl;
    }
};

#endif