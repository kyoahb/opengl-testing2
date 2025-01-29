#include "ObjectManager.h"

ObjectManager::ObjectManager() {}

void ObjectManager::addInstanceGroup(InstanceGroup* instanceGroup) {
	instanceGroup->setId(maxID++);
	instanceGroups.push_back(instanceGroup);
}

std::vector<InstanceGroup*>* ObjectManager::getInstanceGroups() {
	return &instanceGroups;
}

InstanceGroup* ObjectManager::createInstantiable(
	const std::string& _name,
	const std::vector<Vertex>& _vertices,
	const std::vector<unsigned int>& _indices,
	const Material& _material,
	const glm::vec3& _position,
	const glm::quat& _rotation,
	const glm::vec3& _scale) {

	InstanceGroup* instanceGroup = new InstanceGroup(_name, _vertices, _indices, _material);
	Instance* instance = new Instance("instance", _position, _rotation, _scale);

	instanceGroups.push_back(instanceGroup);

	return instanceGroup;

}

InstanceGroup* ObjectManager::createCubeInstanceGroup(float width, float height, float depth, glm::vec3 centre, std::string name) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	vertices = {
		Vertex(glm::vec3(-width / 2, -height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(width / 2, -height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(width / 2, height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(-width / 2, height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(-width / 2, -height / 2, depth / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(width / 2, -height / 2, depth / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(width / 2, height / 2, depth / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(-width / 2, height / 2, depth / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
	};

	indices = {
		0, 1, 2,
		2, 3, 0,
		1, 5, 6,
		6, 2, 1,
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3
	};
	Texture* texture = TextureManager::createTexture(TextureType::Diffuse, "textures/hlbox.jpg");

	Material material = Material();
	material.diffuse = texture;

	InstanceGroup* cubes = new InstanceGroup(name, vertices, indices, material, centre);
	addInstanceGroup(cubes);

	return cubes;
}

void ObjectManager::addCube(InstanceGroup* instanceGroup, const glm::vec3& centre, const glm::vec3& rotation, const glm::vec3& scale, const std::string& name) {
	Instance* cube = new Instance("cube", centre, rotation, scale);
	instanceGroup->addInstance(cube);
}