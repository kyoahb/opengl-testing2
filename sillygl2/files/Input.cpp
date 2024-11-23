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

	Key forward = Key(GLFW_KEY_W);
	forward.holdFunction = [this]() { globalCamera->move("forward"); };
	addKey(forward);

	Key left = Key(GLFW_KEY_A);
	left.holdFunction = [this]() { globalCamera->move("left"); };
	addKey(left);

	Key back = Key(GLFW_KEY_S);
	back.holdFunction = [this]() { globalCamera->move("back"); };
	addKey(back);

	Key right = Key(GLFW_KEY_D);
	right.holdFunction = [this]() { globalCamera->move("right"); };
	addKey(right);

	Key addCube = Key(GLFW_KEY_E);
	addCube.pressFunction = [this]() {
		objectManager->addCube(0.5f, 0.5f, 0.5f, glm::vec3(rand_float(-5, 5), rand_float(-5, 5), rand_float(-5, 5)), "cube");
		};
	addKey(addCube);

	Key rotateCubes = Key(GLFW_KEY_Q);
	rotateCubes.holdFunction = [this]() {
		objectManager->rotateObjectsR(objectManager->getObjectListByName("cube"), (float)deltaTime * glm::vec3(360.0f, 0.0f, 0.0f));
		};
	addKey(rotateCubes);

	Key scaleCubes = Key(GLFW_KEY_R);
	scaleCubes.pressFunction = [this]() {
		for (auto& object : objectManager->getObjectListByName("cube")) {
			object->scale(glm::vec3(2.0f, 2.0f, 2.0f));
		}
		};
	addKey(scaleCubes);

	Key moveCube = Key(GLFW_KEY_T);
	moveCube.pressFunction = [this]() {
		GameObject* c = objectManager->getObjectByName("cube");
		c->move(glm::vec3(0.0f, 0.0f, 1.0f));
		};
	addKey(moveCube);

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

