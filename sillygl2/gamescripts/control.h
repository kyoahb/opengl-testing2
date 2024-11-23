#pragma once

#include "Script.h"
#include <iostream>

class ControlScript : public Script {
public:
    void onStart() override {
		Key forward = Key(GLFW_KEY_W);
		forward.holdFunction = [this]() { camera->move("forward", dTime); };
		inputManager->addKey(forward);

		Key left = Key(GLFW_KEY_A);
		left.holdFunction = [this]() { camera->move("left", dTime); };
		inputManager->addKey(left);

		Key back = Key(GLFW_KEY_S);
		back.holdFunction = [this]() { camera->move("back", dTime); };
		inputManager->addKey(back);

		Key right = Key(GLFW_KEY_D);
		right.holdFunction = [this]() { camera->move("right", dTime); };
		inputManager->addKey(right);

		Key addCube = Key(GLFW_KEY_E);
		addCube.pressFunction = [this]() {
			objectManager->addCube(0.5f, 0.5f, 0.5f, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)), "cube");
			};
		inputManager->addKey(addCube);

		Key rotateCubes = Key(GLFW_KEY_Q);
		rotateCubes.holdFunction = [this]() {
			objectManager->rotateObjectsR(objectManager->getObjectListByName("cube"), (float)dTime * glm::vec3(360.0f, 0.0f, 0.0f));
			};
		inputManager->addKey(rotateCubes);

		Key scaleCubes = Key(GLFW_KEY_R);
		scaleCubes.pressFunction = [this]() {
			for (auto& object : objectManager->getObjectListByName("cube")) {
				object->scale(glm::vec3(2.0f, 2.0f, 2.0f));
			}
			};
		inputManager->addKey(scaleCubes);

		Key moveCube = Key(GLFW_KEY_T);
		moveCube.pressFunction = [this]() {
			GameObject* c = objectManager->getObjectByName("cube");
			c->move(glm::vec3(0.0f, 0.0f, 1.0f));
			};
		inputManager->addKey(moveCube);
    }

    void onUpdate(double deltaTime) override {
		dTime = deltaTime;
        //std::cout << "ExampleScript updating. Delta time: " << deltaTime << std::endl;
    }
private:
	double dTime;
};
