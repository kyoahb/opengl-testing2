#pragma once
#include <math.h>
#include <string>
#include "Useful.h"
#include <glm/gtx/quaternion.hpp>
class Camera
{
public:
	bool updatedView = false; // If view matrix has been updated
	glm::mat4 view = glm::mat4(1.0f); // View matrix
	glm::mat4 perspective; // = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // Perspective matrix
	
	Camera(); 

	void move(const glm::vec3& change);
	void rotateEuler(const glm::vec3& _rotation);
	void rotateQuat(const glm::quat& _rotation);

	const glm::vec3& getPosition();
	void setPosition(const glm::vec3& _position);
	
	const glm::vec3& getEulerRotation();
	const glm::quat& getQuatRotation();
	void setRotation(const glm::vec3& _rotation);
	void setRotation(const glm::quat& _rotation);

private:
	void updateView(); // Update view matrix
	void calculateFront(); // Calculate front vector
	glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f); // Position always in front
	glm::vec3 initialCameraFront = cameraFront; // Initial front position
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Position always above
	glm::vec3 position = glm::vec3(0.0f); // camera position as a vertex in the space
	glm::vec3 rotation = glm::vec3(0.0f); // Pitch (distance from straight up where 0 is up and 180 is down), Yaw (distance from forward rotation increasing going left), Roll (rotation around z-axis)
};
