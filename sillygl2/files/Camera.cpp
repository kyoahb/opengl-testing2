#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, 0.0f);

	view = glm::lookAt(position, position + cameraFront, cameraUp);
}

// Change direction of camera and resulting cameraFront position and view matrix
void Camera::changeDirection(glm::vec3 changeDirection) {
	// Yaw is flipped, think OpenGL just does that
	direction += glm::vec3(changeDirection.x, -1 * changeDirection.y, changeDirection.z);

	// Overfill direction between 0-360
	direction = vec3Overfill(direction, 0.0f, 360.0f);

	// Clamp the pitch around 0 (straight up) and 180 (straight down) to avoid flipping
	direction.x = glm::clamp(direction.x, 5.0f, 175.0f);

	cameraFront = vec3Rotate(direction, glm::vec3(0.0f, 1.0f, 0.0f));
	updateView();
}

// Use if camera is not attached to a GameObject, though it really should be
void Camera::move(glm::vec3 change) {
	position += change;

	updateView();
}

void Camera::updateView() {
	view = glm::lookAt(position, position + cameraFront, cameraUp);
}

glm::vec3& Camera::getPosition() {
	return position;
}

void Camera::setPosition(glm::vec3& _position) {
	position = _position;
	updateView();
}

glm::vec3& Camera::getDirection() {
	return direction;
}
