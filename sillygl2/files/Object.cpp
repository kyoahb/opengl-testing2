#include "Object.h"

GameObject::GameObject(glm::vec3 _position, std::string _name)
    : id(-1), position(_position), name(_name), rotation(glm::vec3(0.0f, 0.0f, 0.0f)), vertices({}), indices({}), attachedCamera(nullptr), children({}) {}

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
}

void GameObject::rotate(glm::mat4 rotationMatrix) {
    for (auto& vert : vertices) {
        glm::vec3 translated = vert - position;
        glm::vec4 simd_translated = glm::vec4(translated, 1.0f);
        glm::vec4 simd_result = rotationMatrix * simd_translated;
        vert = glm::vec3(simd_result) + position;
    }
}

void GameObject::scaleInPlace(glm::vec3 scale) {
    for (auto& vert : vertices) {
        vert -= position;
        vert *= scale;
        vert += position;
    }
}

void GameObject::scale(glm::vec3 scale) {
    position *= scale;
    for (auto& vert : vertices) {
        vert *= scale;
    }
}

void GameObject::getAABB(glm::vec3& min, glm::vec3& max) const {
    if (vertices.empty()) {
        min = max = position;
        return;
    }
    min = max = vertices[0];
    for (const auto& vert : vertices) {
        min = glm::min(min, vert);
        max = glm::max(max, vert);
    }
}

void GameObject::attachCamera(Camera* camera) {
    this->attachedCamera = camera;
}

void GameObject::attachChild(GameObject* child) {
	this->children.push_back(child);
}
