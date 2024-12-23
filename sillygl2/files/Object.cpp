#include "Object.h"

bool anyTransformationsThisFrame = false;

GameObject::GameObject(glm::vec3 _position, std::string _name, bool _rendered)
    : id(-1), position(_position), name(_name), rotation(glm::vec3(0.0f, 0.0f, 0.0f)), vertices({}), indices({}), attachedCamera(nullptr), children({}), rendered(_rendered) {}

void GameObject::setTransformedThisFrame() {
    if(rendered)
    {
        anyTransformationsThisFrame = true;
		transformedThisFrame = true;
        //transformedObjectsThisFrame.push_back(this);
    }
}

void GameObject::move(glm::vec3 change) {
    // Move self
    position += change;
    for (auto& vert : vertices) {
        vert += change;
    }

    // Move camera
    if (attachedCamera != nullptr) {
        attachedCamera->position = position;
        attachedCamera->updateView();
    }

	// Move children
	for (auto& child : children) {
		child->move(change);
	}
    setTransformedThisFrame();
}

void GameObject::rotate(glm::mat4 rotationMatrix, glm::vec3 _rotation) {
    rotation = vec3Clamp(rotation + _rotation, 360.0f);
    rotation = vec3Clamp(rotation, -360.0f);
    for (auto& vert : vertices) {
        glm::vec3 translated = vert - position;
        glm::vec4 simd_translated = glm::vec4(translated, 1.0f);
        glm::vec4 simd_result = rotationMatrix * simd_translated;
        vert = glm::vec3(simd_result) + position;
    }
    setTransformedThisFrame();
}

void GameObject::scaleInPlace(glm::vec3 scale) {
    for (auto& vert : vertices) {
        vert -= position;
        vert *= scale;
        vert += position;
    }
    setTransformedThisFrame();
}

void GameObject::scale(glm::vec3 scale) {
    position *= scale;
    for (auto& vert : vertices) {
        vert *= scale;
    }
    setTransformedThisFrame();
}

void GameObject::getAABB(glm::vec3& min, glm::vec3& max) {
    if (vertices.empty()) {
        min = max = position;
        return;
    }
    min = max = vertices[0];
    for (const auto& vert : vertices) {
        min = glm::min(min, vert);
        max = glm::max(max, vert);
    }
    setTransformedThisFrame();
}

void GameObject::attachCamera(Camera* camera) {
    this->attachedCamera = camera;
}

void GameObject::attachChild(GameObject* child) {
	this->children.push_back(child);
}
