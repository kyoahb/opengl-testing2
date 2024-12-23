#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <functional>
#include <vector>
#include <string>

class Menu {
public:
	std::function<void()> showFunction;
	std::string name;
	bool visible;
	Menu(std::function<void()> _showFunction, std::string _name);
	void show();
	void hide();
};