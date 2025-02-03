#pragma once

#include "Script.h"
#include <iostream>

/*
template<typename T>
concept GameObjectType = std::is_base_of<GameObject, T>::value || std::is_same<Camera, T>::value;

template<typename T>
concept VertexObjectType = std::is_base_of<VertexObject, T>::value;
*/

template<typename T>
concept ObjectType = std::is_base_of<Object, T>::value;


template<typename T>
concept InstanceGroupType = std::is_base_of<InstanceGroup, T>::value;

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

			if (ImGui::CollapsingHeader("Object Types List")) {
				if (ImGui::TreeNode("Instance Groups")) {
					for (auto& instanceGroup : *(objectManager->getInstanceGroups())) {
						InstanceGroupTree(instanceGroup);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Objects")) {
					for (auto& object : *(objectManager->getObjects())) {
						ObjectTree(object);
					}
					ImGui::TreePop();
				}
			}
			ImGui::End();
			}, "Debug");
		menuManager->addMenu(debugMenu);
	}

	template <ObjectType T>
	void ChildrenComponent(std::shared_ptr<T> object) {
		if (object->getChildren().size() > 0) {
			if (ImGui::CollapsingHeader("Children")) {
				for (auto& child : object->getChildren()) {
					ObjectTree(child);
				}
			}
		}
	}

	void InstancesComponent(std::shared_ptr<InstanceGroup> object) {
		if (object->getInstances()->size() > 0) {
			if (ImGui::CollapsingHeader("Instances")) {
				for (auto& child : *(object->getInstances())) {
					ObjectTree(child);
				}
			}
		}
	}

	template <ObjectType T>
	void RenderableComponent(std::shared_ptr<T> object) {
		RenderComponent* renderComp = object->getRenderComponent();
		if (renderComp == nullptr) {
			return;
		}


		if (ImGui::CollapsingHeader("Vertices")) {
			const std::vector<Vertex>& vertices = renderComp->getVertices();
			const std::vector<unsigned int>& indices = renderComp->getIndices();
			std::string indicesString = "Indices: ";
			for (int i = 0; i < indices.size(); i++) {
				indicesString += std::to_string(indices[i]) + " ";
			}
			ImGui::Text(indicesString.c_str());
			if (ImGui::Button("Add Vertex")) {
				renderComp->addVerticesIndices({ Vertex(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)) }, { 0 });
			}

			glm::vec3 objectPosition = object->getPosition();
			// Display vertices
			for (int i = 0; i < vertices.size(); i++) {
				const Vertex& vertex = vertices[i]; // Reference to existing vertex position, will be updated in code
				std::string vertexName = "Vertex " + std::to_string(i);
				if (ImGui::TreeNode(vertexName.c_str())) {
					VertexInputV3(i, vertex, object, relativePositioning);
					ImGui::TreePop();
				}
			}
		}

		if (ImGui::CollapsingHeader("Material")) {
			std::string materialName = "Material: " + renderComp->getMaterial()->name;
			ImGui::Text(materialName.c_str());

			if (ImGui::TreeNode("Textures")) {
				Texture* diffuse = renderComp->getMaterial()->diffuse;
				if (diffuse != nullptr) {
					ImGui::Text("Diffuse Texture ID: %d", diffuse->id);
					ImGui::Text("Diffuse Texture Path: %s", diffuse->path.c_str());
					ImGui::Image(diffuse->id, ImVec2(100, 100));
				}

				Texture* specular = renderComp->getMaterial()->specular;
				if (specular != nullptr) {
					ImGui::Text("Specular Texture ID: %d", specular->id);
					ImGui::Text("Specular Texture Path: %s", specular->path.c_str());
					ImGui::Image(specular->id, ImVec2(100, 100));
				}

				Texture* normal = renderComp->getMaterial()->normal;
				if (normal != nullptr) {
					ImGui::Text("Normal Texture ID: %d", normal->id);
					ImGui::Text("Normal Texture Path: %s", normal->path.c_str());
					ImGui::Image(normal->id, ImVec2(100, 100));
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Shader")) {
				ImGui::Text("Shader ID: %d", renderComp->getShader()->ID);
				ImGui::Text("Vertex Shader Path: %s", renderComp->getShader()->vertPath.c_str());
				ImGui::Text("Fragment Shader Path: %s", renderComp->getShader()->fragPath.c_str());
				ImGui::TreePop();
			}
		}
	}

	template <ObjectType T>
	void ObjectTree(std::shared_ptr<T> object) {
		std::string name = object->getName();
		std::string id = std::to_string(object->getId());
		std::string nameID = id + name;
		ImGui::PushID(id.c_str());
		if (ImGui::TreeNode(nameID.c_str())) {
			if (ImGui::CollapsingHeader("Position")) {
				PositionVec3(object); // No offset
			}
			if (ImGui::CollapsingHeader("Rotation")) {
				if (rotationsAreQuat) {
					QuatRotationVec3(object); // No offset
				}
				else {
					EulerRotationVec3(object); // No offset
				}
			}
			if (ImGui::CollapsingHeader("Scale")) {
				ScaleDragV3(object); // No offset
			}

			if (object->hasRenderComponent()) {
				RenderableComponent(object);
			}

			ChildrenComponent(object);
			

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	void InstanceGroupTree(std::shared_ptr<InstanceGroup> object) {
		std::string name = object->getName();
		std::string id = std::to_string(object->getId());
		std::string nameID = id + name;
		ImGui::PushID(id.c_str());
		if (ImGui::TreeNode(nameID.c_str())) {
			if (ImGui::CollapsingHeader("Position")) {
				PositionVec3(object); // No offset
			}
			if (ImGui::CollapsingHeader("Rotation")) {
				if (rotationsAreQuat) {
					QuatRotationVec3(object); // No offset
				}
				else {
					EulerRotationVec3(object); // No offset
				}
			}
			if (ImGui::CollapsingHeader("Scale")) {
				ScaleDragV3(object); // No offset
			}
			if (object->hasRenderComponent()) {
				RenderableComponent(object);
			}

			InstancesComponent(object);

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	template <ObjectType T>
	void PositionVec3(std::shared_ptr<T> object, const glm::vec3& absoluteOffset = glm::vec3(0.0f)) {
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
	}

	template <ObjectType T>
	void EulerRotationVec3(std::shared_ptr<T> object, const glm::vec3& offset = glm::vec3(0.0f)) {
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
			object->setQuatRotation(deltaQuat * object->getQuatRotation());
		}
	}

	template <ObjectType T>
	void QuatRotationVec3(std::shared_ptr<T> object, const glm::quat& offset = glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {
		ImGui::PushItemWidth(100);

		glm::quat previousRotation = object->getQuatRotation() * offset;
		bool modified = false;
		modified |= ImGui::DragFloat("X##xx", &previousRotation.x, 0.1f, -1.0f, 1.0f, "%.1f");
		modified |= ImGui::DragFloat("Y##xx", &previousRotation.y, 0.1f, -1.0f, 1.0f, "%.1f");
		modified |= ImGui::DragFloat("Z##xx", &previousRotation.z, 0.1f, -1.0f, 1.0f, "%.1f");
		modified |= ImGui::DragFloat("W##xx", &previousRotation.w, 0.1f, -1.0f, 1.0f, "%.1f");

		if (modified) {
			object->setQuatRotation(glm::inverse(offset) * previousRotation);
		}

		ImGui::PopItemWidth();
	}

	void CameraRotation(std::shared_ptr<Camera> object) {
		ImGui::PushItemWidth(100);
		glm::vec3 eulerRotation = object->getEulerRotation(); // Get Euler angles (degrees)

		bool modified = false;
		modified |= ImGui::DragFloat("Pitch##xx", &eulerRotation.x, 0.1f, -180.0f, 180.0f, "%.1f");
		modified |= ImGui::DragFloat("Yaw##xx", &eulerRotation.y, 0.1f, -180.0f, 180.0f, "%.1f");
		modified |= ImGui::DragFloat("Roll##xx", &eulerRotation.z, 0.1f, -180.0f, 180.0f, "%.1f");

		if (modified) {
			object->setRotation(eulerRotation);
		}
	}

	template <ObjectType T>
	void ScaleDragV3(std::shared_ptr<T> object, const glm::vec3& offset = glm::vec3(1.0f)) {
		ImGui::PushID("##xx");
		ImGui::PushItemWidth(100); // Input fields have a max width 
		glm::vec3 tempScale = offset * object->getScale(); // Copy of scale
		if (ImGui::DragFloat("X", &tempScale.x, 0.1f, 0.0f, 100.0f, "%.1f") || ImGui::DragFloat("Y", &tempScale.y, 0.1f, 0.0f, 100.0f, "%.1f") || ImGui::DragFloat("Z", &tempScale.z, 0.1f, 0.0f, 100.0f, "%.1f")) {
			glm::vec3 change = (tempScale / offset) - object->getScale();
			object->addScale(change);
		}
		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	template <ObjectType T>
	void VertexInputV3(unsigned int vertexIndex, const Vertex& vertex, std::shared_ptr<T> object, bool relative) {
		RenderComponent* renderComp = object->getRenderComponent();
		
		ImGui::PushItemWidth(50); // Input fields have a max width 
		float speed = 0.01f; // Speed of input change
		float min = 0.0f; // No minimum
		float max = 0.0f; // No maximum

		Vertex tempVertex = vertex; // Copy of pre-change vertex
		if (relative) {
			tempVertex.position += object->getPosition();
		}
		ImGui::PushID(vertexIndex);

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
				renderComp->setVertex(Vertex(tempVertex.position - object->getPosition(), tempVertex.normal, tempVertex.texCoords), vertexIndex);
				// some update 
			}
			else if (posChanged) {
				posChanged = false; // Reset flag
				renderComp->setVertex(tempVertex, vertexIndex);
				// some update 
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Normal")) {
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
				renderComp->setVertex(Vertex(tempVertex.position, tempVertex.normal, tempVertex.texCoords), vertexIndex);
				// some update 
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("TexCoords")) {
			bool texChanged = false; // Flag tracks change of texCoords inputs
			// Input for x
			texChanged |= ImGui::DragFloat("x", &tempVertex.texCoords.x, speed, min, max, "%.2f");
			ImGui::SameLine();

			// Input for y
			texChanged |= ImGui::DragFloat("y", &tempVertex.texCoords.y, speed, min, max, "%.2f");

			if (texChanged) {
				texChanged = false; // Reset flag
				renderComp->setVertex(Vertex(tempVertex.position, tempVertex.normal, tempVertex.texCoords), vertexIndex);
				// some update 
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
		ImGui::PopItemWidth();
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
