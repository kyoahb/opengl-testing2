#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <functional>
#include "Camera.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Useful.h"

class Key {
public:
	const int keyCode;
	bool isHeld = false;
	bool immortal = false; // If immortal, it cannot be disabled even when INPUT_DISABLED is True.
	std::function<void()> pressFunction;
	std::function<void()> holdFunction;
	std::function<void()> releaseFunction;

	Key(int keyCode);
	void press();
	void release();

	// hold functions are managed inside a hold function update within the input manager, run every frame.

};

class Mouse {
public:
	double xPos;
	double yPos;
	double lastXPos;
	double lastYPos;
	std::function<void()> changeFunction = []() {};
	void change();

	Mouse();
};

class InputManager {
public:
	bool INPUT_DISABLED = false;

	InputManager(ObjectManager* objManager);
	void addKey(Key key);
	void key_call(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_call(GLFWwindow* window, double xpos, double ypos);
	void update(double dTime);
	void manageHeldKeys();
	Key* getKey(int keyCode);
	Mouse* getMouse();
	void setMouse(Mouse* mouse);
private:
	ObjectManager* objectManager;
	Mouse* mouse;
	std::vector<Key> keys;
	double deltaTime;
};