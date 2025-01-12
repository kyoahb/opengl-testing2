#pragma once
#include "Instance.h"

class InstanceGroup {
public:
	std::vector<Instance*> instances; // List of instances rendered every frame

	unsigned int nextInstanceID = 0;
	std::vector<Vertex> vertices = {};
	std::vector<unsigned int> indices = {};
	std::vector<Texture*> textures = {};
	Shader* shader;

	glm::vec3 position = glm::vec3(0.0f); // Offset position applied to all instances
	glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f))); // Offset rotation applied to all instances 
	glm::vec3 scale = glm::vec3(1.0f); // Offset scale applied to all instances


	std::string name = ""; // Group name

	InstanceGroup(Shader* _shader, std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture*> _textures, std::string _name = "Unnamed InstanceGroup");

	void draw();

	void rotateEuler(glm::vec3 _rotation);
	void rotateQuat(glm::quat _rotation);
	void move(glm::vec3 change);
	void addScale(glm::vec3 _scale);

	void calcAndSendModel();

	std::vector<Instance*> getInstancesByName(std::string name);
	Instance* getInstanceById(unsigned int id);
	void updateInstances();
	void addInstance(Instance* instance);

	void resizeModelBuffer();

	//void addInstance(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), std::string name = "");



private:
	unsigned int VAO, EBO, VBO, modelBuffer;
	unsigned int modelBufferSize = 1000; // Initial pre-gen size of model buffer
	unsigned int modelBufferIncrement = 1000; // Amount to increase model buffer size by when it runs out of space
};