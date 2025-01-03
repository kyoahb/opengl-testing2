#pragma once

#include "Script.h"
#include <iostream>

class EngineScript : public Script {
public:
	Renderer* renderer;
	ObjectManager* objectManager;
	InputManager* inputManager;
	MenuManager* menuManager;
	GLFWwindow* window;


	bool inputDisabled = false;
	void onStart() override {
		renderer = Manager::getInstance().getRenderer();
		objectManager = Manager::getInstance().getObjectManager();
		inputManager = Manager::getInstance().getInputManager();
		menuManager = Manager::getInstance().getMenuManager();
		window = Manager::getInstance().getWindow();

		Key* disableInput = new Key(GLFW_KEY_TAB);
		disableInput->pressFunction = [this]() {
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
				Mouse* mouse = inputManager->getMouse();
				mouse->disabled = false;
				mouse->returnFrame = true;

				
				for (auto& key : *(inputManager->getKeys())) {
					key->disabled = false;
				}
				inputManager->getMouse()->setVisibility(false);
			}
			};
		inputManager->addKey(disableInput);

		Key* testKey = new Key(GLFW_KEY_T);
		testKey->pressFunction = [this]() {
			GameObject* parent = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), "parent", false);
			GameObject* child = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), "child", false);
			parent->attachChild(child);
			objectManager->addObject(parent);
			objectManager->addObject(child);
			};
		inputManager->addKey(testKey);



		// Setup Existing Demo Window as a window
		Menu* demoMenu = new Menu([]() {
			ImGui::ShowDemoWindow();
			}, "Demo");
		menuManager->addMenu(demoMenu);


		// Debug menu creation
		Menu* debugMenu = new Menu([this, debugMenu]() {
			ImGui::Begin("Debug Menu");
			ImGui::SetWindowSize(ImVec2(300, 300), ImGuiWindowFlags_AlwaysAutoResize);
			std::vector<GameObject*>* objects_ptr = objectManager->getObjects();
			std::vector<GameObject*> objects = *(objects_ptr);
			ImGui::Text("This is a debug menu");
			if (ImGui::CollapsingHeader("See Objects")) {
                ImGui::Text("Number of Objects: %zu", objects.size());
				if(ImGui::TreeNode("Objects")) {
					objectTree(objects);
					ImGui::TreePop();
				}

			}

			ImGui::End();
			}, "Debug");
		menuManager->addMenu(debugMenu);



	}

	void ImGuiVector3(const char* label, glm::vec3& vec) {
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::PushItemWidth(200); // Input fields have a max width 
		ImGui::InputFloat("X##x", &vec.x, 0.1f, 1.0f, "%.2f");
		ImGui::SameLine();
		ImGui::InputFloat("Y##x", &vec.y, 0.1f, 1.0f, "%.2f");
		ImGui::SameLine();
		ImGui::InputFloat("Z##x", &vec.z, 0.1f, 1.0f, "%.2f");
		ImGui::PopItemWidth();
	}

	void objectTree(std::vector<GameObject*> objects) {
		for (int i = 0; i < objects.size(); i++) {
			GameObject* object = objects[i];
			std::string name = object->name;
			std::string id = std::to_string(object->id);
			std::string nameWithID = id + name;

			if (ImGui::TreeNode(nameWithID.c_str())) {
				glm::vec3& position = object->position;
				glm::vec3& rotation = object->rotation;
				ImGuiVector3("Position", position);
				ImGui::Text("Position: (%f, %f, %f)", position.x, position.y, position.z);
				ImGui::Text("Rotation: (%f, %f, %f)", rotation.x, rotation.y, rotation.z);
				ImGui::Text("Visible: %s", object->rendered ? "true" : "false");
				ImGui::Text("Name: %s", name.c_str());
				ImGui::Text("ID: %s", id.c_str());

				if (object->children.size() > 0) { // Recursive for each child
					if (ImGui::TreeNode("Children")) {
						objectTree(object->children);
						ImGui::TreePop();
					}
				}
				if (object->attachedCamera) {
					if (ImGui::TreeNode("Attached Camera")) {
						Camera* camera = object->attachedCamera;
						glm::vec3 cameraPosition = camera->position;
						glm::vec3 cameraRotation = camera->direction;
						ImGui::Text("Position: (%f, %f, %f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
						ImGui::Text("Rotation: (%f, %f, %f)", cameraRotation.x, cameraRotation.y, cameraRotation.z);
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}

	void onUpdate(double deltaTime) override {
	}

private:

};
