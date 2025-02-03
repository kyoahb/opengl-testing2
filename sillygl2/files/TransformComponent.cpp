#include "TransformComponent.h"

/* Transform */
TransformComponent::TransformComponent(
	const glm::vec3& _position,
	const glm::quat& _rotation,
	const glm::vec3& _scale)
	: position(_position), scale(_scale), rotation(_rotation)
{
	if (scale == glm::vec3(0.0f)) {
		spdlog::warn("Scale of object transform is 0.0f, setting to 1.0f");
		scale = glm::vec3(1.0f);
	}

	calculateModelMatrix();
}

/* Getters */

const glm::vec3& TransformComponent::getPosition() const {
	return position;
}

const glm::quat& TransformComponent::getQuatRotation() const {
	return rotation;
}

const glm::vec3& TransformComponent::getEulerRotation() const {
	return glm::degrees(glm::eulerAngles(rotation));
}

const glm::vec3& TransformComponent::getScale() const {
	return scale;
}

const glm::mat4& TransformComponent::getModelMatrix() const {
	return modelMatrix;
}

/* Setters */

void TransformComponent::setPosition(const glm::vec3& _position) {
	position = _position;
	transformModified = true;
}

void TransformComponent::setQuatRotation(const glm::quat& _rotation) {
	rotation = _rotation;
	transformModified = true;
}

void TransformComponent::setEulerRotation(const glm::vec3& _rotation) {
	rotation = glm::quat(glm::radians(_rotation));
	transformModified = true;
}

void TransformComponent::setScale(const glm::vec3& _scale) {
	scale = _scale;
	transformModified = true;
}

/* Relative transformations */

void TransformComponent::move(const glm::vec3& change) {
	position += change;
	transformModified = true;
}

void TransformComponent::rotateEuler(const glm::vec3& _rotation) {
	glm::vec3 filledRot = vec3Overfill(_rotation, -180.0f, 180.0f);
	rotation = glm::quat(glm::radians(filledRot)) * rotation;
	rotation = glm::normalize(rotation);
	transformModified = true;
}

void TransformComponent::rotateQuat(const glm::quat& _rotation) {
	rotation = _rotation * rotation;
	rotation = glm::normalize(rotation);
	transformModified = true;
}

void TransformComponent::addScale(const glm::vec3& _scale) {
	scale += _scale;
	transformModified = true;
}

/* Calculate model matrix */

void TransformComponent::update() {
	if (transformModified) {
		calculateModelMatrix();
		transformModified = false;
	}
}

void TransformComponent::calculateModelMatrix() {
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix *= glm::toMat4(rotation);
	modelMatrix = glm::scale(modelMatrix, scale);
}
