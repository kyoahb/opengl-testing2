#pragma once

#include "Script.h"
#include <iostream>

class ControlScript : public Script {
public:
    void onStart() override {
		Key forward = Key(GLFW_KEY_W);
		forward.holdFunction = [this]() { this->movement("forward"); };
		inputManager->addKey(forward);

		Key left = Key(GLFW_KEY_A);
		left.holdFunction = [this]() { this->movement("left"); };
		inputManager->addKey(left);

		Key back = Key(GLFW_KEY_S);
		back.holdFunction = [this]() { this->movement("back"); };
		inputManager->addKey(back);

		Key right = Key(GLFW_KEY_D);
		right.holdFunction = [this]() { this->movement("right"); };
		inputManager->addKey(right);

		Key down = Key(GLFW_KEY_LEFT_SHIFT);
		down.holdFunction = [this]() { this->movement("down"); };
		inputManager->addKey(down);

		Key up = Key(GLFW_KEY_SPACE);
		up.holdFunction = [this]() { this->movement("up"); };
		inputManager->addKey(up);


		Key addCube = Key(GLFW_KEY_E);
		addCube.holdFunction = [this]() {
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

		Key speedUp = Key(GLFW_KEY_LEFT_CONTROL);
		speedUp.pressFunction = [this]() {
			speed = 10.0f;
			};
		speedUp.releaseFunction = [this]() {
			speed = 5.0f;
			};
		inputManager->addKey(speedUp);

		Mouse* m = new Mouse();
		m->changeFunction = [this, m]() {
			float sensitivity = 0.05f;

			double xoffset = (m->xPos - m->lastXPos) * sensitivity;
			double yoffset = (m->yPos - m->lastYPos) * sensitivity;

			camera->changeDirection(glm::vec3(static_cast<float>(yoffset), static_cast<float>(xoffset), 0.0f));
			m->lastXPos = m->xPos;
			m->lastYPos = m->yPos;
			};
		inputManager->setMouse(m);

    }

	void movement(std::string way) {
		glm::vec3 change = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = camera->direction;
		if (way == "forward") {
			change.z += (float)cos(direction.y * M_PI / 180);
			change.x += (float)sin(direction.y * M_PI / 180);
			change.y += (float)sin(-1 * direction.x * M_PI / 180);
		}
		else if (way == "back") {
			change.z -= (float)cos(direction.y * M_PI / 180);
			change.x -= (float)sin(direction.y * M_PI / 180);
			change.y -= (float)sin(-1 * direction.x * M_PI / 180);
		}
		else if (way == "left") {
			change.x += (float)cos(-1 * direction.y * M_PI / 180);
			change.z += (float)sin(-1 * direction.y * M_PI / 180);
		}
		else if (way == "right") {
			change.x -= (float)cos(-1 * direction.y * M_PI / 180);
			change.z -= (float)sin(-1 * direction.y * M_PI / 180);
		}
		else if (way == "up") {
			change.y += 1.0f;
		}
		else if (way == "down") {
			change.y -= 1.0f;
		}
		glm::vec3 overall = change * speed * (float)dTime;
		camera->move(overall);
	}

    void onUpdate(double deltaTime) override {
		dTime = deltaTime;
    }
private:
	double dTime;
	float speed = 5.0f;
};
