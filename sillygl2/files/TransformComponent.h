#pragma once
#include <vector>
#include "Useful.h"
#include "Log.h"

class TransformComponent {
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 modelMatrix = glm::mat4(1.0f);

public:
	bool transformModified = false;
	TransformComponent(
		const glm::vec3& _position = glm::vec3(0.0f),
		const glm::quat& _rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		const glm::vec3& _scale = glm::vec3(1.0f));

	// getters
	const glm::vec3& getPosition() const;
	const glm::quat& getQuatRotation() const;
	const glm::vec3& getEulerRotation() const;
	const glm::vec3& getScale() const;
	const glm::mat4& getModelMatrix() const;

	// setters
	void setPosition(const glm::vec3& _position);
	void setQuatRotation(const glm::quat& _rotation);
	void setEulerRotation(const glm::vec3& _rotation);
	void setScale(const glm::vec3& _scale);

	// relative transformations
	void move(const glm::vec3& change);
	void rotateEuler(const glm::vec3& _rotation);
	void rotateQuat(const glm::quat& _rotation);
	void addScale(const glm::vec3& _scale);

	// calculate model matrix
	void update(); // update if model matrix requires modification
	void calculateModelMatrix();
};