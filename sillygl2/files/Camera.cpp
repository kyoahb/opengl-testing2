#include "Camera.h"
#include "Useful.h"

Camera::Camera() : speed(5.0f) {
	position = glm::vec3(0.0f, 0.0f, -3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::normalize(position - target);
	glm::vec3 true_up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(true_up, direction));
	cameraUp = glm::cross(direction, cameraRight);

	view = glm::lookAt(position, position + cameraFront, cameraUp);
}

void Camera::changeDirection(glm::vec3 changeDirection) {
	// Pitch, Yaw, Roll -> changeDirection
	// Yaw is flipped, think OpenGL just does that
	direction += glm::vec3(changeDirection.x, -1 * changeDirection.y, changeDirection.z);

	// Clamp the pitch to avoid flipping
	if (direction.x > 85.0f) {
		direction.x = 85.0f;
	}
	if (direction.x < -85.0f) {
		direction.x = -85.0f;
	}
	direction = vec3Clamp(direction, 360.0f);
	cameraFront = vec3Rotate(direction, glm::vec3(0.0f, 0.0f, 1.0f));
	view = glm::lookAt(position, position + cameraFront, cameraUp);
}

void Camera::move(glm::vec3 change) {
	position += change;

	view = glm::lookAt(position, position + cameraFront, cameraUp);
}

void Camera::setSpeed(float speed) {
	this->speed = speed;
}