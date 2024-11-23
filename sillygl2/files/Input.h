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
	std::function<void()> pressFunction;
	std::function<void()> holdFunction;

	Key(int keyCode);
	void press();
	void release();

	// hold functions are managed inside a hold function update within the input manager, run every frame.

};

class InputManager {
public:
	InputManager(Camera* camera, ObjectManager* objManager);
	void addKey(Key key);
	void key_call(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_call(GLFWwindow* window, double xpos, double ypos);
	void update(double dTime);
	void manageHeldKeys();
	Key* getKey(int keyCode);
private:
	Camera* globalCamera;
	ObjectManager* objectManager;
	std::vector<Key> keys;
	double deltaTime;
};