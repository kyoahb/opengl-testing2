#pragma once
#include <vector>
#include "Vertex.h"
#include "Material.h"
#include "Useful.h"
#include "Log.h"

class Object;

class Transform {
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 modelMatrix = glm::mat4(1.0f);

public:
	bool transformModified = false;
	Transform(
		const glm::vec3& _position = glm::vec3(0.0f),
		const glm::quat& _rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		const glm::vec3& _scale = glm::vec3(1.0f));

	// getters
	const glm::vec3& getPosition() const;
	const glm::quat& getQuatRotation() const;
	const glm::vec3& getEulerRotation() const;
	const glm::vec3& getScale() const;
	const glm::mat4& getModelMatrix() const;

	// setters
	void setPosition(const glm::vec3& _position);
	void setQuatRotation(const glm::quat& _rotation);
	void setEulerRotation(const glm::vec3& _rotation);
	void setScale(const glm::vec3& _scale);

	// relative transformations
	void move(const glm::vec3& change);
	void rotateEuler(const glm::vec3& _rotation);
	void rotateQuat(const glm::quat& _rotation);
	void addScale(const glm::vec3& _scale);
	
	// calculate model matrix
	void update(); // update if model matrix requires modification
	void calculateModelMatrix();
};

class RenderComponent {
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::unique_ptr<Shader> shader;

public:
	GLuint VAO, VBO, EBO;
	std::shared_ptr<Material> material;

	RenderComponent(
		const std::vector<Vertex>& _vertices = {},
		const std::vector<unsigned int>& _indices = {},
		std::shared_ptr<Material> _material = std::make_shared<Material>(),
		std::unique_ptr<Shader> _shader = std::make_unique<Shader>("shaders/material_shader.vert", "shaders/material_shader.frag"));

	// getters
	const std::vector<Vertex>& getVertices() const;
	const std::vector<unsigned int>& getIndices() const;
	const std::shared_ptr<Material>& getMaterial() const;
	Shader* getShader() const;

	// setters
	void setVertices(const std::vector<Vertex>& _vertices);
	void setIndices(const std::vector<unsigned int>& _indices);
	void setMaterial(std::shared_ptr<Material> _material);
	void setVertex(const Vertex& vertex, unsigned int index);
	void setShader(std::unique_ptr<Shader> _shader);
	
	// other methods
	void removeVertex(unsigned int index);
	void addVertices(const std::vector<Vertex>& _vertices);
	void addIndices(const std::vector<unsigned int>& _indices);
	void addVerticesIndices(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices);

	void draw() const; // Sets material, and draws single object using vertices and indices.
	void setupBuffers(); // Sets up VAO, VBO, EBO, and modelBuffer
	void setupMaterial(); // Sets up material by binding it to shader

};

class PhysicsComponent {

};

class Object{
public:
	unsigned int id;
	std::string name;

	Transform transform;
	std::unique_ptr<RenderComponent> renderComponent;
	//std::unique_ptr<PhysicsComponent> physicsComponent;

	Object(const std::string& _name = "Unnamed Object");


	void addRenderComponent();

	void draw(); // Update object if necessary and then draw. Requires a render component
	void update(); // Resends group matrix if transform has been updated

};

class Instance2 {
public:
	unsigned int id;
	unsigned int modelBufferIndex;
	unsigned int relativeInstanceId;
	std::string name;
	Transform transform;
	// No render component as it is shared with the instance group

	Instance2(const std::string& _name = "Unnamed Instance");

};

class InstanceGroup2 : public Object {
public:
	std::vector<std::shared_ptr<Instance2>> instances;
	GLuint modelBuffer;
	InstanceGroup2(const std::string& _name = "Unnamed InstanceGroup");

	void addRenderComponent();

	void draw();
	void addInstance(std::shared_ptr<Instance2> instance);
	std::shared_ptr<Instance2> addInstance(const std::string& name);
	void removeInstance(std::shared_ptr<Instance2> instance);
	std::shared_ptr<Instance2> getInstanceById(unsigned int id);
	std::vector<std::shared_ptr<Instance2>> getInstancesByName(std::string name);

private:
	void setupModelBuffer(); // Setup model buffer for instances, reserve space

	void updateInstances();
	void resizeModelBuffer(); // Increase model buffer size by modelBufferIncrement

private:
	unsigned int modelBufferIncrement = 1000; // How much to increase model buffer size by when necessary
	unsigned int nextModelBufferIndex = 0; // Next free index in the model buffer
	unsigned int modelBufferSize = 1000; // Size of model buffer (in matrices)
	unsigned int nextInstanceId = 0; // Next free instance id
};
