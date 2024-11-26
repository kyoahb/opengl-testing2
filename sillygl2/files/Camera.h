#pragma once

#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <math.h>
#include <string>
#include "Useful.h"
class Camera
{
public:
	glm::vec3 position; // camera position as a vertex in the space
	glm::vec3 direction; // Pitch, -Yaw, Roll

	glm::mat4 view = glm::mat4(1.0f);
	Camera();
	void changeDirection(glm::vec3 changeDirection);

	void move(glm::vec3 change);
	void setSpeed(float speed);

private:
	glm::vec3 cameraFront; // Position always in front
	glm::vec3 cameraRight; // Position always to the right
	glm::vec3 cameraUp; // Position always above
	float speed;
};
