#include "ObjectManager.h"

ObjectManager::ObjectManager() {}

// instance groups
void ObjectManager::addInstanceGroup(std::shared_ptr<InstanceGroup> instanceGroup) {
	instanceGroup->setId(nextId++);
	instanceGroups.push_back(instanceGroup);
}

std::vector<std::shared_ptr<InstanceGroup>>* ObjectManager::getInstanceGroups() {
	return &instanceGroups;
}

// objects
void ObjectManager::addObject(std::shared_ptr<Object> object) {
	object->setId(nextId++);
	gameObjects.push_back(object);
}

std::shared_ptr<Object> ObjectManager::createObject(const std::string& name) {
	std::shared_ptr<Object> object = std::make_shared<Object>(name);
	addObject(object);
	return object;
}

std::vector<std::shared_ptr<Object>>* ObjectManager::getObjects() {
	return &gameObjects;
}

// other methods
std::shared_ptr<InstanceGroup> ObjectManager::createCubeInstanceGroup(
	float width, 
	float height, 
	float depth, 
	const glm::vec3& centre, 
	const std::string& name) 
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

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

	std::shared_ptr<InstanceGroup> cubes = std::make_shared<InstanceGroup>(name);
	cubes->getRenderComponent()->addVerticesIndices(vertices, indices);

	addInstanceGroup(cubes);

	return cubes;
}

void ObjectManager::draw() {
	for (auto& instanceGroup : instanceGroups) {
		instanceGroup->draw();
	}
	for (auto& gameObject : gameObjects) {
		gameObject->draw();
	}
	// TODO: add rendered object drawing
}