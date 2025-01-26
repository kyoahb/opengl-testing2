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
	std::weak_ptr<Object> object;
	Transform(
		std::weak_ptr<Object> _object,
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
	GLuint VAO, VBO, EBO, modelBuffer;
	unsigned int modelBufferSize; // Allow for greater size for insatancegroups 
	std::unique_ptr<Shader> shader;

public:
	std::weak_ptr<Object> object;
	std::shared_ptr<Material> material;

	RenderComponent(
		std::weak_ptr<Object> _object,
		const std::vector<Vertex>& _vertices = {},
		const std::vector<unsigned int>& _indices = {},
		std::shared_ptr<Material> _material = std::make_shared<Material>(),
		std::unique_ptr<Shader> _shader = std::make_unique<Shader>("shaders/material_shader.vert", "shaders/material_shader.frag"),
		unsigned int _modelBufferSize = 1);

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

	void draw() const;
	void setupBuffers();
	void setupMaterial();

};

class PhysicsComponent {

};

class Object : public std::enable_shared_from_this<Object> {
public:
	unsigned int id;
	std::string name;

	Transform transform;
	std::unique_ptr<RenderComponent> renderComponent;
	//std::unique_ptr<PhysicsComponent> physicsComponent;

	Object(const std::string& _name = "Unnamed Object");


	void addRenderComponent();

	void draw();
	void update();

};
