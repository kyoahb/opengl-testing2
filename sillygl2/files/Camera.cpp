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

	if (direction.y < 0.0f) {
		direction.y += 360.0f;
	}

	// Fmod direction between 0-360
	direction = vec3Clamp(direction, 360.0f);

	// Clamp the pitch around 0 (straight up) and 180 (straight down) to avoid flipping
	if (direction.x < 5.0f) {
		direction.x = 5.0f;
	}
	if (direction.x > 175.0f) {
		direction.x = 175.0f;
	}
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