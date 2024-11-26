#include "Input.h"

Key::Key(int keyCode) : keyCode(keyCode), pressFunction([]() {}), holdFunction([]() {}), releaseFunction([]() {}) {}

void Key::press() {
	isHeld = true;
	if (pressFunction) {
		pressFunction();
	}
}

void Key::release() {
	isHeld = false;
	if (releaseFunction) {
		releaseFunction();
	}
}

void Mouse::change() {
	if (changeFunction) {
		this->changeFunction();
	}
}

InputManager::InputManager(Camera* camera, ObjectManager* objManager) : globalCamera(camera), objectManager(objManager), mouseExists(false) {
	if (globalCamera == nullptr) {
		std::cerr << "Error: globalCamera is null" << std::endl;
	}

}

void InputManager::addKey(Key key) {
	keys.emplace_back(key);
}

void InputManager::key_call(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Key* k = getKey(key);
	if (k == nullptr) {
		return;
	}

	if (action == GLFW_PRESS) {
		k->press();
	}
	else if (action == GLFW_RELEASE) {
		k->release();
	}
}

void InputManager::mouse_call(GLFWwindow* window, double xpos, double ypos) {
	if (mouse) {
		mouse->xPos = xpos;
		mouse->yPos = ypos;

		mouse->change();
	} 
}

Mouse::Mouse() : xPos(0.0f), yPos(0.0f), lastXPos(0.0f), lastYPos(0.0f), changeFunction([]() {}) {};


void InputManager::update(double dTime) {
	deltaTime = dTime;
	manageHeldKeys();
}

void InputManager::manageHeldKeys() {
	for (auto& key : keys) {
		if (key.isHeld) {
			if (key.holdFunction) {
				key.holdFunction();
			}
		}
	}
}

Key* InputManager::getKey(int keyCode) {
	for (auto& key : keys) {
		if (key.keyCode == keyCode) {
			return &key;
		}
	}
	return nullptr;
}

Mouse* InputManager::getMouse() {
	return mouse;
}

void InputManager::setMouse(Mouse* m) {
	mouseExists = true;
	this->mouse = m;
}