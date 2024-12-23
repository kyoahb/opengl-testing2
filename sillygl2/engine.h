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
			std::vector<GameObject*> objects = *(objects_ptr);
			ImGui::Text("This is a debug menu");
			if (ImGui::CollapsingHeader("See Objects")) {
                ImGui::Text("Number of Objects: %zu", objects.size());
				if(ImGui::TreeNode("Objects")) {
					for (int i = 0; i < objects.size(); i++) {
						std::string name = objects[i]->name;
						std::string id = std::to_string(objects[i]->id);
						std::string nameWithID = id + name;
						if (ImGui::TreeNode(nameWithID.c_str())) {
							ImGui::Text("Position: (%f, %f, %f)", objects[i]->position.x, objects[i]->position.y, objects[i]->position.z);
							ImGui::Text("Rotation: (%f, %f, %f)", objects[i]->rotation.x, objects[i]->rotation.y, objects[i]->rotation.z);
							ImGui::Text("Visible: %s", objects[i]->rendered ? "true" : "false");
							ImGui::Text("Name: %s", name.c_str());
							ImGui::Text("ID: %s", id.c_str());
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}

			}

			ImGui::End();
			}, "Debug");
		menuManager->addMenu(debugMenu);



	}

	void onUpdate(double deltaTime) override {
	}
private:

};
