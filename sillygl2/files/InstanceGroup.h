#pragma once
#include "Instance.h"
#include "Object.h"

class InstanceGroup : public VertexObject {
public:

	InstanceGroup(
		const std::string& _name = "Unnamed InstanceGroup",
		const std::vector<Vertex>& _vertices = {},
		const std::vector<unsigned int>& _indices = {},
		const std::vector<Texture*>& _textures = {},
		Shader* _shader = nullptr,
		const glm::vec3& _position = glm::vec3(0.0f),
		const glm::quat& _rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
		const glm::vec3& _scale = glm::vec3(1.0f));

	void draw();

	void rotateEuler(const glm::vec3& _rotation);
	void rotateQuat(const glm::quat& _rotation);
	void setRotation(const glm::quat& _rotation);
	void move(const glm::vec3& change);
	void addScale(const glm::vec3& _scale);


	void calcAndSendModel();

	const std::vector<Instance*>& getInstances();
	std::vector<Instance*> getInstancesByName(std::string name);
	Instance* getInstanceById(unsigned int id);
	void updateInstances();
	void addInstance(Instance* instance);

	void resizeModelBuffer();

	//void addInstance(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), std::string name = "");



private:
	std::vector<Instance*> instances; // List of instances rendered every frame

	unsigned int nextInstanceID = 0;

	unsigned int VAO, EBO, VBO, modelBuffer;
	unsigned int modelBufferSize = 1000; // Initial pre-gen size of model buffer
	unsigned int modelBufferIncrement = 1000; // Amount to increase model buffer size by when it runs out of space;
};