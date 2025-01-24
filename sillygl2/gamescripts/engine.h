#pragma once

#include "Script.h"
#include <iostream>


template<typename T>
concept GameObjectType = std::is_base_of<GameObject, T>::value || std::is_same<Camera, T>::value;

template<typename T>
concept VertexObjectType = std::is_base_of<VertexObject, T>::value;


class EngineScript : public Script {
public:

	EngineScript() : Script("EngineScript") {}

	Renderer* renderer;
	ObjectManager* objectManager;
	InputManager* inputManager;
	MenuManager* menuManager;
	GLFWwindow* window;


	bool inputDisabled = false;
	bool cursorVisible = true;
	bool relativePositioning = true;
	bool rotationsAreQuat = false;

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

		// Setup Existing Demo Window as a window
		Menu* demoMenu = new Menu([]() {
			ImGui::ShowDemoWindow();
			}, "Demo");
		menuManager->addMenu(demoMenu);


		// Debug menu creation
		Menu* debugMenu = new Menu([this, debugMenu]() {
			ImGui::Begin("Debug Menu");
			ImGui::SetWindowSize(ImVec2(300, 300), ImGuiWindowFlags_AlwaysAutoResize);

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

			ImGui::Checkbox("Relative Positioning", &relativePositioning);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("Whether objects in containers should have their position relative to the container. If off, will have position in world space.");

			ImGui::Checkbox("Rotations are shown as quaternions", &rotationsAreQuat);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("If off, rotations are shown as Euler angles.");

			ImGui::Text("FPS: %.1f", framerate); 
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("Calculated by taking a counter of frames until one second has passed.");

			if (ImGui::CollapsingHeader("See Objects")) {
				if (ImGui::TreeNode("Instances")) {
					std::vector<InstanceGroup*>* instanceGroups_ptr = objectManager->getInstanceGroups();
					for (InstanceGroup* instanceGroup : *instanceGroups_ptr) {
						std::string name = instanceGroup->getName() + "##xx";

						if (ImGui::TreeNode((name).c_str())) {
							instanceGroupTree(instanceGroup, relativePositioning, rotationsAreQuat);
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

	template <GameObjectType T>
	void PositionInputV3(T* object, const glm::vec3& absoluteOffset = glm::vec3(0.0f)) {
		ImGui::PushID("##xx");
		ImGui::PushItemWidth(100); // Input fields have a max width 
		glm::vec3 tempPosition = object->getPosition() + absoluteOffset; // Copy of position
		float smallStep = 1.0f;
		float largeStep = 10.0f;
		// This way of using an if-statement with all three inputs all in one makes it impossible to put it on a single line. Check solution for vertex inputs below.
		if (ImGui::InputFloat("X##xx", &tempPosition.x, smallStep, largeStep, "%.1f") || ImGui::InputFloat("Y##xx", &tempPosition.y, smallStep, largeStep, "%.1f") || ImGui::InputFloat("Z##xx", &tempPosition.z, smallStep, largeStep, "%.1f")) {
			glm::vec3 change = tempPosition - (object->getPosition() + absoluteOffset);
			object->move(change);

		}
		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	template <GameObjectType T>
	void RotationDragV3(T* object, const glm::vec3& offset = glm::vec3(0.0f)) {
		ImGui::PushID("##xx");
		ImGui::PushItemWidth(100);

		glm::vec3 eulerRotation = object->getEulerRotation() + offset; // Get Euler angles (degrees)
		glm::vec3 prevRotation = eulerRotation; // Store previous rotation for detecting changes

		bool modified = false;
		modified |= ImGui::DragFloat("Pitch##xx", &eulerRotation.x, 0.1f, -180.0f, 180.0f, "%.1f");
		modified |= ImGui::DragFloat("Yaw##xx", &eulerRotation.y, 0.1f, -180.0f, 180.0f, "%.1f");
		modified |= ImGui::DragFloat("Roll##xx", &eulerRotation.z, 0.1f, -180.0f, 180.0f, "%.1f");

		if (modified) {
			// Calculate delta rotation in Euler angles
			glm::vec3 deltaRotation = glm::radians(eulerRotation - prevRotation);

			// Convert to quaternion and apply incremental rotation
			glm::quat deltaQuat = glm::quat(deltaRotation);
			object->setRotation(deltaQuat * object->getQuatRotation());
		}


		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	template <GameObjectType T>
	void QuatRotationDragV3(T* object, const glm::quat& offset = glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {
		ImGui::PushID("##xx");
		ImGui::PushItemWidth(100);

		glm::quat previousRotation = object->getQuatRotation() * offset;
		bool modified = false;
		modified |= ImGui::DragFloat("X##xx", &previousRotation.x, 0.1f, -1.0f, 1.0f, "%.1f");
		modified |= ImGui::DragFloat("Y##xx", &previousRotation.y, 0.1f, -1.0f, 1.0f, "%.1f");
		modified |= ImGui::DragFloat("Z##xx", &previousRotation.z, 0.1f, -1.0f, 1.0f, "%.1f");
		modified |= ImGui::DragFloat("W##xx", &previousRotation.w, 0.1f, -1.0f, 1.0f, "%.1f");

		if (modified) {
			object->setRotation(glm::inverse(offset) * previousRotation);
		}

		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	template <GameObjectType T>
	void ScaleDragV3(T* object, const glm::vec3& offset = glm::vec3(1.0f)) {
		ImGui::PushID("##xx");
		ImGui::PushItemWidth(100); // Input fields have a max width 
		glm::vec3 tempScale = object->getScale() * offset; // Copy of scale
		if (ImGui::DragFloat("X", &tempScale.x, 0.1f, 0.0f, 100.0f, "%.1f") || ImGui::DragFloat("Y", &tempScale.y, 0.1f, 0.0f, 100.0f, "%.1f") || ImGui::DragFloat("Z", &tempScale.z, 0.1f, 0.0f, 100.0f, "%.1f")) {
			glm::vec3 change = (tempScale / offset) - object->getScale();
			object->addScale(change);
		}
		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	template <VertexObjectType T>
	void VertexInputV3(unsigned int index, const Vertex& vertex, T* object, bool relative) {
		ImGui::PushItemWidth(50); // Input fields have a max width 
		float speed = 0.01f; // Speed of input change
		float min = 0.0f; // No minimum
		float max = 0.0f; // No maximum

		Vertex tempVertex = vertex; // Copy of pre-change vertex
		if(relative){
			tempVertex.position += object->getPosition();
		}
		ImGui::PushID(index);

		if (ImGui::TreeNode("Position")) {
			bool posChanged = false; // Flag tracks change of position inputs
			// Input for x
			posChanged |= ImGui::DragFloat("x", &tempVertex.position.x, speed, min, max, "%.2f");
			ImGui::SameLine();

			// Input for y
			posChanged |= ImGui::DragFloat("y", &tempVertex.position.y, speed, min, max, "%.2f");
			ImGui::SameLine();

			// Input for z
			posChanged |= ImGui::DragFloat("z", &tempVertex.position.z, speed, min, max, "%.2f");

			if (posChanged && relative) {
				posChanged = false; // Reset flag
				object->setVertex(Vertex(tempVertex.position - object->getPosition(), tempVertex.normal, tempVertex.texCoords), index);
				// some update 
			}
			else if (posChanged) {
				posChanged = false; // Reset flag
				object->setVertex(tempVertex, index);
				// some update 
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Normal")) {
			bool normalChanged = false; // Flag tracks change of normal inputs
			// Input for x
			normalChanged |= ImGui::DragFloat("x", &tempVertex.normal.x, speed, min, max, "%.2f");
			ImGui::SameLine();

			// Input for y
			normalChanged |= ImGui::DragFloat("y", &tempVertex.normal.y, speed, min, max, "%.2f");
			ImGui::SameLine();

			// Input for z
			normalChanged |= ImGui::DragFloat("z", &tempVertex.normal.z, speed, min, max, "%.2f");

			if (normalChanged) {
				normalChanged = false; // Reset flag
				object->setVertex(Vertex(tempVertex.position, tempVertex.normal, tempVertex.texCoords), index);
				// some update 
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("TexCoords")) {
			bool texChanged = false; // Flag tracks change of texCoords inputs
			// Input for x
			texChanged |= ImGui::DragFloat("x", &tempVertex.texCoords.x, speed, min, max, "%.2f");
			ImGui::SameLine();

			// Input for y
			texChanged |= ImGui::DragFloat("y", &tempVertex.texCoords.y, speed, min, max, "%.2f");

			if (texChanged) {
				texChanged = false; // Reset flag
				object->setVertex(Vertex(tempVertex.position, tempVertex.normal, tempVertex.texCoords), index);
				// some update 
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
		ImGui::PopItemWidth();
	}

	void instanceGroupTree(InstanceGroup* instanceGroup, bool relativePositioning, bool rotationsAreQuat) {
		std::vector<Instance*> instances = instanceGroup->getInstances();

		Shader* shader = instanceGroup->getShader();
		// Instance Group characteristics
		if (ImGui::CollapsingHeader("Position")) {

			PositionInputV3(instanceGroup);
		}
		if (ImGui::CollapsingHeader("Rotation")) {
			if (rotationsAreQuat) {
				QuatRotationDragV3(instanceGroup);
			}
			else {
				RotationDragV3(instanceGroup);
			}
		}
		if (ImGui::CollapsingHeader("Scale")) {
			ScaleDragV3(instanceGroup);
		}
		if (ImGui::CollapsingHeader("Appearance")) {

			if (ImGui::TreeNode("Textures")) {
				std::vector<Texture*> textures = instanceGroup->getTextures();
				for (int i = 0; i < textures.size(); i++) {
					Texture* texture = textures[i];
					std::string textureName = "Texture " + std::to_string(i);
					if (ImGui::TreeNode(textureName.c_str())) {
						ImGui::Text("Texture ID: %d", texture->id);
						ImGui::Text("Texture Path: %s", texture->path.c_str());
						ImGui::Image(texture->id, ImVec2(texture->width, texture->height));
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Shader")) {
				ImGui::Text("Shader ID: %d", shader->ID);
				ImGui::Text("Vertex Shader Path: %s", shader->vertPath.c_str());
				ImGui::Text("Fragment Shader Path: %s", shader->fragPath.c_str());
				ImGui::TreePop();
			}
		}
		if (instanceGroup->getVertices().size() > 0) {
			if (ImGui::CollapsingHeader("Vertices")) {
				const std::vector<Vertex>& vertices = instanceGroup->getVertices();
				const std::vector<unsigned int>& indices = instanceGroup->getIndices();
				std::string indicesString = "Indices: ";
				for (int i = 0; i < indices.size(); i++) {
					indicesString += std::to_string(indices[i]) + " ";
				}
				ImGui::Text(indicesString.c_str());
				if (ImGui::Button("Add Vertex")) {
					instanceGroup->addVerticesIndices({ Vertex(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)) }, {0});
				}

				glm::vec3 objectPosition = instanceGroup->getPosition();
				// Display vertices
				for (int i = 0; i < vertices.size(); i++) {
					const Vertex& vertex = vertices[i]; // Reference to existing vertex position, will be updated in code
					std::string vertexName = "Vertex " + std::to_string(i);
					if (ImGui::TreeNode(vertexName.c_str())) {
						VertexInputV3(i, vertex, instanceGroup, relativePositioning);
						
						std::string deleteButtonName = "Delete##delbutton" + std::to_string(i);
						if (ImGui::Button(deleteButtonName.c_str())) {
							instanceGroup->removeVertex(i);
						}
						ImGui::TreePop();
					}
				}
			}
		}
		if (instanceGroup->isCameraAttached()) {
			if (ImGui::TreeNode("Camera")) {
				Camera* camera = instanceGroup->getAttachedCamera();
				if (ImGui::CollapsingHeader("Position")) {
					PositionInputV3(camera);
				}
				if (ImGui::CollapsingHeader("Rotation")) {
					if (rotationsAreQuat) {
						QuatRotationDragV3(camera);
					}
					else {
						RotationDragV3(camera);
					}
				}
				ImGui::TreePop();
			}

		}
		if (instances.size() > 0) {
			if (ImGui::CollapsingHeader("Instances")) {
				glm::vec3 posOffset = glm::vec3(0.0f);
				glm::vec3 rotEOffset = glm::vec3(0.0f);
				glm::quat rotQOffset = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 scaleOffset = glm::vec3(0.0f);
				if (!relativePositioning) {
					posOffset = instanceGroup->getPosition();
					rotEOffset = instanceGroup->getEulerRotation();
					rotQOffset = instanceGroup->getQuatRotation();
					scaleOffset = instanceGroup->getScale();
				}
				for (int i = 0; i < instances.size(); i++) {
					Instance* instance = instances.at(i);
					std::string name = instance->getName();
					std::string id = std::to_string(instance->instanceID);
					std::string nameWithID = id + name;

					if (ImGui::TreeNode(nameWithID.c_str())) {
						ImGui::Text("Name: %s", name.c_str());
						ImGui::Text("ID: %s", id.c_str());
						if (ImGui::Button("Delete")) {
							instanceGroup->removeInstance(instance);
						}
						if (ImGui::CollapsingHeader("Position")) {
							PositionInputV3(instance, posOffset);
						}
						if (ImGui::CollapsingHeader("Rotation")) {
							if (rotationsAreQuat) {
								QuatRotationDragV3(instance, rotQOffset);
							}
							else {
								RotationDragV3(instance, rotEOffset);
							}
						}
						if (ImGui::CollapsingHeader("Scale")) {
							ScaleDragV3(instance, scaleOffset);
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
