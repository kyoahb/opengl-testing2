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
				ImGui::SetTooltip("Vertices are subtracted by object position.");

			ImGui::Text("FPS: %.1f", framerate); 
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("Calculated by taking a counter of frames until one second has passed.");

			if (ImGui::CollapsingHeader("See Objects")) {
				ImGui::Text("Number of Objects: %zu", objects.size());
				if (ImGui::TreeNode("Objects")) {
					objectTree(objects, verticesAreRelative);
					ImGui::TreePop();
				}

			}

			ImGui::End();
			}, "Debug");
		menuManager->addMenu(debugMenu);
		}

	void PositionInputV3(GameObject* object) {
		ImGui::PushItemWidth(100); // Input fields have a max width 
		glm::vec3 tempPosition = object->getPosition(); // Copy of position
		float smallStep = 1.0f;
		float largeStep = 10.0f;
		// This way of using an if-statement with all three inputs all in one makes it impossible to put it on a single line. Check solution for vertex inputs below.
		if (ImGui::InputFloat("X##xx", &tempPosition.x, smallStep, largeStep, "%.1f") || ImGui::InputFloat("Y##xx", &tempPosition.y, smallStep, largeStep, "%.1f") || ImGui::InputFloat("Z##xx", &tempPosition.z, smallStep, largeStep, "%.1f")) {
			glm::vec3 change = tempPosition - object->getPosition();
			object->move(change);

		}
		ImGui::PopItemWidth();

	}

	void RotationDragV3(GameObject* object) {
		ImGui::PushItemWidth(100); // Input fields have a max width 
		glm::vec3 tempRotation = object->getRotation(); // Copy of rotation
		if (ImGui::DragFloat("Pitch##xx", &tempRotation.x, 0.1f, 0.0f, 360.0f, "%.1f") || ImGui::DragFloat("Yaw##xx", &tempRotation.y, 0.1f, 0.0f, 360.0f, "%.1f") || ImGui::DragFloat("Roll##xx", &tempRotation.z, 0.1f, 0.0f, 360.0f, "%.1f")) {
			glm::vec3 change = tempRotation - object->getRotation();
			object->rotate(change);

		}
		ImGui::PopItemWidth();
	}

	void RelativeVertexInputV3(unsigned int index, glm::vec3& vertex, glm::vec3& relativeVertex, GameObject* object) {
		ImGui::PushItemWidth(50); // Input fields have a max width 
		float speed = 0.01f; // Speed of input change
		float min = 0.0f; // No minimum
		float max = 0.0f; // No maximum
		glm::vec3 tempRelativeVertex = relativeVertex; // Copy of pre-change RELATIVE vertex
		bool changed = false; // Flag tracks change of inputs
		ImGui::PushID(index);

		// Input for x
		changed |= ImGui::DragFloat("x", &tempRelativeVertex.x, speed, min, max, "%.2f");
		ImGui::SameLine();

		// Input for y
		changed |= ImGui::DragFloat("y", &tempRelativeVertex.y, speed, min, max, "%.2f");
		ImGui::SameLine();

		// Input for z
		changed |= ImGui::DragFloat("z", &tempRelativeVertex.z, speed, min, max, "%.2f");

		if (changed) {
			changed = false; // Reset flag
			glm::vec3 change = tempRelativeVertex - relativeVertex;
			relativeVertex = tempRelativeVertex; 
			vertex += change;
			object->setObjectEvent(Object_Transformation); // Ensure object is updated immediately
		}
		ImGui::PopID();
		ImGui::PopItemWidth();
	}

	void VertexInputV3(unsigned int index, glm::vec3& vertex, GameObject* object) {
		ImGui::PushItemWidth(50); // Input fields have a max width 
		float speed = 0.01f;
		float min = 0.0f; // No minimum 
		float max = 0.0f; // No maximum
		glm::vec3 tempVertex = vertex; // Copy of pre-change vertex
		bool changed = false; // Flag tracks change of inputs

		ImGui::PushID(index); // Necessary to allow multiple widgets with same name

		// Input for x
		changed |= ImGui::DragFloat("x", &tempVertex.x, speed, min, max, "%.2f");
		ImGui::SameLine(); 

		// Input for y
		changed |= ImGui::DragFloat("y", &tempVertex.y, speed, min, max, "%.2f");
		ImGui::SameLine(); 

		// Input for z
		changed |= ImGui::DragFloat("z", &tempVertex.z, speed, min, max, "%.2f");

		// Check if any value changed
		if (changed) {
			changed = false;
			glm::vec3 change = tempVertex - vertex; // Detect difference, and apply it to the vertex
			vertex = tempVertex; // Update vertex with tempVertex
			object->setObjectEvent(Object_Transformation); // Ensure object is updated immediately
		}

		ImGui::PopID();
		ImGui::PopItemWidth();
	}

	void objectTree(std::vector<GameObject*> objects, bool verticesAreRelative) {
		for (int i = 0; i < objects.size(); i++) {
			GameObject* object = objects[i];
			std::string name = object->name;
			std::string id = std::to_string(object->id);
			std::string nameWithID = id + name;

			if (ImGui::TreeNode(nameWithID.c_str())) {
				ImGui::Checkbox("Visible##xx", &object->visible);
				ImGui::Text("Name: %s", name.c_str());
				ImGui::Text("ID: %s", id.c_str());
				if (ImGui::CollapsingHeader("Position")) {
					PositionInputV3(object);
				}
				if (ImGui::CollapsingHeader("Rotation")) {
					RotationDragV3(object);
				}

				std::vector<glm::vec3>& vertices = object->getVertices();
				if (vertices.size() > 0) {
					if (ImGui::CollapsingHeader("Vertices")) {
						if (ImGui::Button("Add Vertex")) {
							std::vector<unsigned int> newIndices = { (unsigned int)vertices.size() };
							std::vector<glm::vec3> newVerts = { glm::vec3(0.0f, 0.0f, 0.0f) };
							object->addVerticesIndices(newVerts, newIndices, true);
							object->setObjectEvent(Object_Vertices_Changed);
						}
						glm::vec3 objectPosition = object->getPosition();
						// Display vertices
						for (int i = 0; i < vertices.size(); i++) {
							glm::vec3& vertex = vertices[i]; // Reference to existing vertex position, will be updated in code
							if (verticesAreRelative) {
								glm::vec3 relativeVertex = vertex - objectPosition;
								ImGui::Text("Relative vertex %d:", i);
								ImGui::SameLine();
								RelativeVertexInputV3(i, vertex, relativeVertex, object);
							}
							else {
								ImGui::Text("Vertex %d:", i);
								ImGui::SameLine();
								VertexInputV3(i, vertex, object);
							}
						}
					}
				}

				if (object->getChildren().size() > 0) { // Recursive for each child
					if (ImGui::CollapsingHeader("Children")) {
						objectTree(object->getChildren(), verticesAreRelative);
					}
				}
				if (object->isCameraAttached) {
					if (ImGui::CollapsingHeader("Attached Camera")) {
						Camera* camera = object->getAttachedCamera();
						glm::vec3& cameraPosition = camera->getPosition();
						glm::vec3& cameraRotation = camera->getDirection();
						
						ImGui::Text("Position: (%f, %f, %f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
						ImGui::Text("Rotation: (%f, %f, %f)", cameraRotation.x, cameraRotation.y, cameraRotation.z);
					}
				}
				if (ImGui::Button("Delete")) {
					objectManager->destroyObject(object);
				}
				ImGui::TreePop();
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
