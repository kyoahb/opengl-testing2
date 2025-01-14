#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	view = glm::lookAt(position, position + cameraFront, cameraUp);
}

// Change direction of camera and resulting cameraFront position and view matrix
void Camera::rotateEuler(glm::vec3 _rotation) {
	// Yaw is flipped, think OpenGL just does that
	rotation += glm::vec3(_rotation.x, -1 * _rotation.y, _rotation.z);

	// Overfill direction between 0-360
	rotation = vec3Overfill(rotation, 0.0f, 360.0f);

	// Clamp the pitch around 0 (straight up) and 180 (straight down) to avoid flipping
	rotation.x = glm::clamp(rotation.x, 5.0f, 175.0f);

	cameraFront = vec3Rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	updateView();
}

// Use if camera is not attached to a GameObject, though it really should be
void Camera::move(glm::vec3 change) {
	position += change;

	updateView();
}

void Camera::updateView() {
	view = glm::lookAt(position, position + cameraFront, cameraUp);
	updatedView = true;
}

const glm::vec3& Camera::getPosition() {
	return position;
}

void Camera::setPosition(glm::vec3& _position) {
	position = _position;
	updateView();
}

const glm::vec3& Camera::getRotation() {
	return rotation;
}
