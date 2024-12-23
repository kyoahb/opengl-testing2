#pragma once

#include "Script.h"
#include <iostream>

class ControlScript : public Script {
public:
	double lastXPos = 0.0f;
	double lastYPos = 0.0f;
	bool inputDisabled = false;
    void onStart() override {
		Renderer* renderer = Manager::getInstance().getRenderer();
		ObjectManager* objectManager = Manager::getInstance().getObjectManager();
		InputManager* inputManager = Manager::getInstance().getInputManager();
		MenuManager* menuManager = Manager::getInstance().getMenuManager();
		GLFWwindow* window = Manager::getInstance().getWindow();


		camera = new Camera();
		renderer->setCamera(camera);

		player = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), "player", false);
		player->attachCamera(camera);
		objectManager->addObject(player);

		Key* forward = new Key(GLFW_KEY_W);
		forward->holdFunction = [this]() { this->movement("forward"); };
		inputManager->addKey(forward);

		Key* left = new Key(GLFW_KEY_A);
		left->holdFunction = [this]() { this->movement("left"); };
		inputManager->addKey(left);

		Key* back = new Key(GLFW_KEY_S);
		back->holdFunction = [this]() { this->movement("back"); };
		inputManager->addKey(back);

		Key* right = new Key(GLFW_KEY_D);
		right->holdFunction = [this]() { this->movement("right"); };
		inputManager->addKey(right);

		Key* down = new Key(GLFW_KEY_LEFT_SHIFT);
		down->holdFunction = [this]() { this->movement("down"); };
		inputManager->addKey(down);

		Key* up = new Key(GLFW_KEY_SPACE);
		up->holdFunction = [this]() { this->movement("up"); };
		inputManager->addKey(up);


		Key* addCube = new Key(GLFW_KEY_E);
		addCube->holdFunction = [this, objectManager]() {
			objectManager->addCube(0.5f, 0.5f, 0.5f, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)), "cube");
			};
		inputManager->addKey(addCube);

		Key* add2250Cubes = new Key(GLFW_KEY_P);
		add2250Cubes->pressFunction = [this, objectManager]() {
			for (int i = 0; i < 2250; i++) objectManager->addCube(0.5f, 0.5f, 0.5f, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)), "cube");
			};
		inputManager->addKey(add2250Cubes);

		Key* removeCube = new Key(GLFW_KEY_F);
		removeCube->holdFunction = [this, objectManager]() {
			objectManager->destroyObject(objectManager->getObjectByName("cube"));
			};
		inputManager->addKey(removeCube);

		Key* rotateCubes = new Key(GLFW_KEY_Q);
		rotateCubes->holdFunction = [this, objectManager]() {
			objectManager->rotateObjectsR(objectManager->getObjectListByName("cube"), (float)dTime * glm::vec3(360.0f, 0.0f, 0.0f));
			};
		inputManager->addKey(rotateCubes);

		Key* speedUp = new Key(GLFW_KEY_LEFT_CONTROL);
		speedUp->pressFunction = [this]() {
			speed = 10.0f;
			};
		speedUp->releaseFunction = [this]() {
			speed = 5.0f;
			};
		inputManager->addKey(speedUp);

		lastXPos = 0.0f;
		lastYPos = 0.0f;
		Mouse* m = new Mouse(window);
		m->changeFunction = [this, m]() {
			float sensitivity = 0.05f;

			double xoffset = (m->xPos - this->lastXPos) * sensitivity;
			double yoffset = (m->yPos - this->lastYPos) * sensitivity;

			camera->changeDirection(glm::vec3(static_cast<float>(yoffset), static_cast<float>(xoffset), 0.0f));
			this->lastXPos = m->xPos;
			this->lastYPos = m->yPos;
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
		player->move(overall);
	}

    void onUpdate(double deltaTime) override {
		dTime = deltaTime;
    }
private:
	double dTime = 0.0f;
	float speed = 5.0f;
	Camera* camera;
	GameObject* player;
};
