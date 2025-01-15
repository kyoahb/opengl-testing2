#include "Camera.h"

Camera::Camera() {
	view = glm::lookAt(position, position + cameraFront, cameraUp);
}

void Camera::calculateFront() {
	glm::vec3 front;
	front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	front.y = sin(glm::radians(rotation.x));
	front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	cameraFront = glm::normalize(front);
}

void Camera::rotateEuler(const glm::vec3& _rotation) {
	glm::vec3 flippedR = glm::vec3(-1 * _rotation.x, _rotation.y, _rotation.z);
	rotation += flippedR;
	rotation = vec3Overfill(rotation, -180.0f, 180.0f);

	rotation.x = glm::clamp(rotation.x, -89.0f, 89.0f);

	calculateFront();
	updateView();
}

void Camera::rotateQuat(const glm::quat& _rotation) {
	rotation = glm::degrees(glm::eulerAngles(_rotation)) + rotation;
	rotation = vec3Overfill(rotation, -180.0f, 180.0f);

	rotation.x = glm::clamp(rotation.x, -89.0f, 89.0f);

	calculateFront();
	updateView();

}


// Use if camera is not attached to a GameObject, though it really should be
void Camera::move(const glm::vec3& change) {
	position += change;

	updateView();
}

void Camera::setRotation(const glm::vec3& _rotation) {
	rotation = _rotation;
	rotation = vec3Overfill(rotation, -180.0f, 180.0f);
	rotation.x = glm::clamp(rotation.x, -89.0f, 89.0f);

	calculateFront();
	updateView();
}

void Camera::setRotation(const glm::quat& _rotation) {
	rotation = glm::degrees(glm::eulerAngles(_rotation));
	rotation = vec3Overfill(rotation, -180.0f, 180.0f);
	rotation.x = glm::clamp(rotation.x, -89.0f, 89.0f);

	calculateFront();
	updateView();
}

void Camera::updateView() {
	view = glm::lookAt(position, position + cameraFront, cameraUp);
	updatedView = true;
}

const glm::vec3& Camera::getPosition() {
	return position;
}

void Camera::setPosition(const glm::vec3& _position) {
	position = _position;
	updateView();
}

const glm::vec3& Camera::getEulerRotation() {
	return rotation;
}

const glm::quat& Camera::getQuatRotation() {
	return glm::quat(glm::radians(rotation));
}
