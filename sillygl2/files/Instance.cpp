#include "Instance.h"

Instance::Instance(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, std::string _name) : position(_position), scale(_scale), name(_name) {
	rotation = glm::quat(glm::radians(_rotation));
	formModelMatrix();
	requestingUpdate = false;
}

void Instance::move(glm::vec3 change) {
	position += change;
	requestingUpdate = true;
}

void Instance::rotateEuler(glm::vec3 _rotation) {
	glm::vec3 filledRot = vec3Overfill(_rotation, 0.0f, 360.0f);
    rotation = glm::quat(glm::radians(filledRot)) * rotation;
	requestingUpdate = true;
}

void Instance::rotateQuat(glm::quat _rotation) {
	rotation = _rotation * rotation;
	requestingUpdate = true;
}

void Instance::addScale(glm::vec3 _scale) {
	scale += _scale;
	requestingUpdate = true;
}

glm::mat4 Instance::getModelMatrix() {
	return model;
}


void Instance::formModelMatrix() {
    bool positionChanged = (position != lastPosition);
    bool rotationChanged = (rotation != lastRotation);
    bool scaleChanged = (scale != lastScale);

    if (positionChanged) {
        translationMatrix = glm::translate(glm::mat4(1.0f), position);
        lastPosition = position;
    }

    if (rotationChanged) {
        rotationMatrix = glm::toMat4(rotation);
        lastRotation = rotation;
    }

    if (scaleChanged) {
        scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        lastScale = scale;
    }

    if (positionChanged || rotationChanged || scaleChanged) {
        model = translationMatrix * rotationMatrix * scaleMatrix;
    }

}