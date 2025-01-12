#pragma once

#include "Script.h"
#include <iostream>


//template<typename T>
//concept Object = requires(T a) {
//	{ a->position } -> std::same_as<glm::vec3>;
//};


class EngineScript : public Script {
public:


	Renderer* renderer;
	ObjectManager* objectManager;
	MeshManager* meshManager;
	InputManager* inputManager;
	MenuManager* menuManager;
	GLFWwindow* window;


	bool inputDisabled = false;
	bool cursorVisible = true;
	bool verticesAreRelative = true;

	void disableInput() {
		inputDisabled = true;
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
	}

	void enableInput() {
		inputDisabled = false;
		Mouse* mouse = inputManager->getMouse();
		mouse->disabled = false;
		mouse->returnFrame = true;
		for (auto& key : *(inputManager->getKeys())) {
			key->disabled = false;
		}
	}

	void showCursor() {
		inputManager->getMouse()->setVisibility(true);
		cursorVisible = true;
	}
	void hideCursor() {
		inputManager->getMouse()->setVisibility(false);
		cursorVisible = false;
	}

	void onStart() override {
		renderer = Manager::getInstance().getRenderer();
		objectManager = Manager::getInstance().getObjectManager();
		meshManager = Manager::getInstance().getMeshManager();
		inputManager = Manager::getInstance().getInputManager();
		menuManager = Manager::getInstance().getMenuManager();
		window = Manager::getInstance().getWindow();

		Key* toggleInput = new Key(GLFW_KEY_TAB);
		toggleInput->pressFunction = [this]() {
			if (this->inputDisabled) {
				this->enableInput();
				inputManager->getMouse()->setVisibility(false);
				cursorVisible = false;
				menuManager->getMenuByName("Debug")->hide();
				menuManager->getMenuByName("Demo")->hide();
			}
			else {
				this->disableInput();
				inputManager->getMouse()->setVisibility(true);
				cursorVisible = true;
				menuManager->getMenuByName("Debug")->show();
				menuManager->getMenuByName("Demo")->show();
			}
		};
		inputManager->addKey(toggleInput);

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

			if (ImGui::Checkbox("Input Disabled", &inputDisabled)) { // True and run when value inputDisabled is CHANGED
				if (inputDisabled) {
					disableInput();
				}
				else {
					enableInput();
				}
			}

			if (ImGui::Checkbox("Show Cursor", &cursorVisible)) {
				if (cursorVisible) {
					showCursor();
				}
				else {
					hideCursor();
				}
			}

			ImGui::Checkbox("Show Demo", &menuManager->getMenuByName("Demo")->visible);

			ImGui::Checkbox("Vertices are relative", &verticesAreRelative);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("If off, vertices are added to their position to see their place in world space.");

			ImGui::Text("FPS: %.1f", framerate); 
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("Calculated by taking a counter of frames until one second has passed.");

			if (ImGui::CollapsingHeader("See Objects")) {
				ImGui::Text("Number of Objects: %zu", objects.size());
				if (ImGui::TreeNode("Meshes")) {
					std::vector<Mesh*>* meshes_ptr = meshManager->getMeshes();
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Instances")) {
					std::vector<InstanceGroup*>* instanceGroups_ptr = meshManager->getInstanceGroups();
					for (InstanceGroup* instanceGroup : *instanceGroups_ptr) {
						std::string name = instanceGroup->name + "##xx";

						if (ImGui::TreeNode((name).c_str())) {
							instanceGroupTree(instanceGroup, verticesAreRelative);
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

	template <typename T>
	void PositionInputV3(T* object) {
		ImGui::PushItemWidth(100); // Input fields have a max width 
		glm::vec3 tempPosition = object->position; // Copy of position
		float smallStep = 1.0f;
		float largeStep = 10.0f;
		// This way of using an if-statement with all three inputs all in one makes it impossible to put it on a single line. Check solution for vertex inputs below.
		if (ImGui::InputFloat("X##xx", &tempPosition.x, smallStep, largeStep, "%.1f") || ImGui::InputFloat("Y##xx", &tempPosition.y, smallStep, largeStep, "%.1f") || ImGui::InputFloat("Z##xx", &tempPosition.z, smallStep, largeStep, "%.1f")) {
			glm::vec3 change = tempPosition - object->position;
			object->move(change);

		}
		ImGui::PopItemWidth();

	}
	
	template <typename T>
	void RotationDragV3(T* object) {
		ImGui::PushItemWidth(100); // Input fields have a max width 
		glm::vec3 tempRotation = glm::degrees(glm::eulerAngles(object->rotation)); // Copy of rotation
		if (ImGui::DragFloat("Pitch##xx", &tempRotation.x, 0.1f, 0.0f, 360.0f, "%.1f") || ImGui::DragFloat("Yaw##xx", &tempRotation.y, 0.1f, 0.0f, 360.0f, "%.1f") || ImGui::DragFloat("Roll##xx", &tempRotation.z, 0.1f, 0.0f, 360.0f, "%.1f")) {
			glm::vec3 change = tempRotation - glm::degrees(glm::eulerAngles(object->rotation));
			object->rotateEuler(change);

		}
		ImGui::PopItemWidth();
	}

	template <typename T>
	void ScaleDragV3(T* object) {
		ImGui::PushItemWidth(100); // Input fields have a max width 
		glm::vec3 tempScale = object->scale; // Copy of scale
		if (ImGui::DragFloat("X##xx", &tempScale.x, 0.1f, 0.0f, 100.0f, "%.1f") || ImGui::DragFloat("Y##xx", &tempScale.y, 0.1f, 0.0f, 100.0f, "%.1f") || ImGui::DragFloat("Z##xx", &tempScale.z, 0.1f, 0.0f, 100.0f, "%.1f")) {
			glm::vec3 change = tempScale - object->scale;
			object->addScale(change);
		}
		ImGui::PopItemWidth();
	}

	template <typename T>
	void VertexInputV3(unsigned int index, Vertex& vertex, T* object, bool relative) {
		ImGui::PushItemWidth(50); // Input fields have a max width 
		float speed = 0.01f; // Speed of input change
		float min = 0.0f; // No minimum
		float max = 0.0f; // No maximum
		bool changed = false; // Flag tracks change of inputs

		Vertex tempVertex = vertex; // Copy of pre-change vertex
		if(relative){
			tempVertex.position += object->position;
		}
		ImGui::PushID(index);

		// Input for x
		changed |= ImGui::DragFloat("x", &tempVertex.position.x, speed, min, max, "%.2f");
		ImGui::SameLine();

		// Input for y
		changed |= ImGui::DragFloat("y", &tempVertex.position.y, speed, min, max, "%.2f");
		ImGui::SameLine();

		// Input for z
		changed |= ImGui::DragFloat("z", &tempVertex.position.z, speed, min, max, "%.2f");

		if (changed && relative) {
			changed = false; // Reset flag
			vertex.position = tempVertex.position - object->position;
			// some update 
		}
		else if (changed) {
			changed = false; // Reset flag
			vertex.position = tempVertex.position;
			// some update 
		}
		ImGui::PopID();
		ImGui::PopItemWidth();
	}



	void meshTree(std::vector<Mesh*> meshes, bool verticesAreRelative) {
		for (int i = 0; i < meshes.size(); i++) {
			Mesh* mesh = meshes[i];
			std::string name = mesh->name;
			std::string id = std::to_string(mesh->id);
			std::string nameWithID = id + name;

			if (ImGui::TreeNode(nameWithID.c_str())) {
				ImGui::Text("Name: %s", name.c_str());
				ImGui::Text("ID: %s", id.c_str());
				if (ImGui::CollapsingHeader("Position")) {
					PositionInputV3(mesh);
				}
				if (ImGui::CollapsingHeader("Rotation")) {
					RotationDragV3(mesh);
				}
				if (ImGui::CollapsingHeader("Scale")) {
					ScaleDragV3(mesh);
				}

				std::vector<Vertex>& vertices = mesh->vertices;
				if (vertices.size() > 0) {
					if (ImGui::CollapsingHeader("Vertices")) {
						glm::vec3 objectPosition = mesh->position;
						// Display vertices
						for (int i = 0; i < vertices.size(); i++) {
							Vertex& vertex = vertices[i]; // Reference to existing vertex position, will be updated in code
							ImGui::Text("Vertex %d:", i);
							ImGui::SameLine();
							VertexInputV3(i, vertex, mesh, verticesAreRelative);
						}
					}
				}

				if (mesh->children.size() > 0) { // Recursive for each child
					if (ImGui::CollapsingHeader("Children")) {
						meshTree(mesh->children, verticesAreRelative);
					}
				}
				if (mesh->attachedCamera != nullptr) {
					if (ImGui::CollapsingHeader("Attached Camera")) {
						Camera* camera = mesh->attachedCamera;
						glm::vec3& cameraPosition = camera->getPosition();
						glm::vec3& cameraRotation = camera->getDirection();

						ImGui::Text("Position: (%f, %f, %f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
						ImGui::Text("Rotation: (%f, %f, %f)", cameraRotation.x, cameraRotation.y, cameraRotation.z);
					}
				}
			}
		}
	}

	void instanceGroupTree(InstanceGroup* instanceGroup, bool verticesAreRelative) {
		std::vector<Instance*>* instances = &instanceGroup->instances;
		if (ImGui::CollapsingHeader("Position")) {

			PositionInputV3(instanceGroup);
		}
		if (ImGui::CollapsingHeader("Rotation")) {
			RotationDragV3(instanceGroup);
		}
		if (ImGui::CollapsingHeader("Scale")) {
			ScaleDragV3(instanceGroup);
		}
		if (instanceGroup->vertices.size() > 0) {
			if (ImGui::CollapsingHeader("Vertices")) {
				glm::vec3 objectPosition = instanceGroup->position;
				// Display vertices
				for (int i = 0; i < instanceGroup->vertices.size(); i++) {
					Vertex& vertex = instanceGroup->vertices[i]; // Reference to existing vertex position, will be updated in code
					ImGui::Text("Vertex %d:", i);
					ImGui::SameLine();
					VertexInputV3(i, vertex, instanceGroup, verticesAreRelative);
				}
			}
		}
		if (instances->size() > 0) {
			if (ImGui::CollapsingHeader("Instances")) {
				for (int i = 0; i < instances->size(); i++) {
					Instance* instance = instances->at(i);
					std::string name = instance->name;
					std::string id = std::to_string(instance->instanceID);
					std::string nameWithID = id + name;

					if (ImGui::TreeNode(nameWithID.c_str())) {
						ImGui::Text("Name: %s", name.c_str());
						ImGui::Text("ID: %s", id.c_str());
						if (ImGui::CollapsingHeader("Position")) {
							PositionInputV3(instance);
						}
						if (ImGui::CollapsingHeader("Rotation")) {
							RotationDragV3(instance);
						}
						if (ImGui::CollapsingHeader("Scale")) {
							ScaleDragV3(instance);
						}
						ImGui::TreePop();
					}
				}
			}
		}
		

	}

	void onUpdate(double deltaTime) override {
		accumulatedFrames += 1;
		accumulatedTime += deltaTime;
		if (accumulatedTime >= 1.0f) {
			framerate = accumulatedFrames / accumulatedTime;
			accumulatedFrames = 0;
			accumulatedTime = 0.0f;
			std::cout << framerate << std::endl;
		}

	}

private:
	double framerate = 0.0f;
	int accumulatedFrames = 0;
	double accumulatedTime = 0.0f;
};
