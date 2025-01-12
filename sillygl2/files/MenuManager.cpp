#include "MenuManager.h"
#include "Manager.h"

MenuManager::MenuManager(GLFWwindow* window) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	io.WantCaptureKeyboard = false;
	io.WantCaptureMouse = false;
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
}

void MenuManager::addMenu(Menu* menu) {
	menus.push_back(menu);
}

Menu* MenuManager::getMenuByName(std::string name) {
	for (auto menu : menus) {
		if (menu->name == name) {
			return menu;
		}
	}
	return nullptr;
}

void MenuManager::frameStart() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	for (auto menu : menus) {
		if (menu->visible) {
			menu->showFunction();
		}
	}
}

void MenuManager::frameEnd() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuManager::shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}