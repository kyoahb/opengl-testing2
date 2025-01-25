#include "Object.h"

GameObject::GameObject(
    const std::string& _name,
    const glm::vec3& _position,
    const glm::quat& _rotation,
    const glm::vec3& _scale)
    : id(-1), position(_position), scale(_scale), name(_name), rotation(_rotation)
{
	if (scale == glm::vec3(0.0f)) {
		spdlog::warn("Scale of object {} is 0, setting to 1.0f", name);
		scale = glm::vec3(1.0f);
	}
	if (name == "") {
		spdlog::warn("Name of object is empty, setting to GameObject");
	}
}

const std::string& GameObject::getName() const {
	return name;
}

const glm::vec3& GameObject::getPosition() const {
	return position;
}

const glm::quat& GameObject::getQuatRotation() const {
	return rotation;
}

const glm::vec3& GameObject::getEulerRotation() const {
	return glm::degrees(glm::eulerAngles(rotation));
}

const glm::vec3& GameObject::getScale() const {
	return scale;
}

int GameObject::getId() const {
	return id;
}

void GameObject::setId(int _id) {
	id = _id;
}

void GameObject::move(const glm::vec3& change) {
    position += change;
}

void GameObject::rotateEuler(const glm::vec3& _rotation) {
    glm::vec3 filledRot = vec3Overfill(_rotation, -180.0f, 180.0f);
    rotation = glm::quat(glm::radians(filledRot)) * rotation;
	rotation = glm::normalize(rotation);
}

void GameObject::rotateQuat(const glm::quat& _rotation) {
	rotation = _rotation * rotation;
	rotation = glm::normalize(rotation);
}

void GameObject::setRotation(const glm::quat& _rotation) {
	rotation = _rotation;
}

void GameObject::addScale(const glm::vec3& _scale) {
    scale += _scale;
}

glm::mat4 GameObject::calculateModelMatrix() const {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model *= glm::toMat4(rotation);
	model = glm::scale(model, scale);
	return model;
}

VertexObject::VertexObject(
    const std::string& _name,
    const std::vector<Vertex>& _vertices,
    const std::vector<unsigned int>& _indices,
	const Material& _material,
    const glm::vec3& _position,
    const glm::quat& _rotation,
    const glm::vec3& _scale) 
	: vertices(_vertices), indices(_indices), material(_material), GameObject(_name, _position, _rotation, _scale)
{
	if (vertices.size() == 0) {
		spdlog::warn("Vertices of object {} is empty", name);
	}
	if (indices.size() == 0) {
		spdlog::warn("Indices of object {} is empty", name);
	}
	if (vertices.size() > 1 and indices.size() > 1) {
		if (*std::max_element(indices.begin(), indices.end()) > vertices.size() - 1) { // Invalid indices
			spdlog::warn("Indices of object {} are invalid", name);
		}
	}
}

const std::vector<Vertex>& VertexObject::getVertices() const {
	return vertices;
};

void VertexObject::setVertex(const Vertex& vertex, unsigned int index) {
	vertices[index] = vertex;
}

const std::vector<unsigned int>& VertexObject::getIndices() const {
	return indices;
};
const Material& VertexObject::getMaterial() const {
	return material;
};
