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

Mouse::Mouse(GLFWwindow* window) : window(window), xPos(0.0f), yPos(0.0f), changeFunction([]() {}) {
	this->setVisibility(false);
}

void Mouse::change() { // change = moved 
	if (changeFunction) {
		this->changeFunction();
	}
}

void Mouse::setVisibility(bool visible) {
	if (visible) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

InputManager::InputManager(GLFWwindow* window) : 
	deltaTime(0.0f), mouse(nullptr), window(window) {

	// Store the pointer to this instance in the GLFW window's user pointer
	glfwSetWindowUserPointer(window, this);

	// Set the static callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void InputManager::addKey(Key* key) {
	keys.emplace_back(key);
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Retrieve the InputManager instance from the GLFW window's user pointer
	InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	if (inputManager) {
		inputManager->key_call(key, scancode, action, mods);
	}
}
void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	// Retrieve the InputManager instance from the GLFW window's user pointer
	InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	if (inputManager) {
		inputManager->mouse_call(xpos, ypos);
	}
}


void InputManager::key_call(int key, int scancode, int action, int mods) {
	Key* k = getKey(key);
	if (k->disabled) {
		return;
	}
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

void InputManager::mouse_call(double xpos, double ypos) {
	if (mouse) {
		if (mouse->disabled) return;
		mouse->xPos = xpos;
		mouse->yPos = ypos;

		mouse->change();
	} 
}

void InputManager::update(double dTime) {
	deltaTime = dTime;
	manageHeldKeys();
}

void InputManager::manageHeldKeys() {
	for (auto& key : keys) {
		if (key->isHeld) {
			if (key->holdFunction) {
				key->holdFunction();
			}
		}
	}
}

Key* InputManager::getKey(int keyCode) {
	for (auto& key : keys) {
		if (key->keyCode == keyCode) {
			return key;
		}
	}
	return nullptr;
}

std::vector<Key*>* InputManager::getKeys() {
	return &keys;
}

Mouse* InputManager::getMouse() {
	return mouse;
}

void InputManager::setMouse(Mouse* m) {
	this->mouse = m;
}