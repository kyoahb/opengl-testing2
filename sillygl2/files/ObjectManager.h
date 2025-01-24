#pragma once
#include "Mesh.h"
#include "InstanceGroup.h"
#include "TextureManager.h"
#include <vector>
#include <string>
#include <algorithm>

class ObjectManager {
public:
	std::vector<InstanceGroup*> instanceGroups;
	ObjectManager(TextureManager* _textureManager);

	TextureManager* textureManager;

	InstanceGroup* addMesh(Mesh* mesh); // Converts mesh to instance group and adds it to list of instance groups rendered every frame

	void addInstanceGroup(InstanceGroup* instanceGroup); // Adds instance group to list of instance groups rendered every frame

	InstanceGroup* createCubeInstanceGroup(float width, float height, float depth, glm::vec3 centre, std::string name);

	std::vector<InstanceGroup*>* getInstanceGroups(); // Returns the list of instance groups
	void addCube(InstanceGroup* instanceGroup, const glm::vec3& centre, const glm::vec3& rotation, const glm::vec3& scale, const std::string& name);

private:
	unsigned int maxID = 0;
};
