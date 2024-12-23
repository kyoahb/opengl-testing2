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
	bool disabled = false;
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
	bool disabled = false;
	std::function<void()> changeFunction = []() {};
	void change();
	void setVisibility(bool visible);

	Mouse(GLFWwindow* window);
private:
	GLFWwindow* window;
};

class InputManager {
public:
	InputManager(GLFWwindow* window);
	void addKey(Key* key);
	void deleteKey(Key* key);
	void update(double dTime);
	void setMouse(Mouse* mouse);
	Key* getKey(int keyCode);
	std::vector<Key*>* getKeys();
	Mouse* getMouse();

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); // static so it can be passed to glfw
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos); // static so it can be passed to glfw
	void key_call(int key, int scancode, int action, int mods);
	void mouse_call(double xpos, double ypos);
	void manageHeldKeys();

	GLFWwindow* window;
	Mouse* mouse;
	std::vector<Key*> keys;
	double deltaTime;
};
