#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <functional>
#include "Camera.h"
#include "Object.h"
#include "Useful.h"
class Key {
public:
	const int keyCode; // GLFW key code
	bool isHeld = false; // Whether the key is currently being held down
	bool disabled = false; // Whether the key is disabled and should not be checked for input
	std::function<void()> pressFunction; // Function to run when the key is pressed
	std::function<void()> holdFunction; // Function to run when the key is held down (every frame)
	std::function<void()> releaseFunction; // Function to run when the key is released

	Key(int keyCode); // Constructor
	void press(); // Run the press function
	void release(); // Run the release function

	// hold functions are managed inside a hold function update within the input manager, run every frame.

};

class Mouse {
public:
	double xPos; // Current x position of the mouse this frame
	double yPos; // Current y position of the mouse this frame
	double lastXPos; // Last frame's x position of the mouse
	double lastYPos; // Last frame's y position of the mouse

	bool disabled = false; // Whether the mouse is disabled and should not be checked for input
	bool returnFrame = false; // Whether returning to game mode from a window, will return xpos to normal. Prevents mouse jumps.
	bool visible = true; // Whether the mouse is visible
	std::function<void()> changeFunction = []() {}; // Function to run when the mouse moves

	void change(); // Run the change function
	void setVisibility(bool _visible); // Set the visibility of the mouse

	Mouse(GLFWwindow* window); // Constructor
private:
	GLFWwindow* window; // The window the mouse is attached to
};

class InputManager {
public:
	InputManager(GLFWwindow* window); // Constructor
	void addKey(Key* key); // Add a key to the input manager
	void deleteKey(Key* key); // Delete a key from the input manager
	void update(double dTime); // Update the input manager (run every frame)
	void setMouse(Mouse* mouse); // Set the mouse of the input manager
	Key* getKey(int keyCode); // Get a key by its key code
	std::vector<Key*>* getKeys(); // Get all tracked keys
	Mouse* getMouse(); // Get the mouse (if does not exist, return nullptr)

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); // static so it can be passed to glfw
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos); // static so it can be passed to glfw
	void key_call(int key, int scancode, int action, int mods); // Key callback, run everytime a key is pressed
	void mouse_call(double xpos, double ypos); // Mouse callback, run everytime the mouse moves
	void manageHeldKeys(); // Run the hold function of all held keys

	GLFWwindow* window; // The window the input manager is attached to
	Mouse* mouse = nullptr; // The mouse of the input manager
	std::vector<Key*> keys; // All tracked keys of the input manager
	double deltaTime;
};
