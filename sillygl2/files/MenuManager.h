#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Menu.h"
#include <vector>

class MenuManager {
public:
	bool showDemoWindow = false;
	MenuManager(GLFWwindow* window);

	void addMenu(Menu* menu);
	Menu* getMenuByName(std::string name);
	void frameStart();
	void frameEnd();
	void shutdown();
private:
	std::vector<Menu*> menus;

};
