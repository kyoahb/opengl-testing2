#pragma once

#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <math.h>
#include <string>
#include "Useful.h"

class Camera
{
public:
	bool updatedView = false; // If view matrix has been updated
	glm::mat4 view = glm::mat4(1.0f); // View matrix
	Camera(); 
	void changeDirection(glm::vec3 changeDirection); // Change direction of camera and resulting cameraFront position and view matrix
	void move(glm::vec3 change); // Move camera position

	glm::vec3& getPosition();
	void setPosition(glm::vec3& _position);
	glm::vec3& getDirection();

private:
	void updateView(); // Update view matrix

	glm::vec3 cameraFront; // Position always in front
	glm::vec3 cameraUp; // Position always above
	glm::vec3 position; // camera position as a vertex in the space
	glm::vec3 direction; // Pitch (distance from straight up where 0 is up and 180 is down), Yaw (distance from forward rotation increasing going left), Roll (rotation around z-axis)
};
