#pragma once
#include "Object.h"
#include "InstanceGroup.h"

class ObjectManager {
public:

	std::vector<std::shared_ptr<Object>> gameObjects;
	std::vector<std::shared_ptr<InstanceGroup>> instanceGroups;

	ObjectManager();

	void addInstanceGroup(std::shared_ptr<InstanceGroup> instanceGroup);
	std::vector<std::shared_ptr<InstanceGroup>>* getInstanceGroups();

	void addObject(std::shared_ptr<Object> object);
	std::shared_ptr<Object> createObject(const std::string& name = "Unnamed Object");
	std::vector<std::shared_ptr<Object>>* getObjects();

	std::shared_ptr<InstanceGroup> createCubeInstanceGroup(
		float width = 1.0f, 
		float height = 1.0f, 
		float depth = 1.0f, 
		const glm::vec3& centre = glm::vec3(0.0f), 
		const std::string& name = "cubes");

	void draw();

private:
	unsigned int nextId = 0;
};