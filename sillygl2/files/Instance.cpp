#include "Instance.h"

Instance::Instance(const std::string& _name,
    const glm::vec3& _position,
    const glm::quat& _rotation,
    const glm::vec3& _scale) 
    : GameObject(_name, _position, _rotation, _scale) {
	formModelMatrix();
	requestingUpdate = false;
}

void Instance::move(const glm::vec3& change) {
    GameObject::move(change);
	requestingUpdate = true;
	positionChanged = true;
}

void Instance::rotateEuler(const glm::vec3& _rotation) {
    GameObject::rotateEuler(_rotation);
	requestingUpdate = true;
	rotationChanged = true;
}

void Instance::rotateQuat(const glm::quat& _rotation) {
    GameObject::rotateQuat(_rotation);
	requestingUpdate = true;
    rotationChanged = true;
}

void Instance::setRotation(const glm::quat& _rotation) {
	GameObject::setRotation(_rotation);
	requestingUpdate = true;
	rotationChanged = true;
}


void Instance::addScale(const glm::vec3& _scale) {
    GameObject::addScale(_scale);
	requestingUpdate = true;
    scaleChanged = true;
}

glm::mat4 Instance::getModelMatrix() {
	return model;
}


void Instance::formModelMatrix() {
    if (positionChanged) {
        translationMatrix = glm::translate(glm::mat4(1.0f), position);
    }

    if (rotationChanged) {
        rotationMatrix = glm::toMat4(rotation);
    }

    if (scaleChanged) {
        scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    }

    if (positionChanged || rotationChanged || scaleChanged) {
        model = translationMatrix * rotationMatrix * scaleMatrix;
        positionChanged = false;
        rotationChanged = false;
        scaleChanged = false;
    }

}