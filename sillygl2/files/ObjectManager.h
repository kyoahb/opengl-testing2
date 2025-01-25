#pragma once
#include "InstanceGroup.h"
#include "TextureManager.h"
#include <vector>
#include <string>
#include <algorithm>

class ObjectManager {
public:
	std::vector<InstanceGroup*> instanceGroups;
	ObjectManager();

	//InstanceGroup* addMesh(Mesh* mesh); // Converts mesh to instance group and adds it to list of instance groups rendered every frame
	InstanceGroup* createInstantiable(
		const std::string& name = "Unnamed Instantiable",
		const std::vector<Vertex>& _vertices = {},
		const std::vector<unsigned int>& _indices = {},
		const Material& _material = Material(),
		const glm::vec3& _position = glm::vec3(0.0f),
		const glm::quat& _rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		const glm::vec3& _scale = glm::vec3(1.0f)); // Creates an instance group with a single instance.
	void addInstanceGroup(InstanceGroup* instanceGroup); // Adds instance group to list of instance groups rendered every frame

	InstanceGroup* createCubeInstanceGroup(float width, float height, float depth, glm::vec3 centre, std::string name);

	std::vector<InstanceGroup*>* getInstanceGroups(); // Returns the list of instance groups
	void addCube(InstanceGroup* instanceGroup, const glm::vec3& centre, const glm::vec3& rotation, const glm::vec3& scale, const std::string& name);

private:
	unsigned int maxID = 0;
};
