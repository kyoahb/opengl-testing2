#include "Input.h"

Key::Key(int keyCode) : keyCode(keyCode), pressFunction([]() {}), holdFunction([]() {}) {}

void Key::press() {
	isHeld = true;
	if (pressFunction) {
		pressFunction();
	}
}

void Key::release() {
	isHeld = false;
}

InputManager::InputManager(Camera* camera, ObjectManager* objManager) : globalCamera(camera), objectManager(objManager) {
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
	static double lastX = xpos;
	static double lastY = ypos;
	static bool firstMouse = true;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	globalCamera->changeDirection(glm::vec3(yoffset, xoffset, 0.0f));
}

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

