#pragma once

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
