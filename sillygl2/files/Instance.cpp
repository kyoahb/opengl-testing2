#include "Instance.h"

Instance::Instance(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, std::string _name) : position(_position), scale(_scale), name(_name) {
	rotation = glm::quat(glm::radians(_rotation));
	formModelMatrix();
	requestingUpdate = false;
}

void Instance::move(glm::vec3 change) {
	position += change;
	requestingUpdate = true;
	positionChanged = true;
}

void Instance::rotateEuler(glm::vec3 _rotation) {
	glm::vec3 filledRot = vec3Overfill(_rotation, 0.0f, 360.0f);
    rotation = glm::quat(glm::radians(filledRot)) * rotation;
	requestingUpdate = true;
	rotationChanged = true;
}

void Instance::rotateQuat(glm::quat _rotation) {
    rotation = _rotation * rotation;
	requestingUpdate = true;
    rotationChanged = true;
}

void Instance::addScale(glm::vec3 _scale) {
	scale += _scale;
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