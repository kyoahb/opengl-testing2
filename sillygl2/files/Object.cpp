#include "Object.h"

GameObject::GameObject(
    const std::string& _name,
    const glm::vec3& _position,
    const glm::quat& _rotation,
    const glm::vec3& _scale)
    : id(-1), position(_position), scale(_scale), name(_name), rotation(_rotation)
{
}

const std::string& GameObject::getName() const {
	return name;
}

const glm::vec3& GameObject::getPosition() const {
	return position;
}

const glm::quat& GameObject::getRotation() const {
	return rotation;
}

const glm::vec3& GameObject::getScale() const {
	return scale;
}

int GameObject::getId() const {
	return id;
}

void GameObject::move(const glm::vec3& change) {
    position += change;
}

void GameObject::rotateEuler(const glm::vec3& _rotation) {
    glm::vec3 filledRot = vec3Overfill(_rotation, 0.0f, 360.0f);
    rotation = glm::quat(glm::radians(filledRot)) * rotation;
}

void GameObject::rotateQuat(const glm::quat& _rotation) {
	rotation = _rotation * rotation;
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
    const std::vector<Texture*>& _textures,
    Shader* _shader,
    const glm::vec3& _position,
    const glm::quat& _rotation,
    const glm::vec3& _scale) 
	: vertices(_vertices), indices(_indices), textures(_textures), shader(_shader), GameObject(_name, _position, _rotation, _scale)
{

}


const std::vector<Vertex>& VertexObject::getVertices() const {
	return vertices;
};
const std::vector<unsigned int>& VertexObject::getIndices() const {
	return indices;
};
const std::vector<Texture*>& VertexObject::getTextures() const {
	return textures;
};
Shader* VertexObject::getShader() const {
	return shader;
};