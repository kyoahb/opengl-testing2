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
		addCube->pressFunction = [this, objectManager]() {
			objectManager->addCube(0.5f, 0.5f, 0.5f, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)), "cube");
			};
		inputManager->addKey(addCube);

		Key* add2250Cubes = new Key(GLFW_KEY_P);
		add2250Cubes->pressFunction = [this, objectManager]() {
			for (int i = 0; i < 2250; i++) objectManager->addCube(0.5f, 0.5f, 0.5f, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)), "cube");
			};
		inputManager->addKey(add2250Cubes);

		Key* removeCube = new Key(GLFW_KEY_F);
		removeCube->pressFunction = [this, objectManager]() {
			objectManager->destroyObject(objectManager->getObjectByName("cube"));
			};
		inputManager->addKey(removeCube);

		Key* rotateCubes = new Key(GLFW_KEY_Q);
		rotateCubes->holdFunction = [this, objectManager]() {
			glm::vec3 rotation = (float)dTime * glm::vec3(360.0f, 0.0f, 0.0f);
			std::vector<GameObject*> cubes = objectManager->getObjectListByName("cube");
			objectManager->rotateObjectsR(cubes, rotation);
			};
		inputManager->addKey(rotateCubes);

		Key* rotateSingularCube = new Key(GLFW_KEY_R);
		rotateSingularCube->holdFunction = [this, objectManager]() {
			glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
			objectManager->getObjectByName("cube")->rotate(rotation);
			};
		inputManager->addKey(rotateSingularCube);

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

			double xoffset = (m->xPos - m->lastXPos) * sensitivity;
			double yoffset = (m->yPos - m->lastYPos) * sensitivity;

			camera->changeDirection(glm::vec3(static_cast<float>(yoffset), static_cast<float>(xoffset), 0.0f));
			};
		inputManager->setMouse(m);


    }

	void movement(std::string way) { // Inefficient but I DONT CARE! for now
		glm::vec3 change = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = camera->getDirection();
        if (way == "forward") {
			change.z += (float)cos(glm::radians(direction.y));
			change.x += (float)sin(glm::radians(direction.y));
			change.y += (float)cos(glm::radians(direction.x));
        }
        else if (way == "back") {
			change.z -= (float)cos(glm::radians(direction.y));
			change.x -= (float)sin(glm::radians(direction.y));
			change.y -= (float)cos(glm::radians(direction.x));
        }
		else if (way == "left") {
			change.x += (float)cos(glm::radians(-direction.y));
			change.z += (float)sin(glm::radians(-direction.y));
		}
		else if (way == "right") {
			change.x -= (float)cos(glm::radians(-direction.y));
			change.z -= (float)sin(glm::radians(-direction.y));
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
