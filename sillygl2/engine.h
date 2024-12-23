#pragma once

#include "Script.h"
#include <iostream>

class EngineScript : public Script {
public:
	bool inputDisabled = false;
	void onStart() override {
		Renderer* renderer = Manager::getInstance().getRenderer();
		ObjectManager* objectManager = Manager::getInstance().getObjectManager();
		InputManager* inputManager = Manager::getInstance().getInputManager();
		MenuManager* menuManager = Manager::getInstance().getMenuManager();
		GLFWwindow* window = Manager::getInstance().getWindow();

		Key* disableInput = new Key(GLFW_KEY_TAB);
		disableInput->pressFunction = [this, inputManager, menuManager]() {
			this->inputDisabled = !this->inputDisabled;
			if (this->inputDisabled) {
				menuManager->getMenuByName("Debug")->show();
				menuManager->getMenuByName("Demo")->show();
				inputManager->getMouse()->disabled = true;
				for (auto& key : *(inputManager->getKeys())) {
					key->isHeld = false;

					if (key->keyCode == GLFW_KEY_TAB) { // Don't disable the tab key
						continue;
					}
					else {
						key->disabled = true;
					}
				}
				inputManager->getMouse()->setVisibility(true);
			}
			else {
				menuManager->getMenuByName("Debug")->hide();
				menuManager->getMenuByName("Demo")->hide();
				inputManager->getMouse()->disabled = false;
				for (auto& key : *(inputManager->getKeys())) {
					key->disabled = false;
				}
				inputManager->getMouse()->setVisibility(false);
			}
			};
		inputManager->addKey(disableInput);

		// Setup Existing Demo Window as a window
		Menu* demoMenu = new Menu([]() {
			ImGui::ShowDemoWindow();
			}, "Demo");
		menuManager->addMenu(demoMenu);


		// Debug menu creation
		Menu* debugMenu = new Menu([debugMenu, objectManager]() {
			ImGui::Begin("Debug Menu");
			ImGui::SetWindowSize(ImVec2(300, 300));

			std::vector<GameObject*>* objects_ptr = objectManager->getObjects();
			std::vector<GameObject*> rendered_objects = {};
			ImGui::Text("This is a debug menu");
			if (ImGui::CollapsingHeader("Objects")) {
				std::vector<GameObject*>* objects_ptr = objectManager->getObjects();
				ImGui::Text("This is a debug menu");

			}

			ImGui::End();
			}, "Debug");
		menuManager->addMenu(debugMenu);



	}

	void onUpdate(double deltaTime) override {
	}
private:

};
