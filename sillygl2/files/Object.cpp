#include "Object.h"

std::vector<GameObject*> transformedObjects = {};
std::vector<GameObject*> vertChangedObjects = {};

GameObject::GameObject(glm::vec3 _position, std::string _name, bool _visible)
    : id(-1), position(_position), name(_name), rotation(glm::vec3(0.0f, 0.0f, 0.0f)), vertices({}), indices({}), attachedCamera(nullptr), children({}), visible(_visible)
{
}

void GameObject::setObjectEvent(ObjectEvent e) {
	if (visible == false) { // No point, saves performance
		return;
	}

    event = e;

	if (e == Object_Transformation) {
		transformedObjects.push_back(this);
	}
	else if (e == Object_Vertices_Changed) {
		vertChangedObjects.push_back(this);
	}
}

glm::vec3& GameObject::getPosition() {
	return position;
}

glm::vec3& GameObject::getRotation() {
	return rotation;
}

std::vector<GameObject*>& GameObject::getChildren() {
	return children;

}
std::vector<glm::vec3>& GameObject::getVertices() {
	return vertices;
}
std::vector<unsigned int>& GameObject::getIndices() {
	return indices;

}
Camera* GameObject::getAttachedCamera() {
	return attachedCamera;
}

void GameObject::addVerticesIndices(std::vector<glm::vec3>& _vertices, std::vector<unsigned int>& _indices, bool toRerender) {
    vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
    indices.insert(indices.end(), _indices.begin(), _indices.end());
    if (toRerender) {
        setObjectEvent(Object_Vertices_Changed);
    }
}


void GameObject::move(glm::vec3& change) {
    // Move self
    position += change;
    for (auto& vert : vertices) {
        vert += change;
    }

    // Move camera
    if (attachedCamera != nullptr) {
        attachedCamera->setPosition(position);
    }

	// Move children
	for (auto& child : children) {
		child->move(change);
	}
    setObjectEvent(Object_Transformation);
}

void GameObject::rotateWithMatrix(glm::mat4& rotationMatrix, glm::vec3 _rotation) {
    rotation += _rotation;
    rotation = vec3Overfill(rotation, 0.0f, 360.0f);
    for (auto& vert : vertices) {
        glm::vec3 translated = vert - position;
        glm::vec4 simd_translated = glm::vec4(translated, 1.0f);
        glm::vec4 simd_result = rotationMatrix * simd_translated;
        vert = glm::vec3(simd_result) + position;
    }
    setObjectEvent(Object_Transformation);
}

void GameObject::rotate(glm::vec3& _rotation) {
	glm::mat4 rotationMatrix = mat4Rotate(_rotation);
	rotateWithMatrix(rotationMatrix, _rotation);
}

void GameObject::scaleInPlace(glm::vec3& scale) {
    for (auto& vert : vertices) {
        vert -= position;
        vert *= scale;
        vert += position;
    }
    setObjectEvent(Object_Transformation);
}

void GameObject::scale(glm::vec3& scale) {
    position *= scale;
    for (auto& vert : vertices) {
        vert *= scale;
    }
    setObjectEvent(Object_Transformation);
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
    setObjectEvent(Object_Transformation);
}

void GameObject::attachCamera(Camera* camera) {
    this->attachedCamera = camera;
    this->isCameraAttached = true;
}

void GameObject::attachChild(GameObject* child) {
	this->children.push_back(child);
}
