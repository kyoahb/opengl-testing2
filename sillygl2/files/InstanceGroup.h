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



	// Camera
	Camera* getAttachedCamera();
	void attachCamera(Camera* camera);
	bool isCameraAttached();

	// Instances
	const std::vector<Instance*>& getInstances();
	std::vector<Instance*> getInstancesByName(std::string name);
	Instance* getInstanceById(unsigned int id);
	void updateInstances();
	void addInstance(Instance* instance);
	void removeInstance(Instance* instance);

	// Vertices
	void setVertex(const Vertex& vertex, unsigned int index); // Replace vertex at index with new vertex
	void removeVertex(unsigned int index); // Remove vertex at index
	void setVertices(const std::vector<Vertex>& _vertices); // Replace vertices with new vertices
	void addVertices(const std::vector<Vertex>& _vertices); // Add new vertices on top of existing ones
	void addVerticesIndices(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices); // Adds vertices and indices, ensuring they are relative to each other. Indices added here cannot access existing indices.
	// Indices
	void setIndices(const std::vector<unsigned int>& _indices); // Replace indices with new indices
	void addIndices(const std::vector<unsigned int>& _indices); // Add new indices on top of existing ones

	// Shader
	void setShader(Shader* _shader);

	// Model buffers
	void calcAndSendModel(); // Calculate GROUP model and send to shader
	void batchResendModelMatrices(); // Resends individual model buffers to GPU
	void resizeModelBuffer(); // Copies old individual model(s) buffer to new larger buffer

	// GameObject overrides with flags added
	void rotateEuler(const glm::vec3& _rotation);
	void rotateQuat(const glm::quat& _rotation);
	void setRotation(const glm::quat& _rotation);
	void move(const glm::vec3& change);
	void addScale(const glm::vec3& _scale);

	void draw();



private:
	std::vector<Instance*> instances; // List of instances rendered every frame

	unsigned int nextModelBufferIndex = 0; // Next available index in model buffer. 
	unsigned int nextID = 0; // Next available ID for an instance in this group. Can never be reduced
	Camera* attachedCamera = nullptr;
	unsigned int modelBuffer;
	unsigned int modelBufferSize = 1000; // Initial pre-gen size of model buffer
	unsigned int modelBufferIncrement = 1000; // Amount to increase model buffer size by when it runs out of space;
};