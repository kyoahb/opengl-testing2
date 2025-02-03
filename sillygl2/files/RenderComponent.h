#pragma once
#include <vector>
#include "Useful.h"
#include "Vertex.h"
#include "Material.h"
#include "Shader_l.h"
#include "Log.h"

class RenderComponent {
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::unique_ptr<Shader> shader;
	bool visible = true;
public:
	GLuint VAO, VBO, EBO, modelBuffer;
	std::shared_ptr<Material> material;

	RenderComponent(
		const std::vector<Vertex>& _vertices = {},
		const std::vector<unsigned int>& _indices = {},
		std::shared_ptr<Material> _material = std::make_shared<Material>(),
		std::unique_ptr<Shader> _shader = nullptr);

	// getters
	const std::vector<Vertex>& getVertices() const;
	const std::vector<unsigned int>& getIndices() const;
	bool isVisible() const;
	// not read-only
	std::shared_ptr<Material> getMaterial();
	Shader* getShader();


	// setters
	void setVertices(const std::vector<Vertex>& _vertices);
	void setIndices(const std::vector<unsigned int>& _indices);
	void setMaterial(std::shared_ptr<Material> _material);
	void setVertex(const Vertex& vertex, unsigned int index);
	void setShader(std::unique_ptr<Shader> _shader);

	// other methods
	void setVisibility(bool _visible);
	void removeVertex(unsigned int index);
	void addVertices(const std::vector<Vertex>& _vertices);
	void addIndices(const std::vector<unsigned int>& _indices);
	void addVerticesIndices(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices);

	void draw() const; // Sets material, and draws single object using vertices and indices.
	void setupBuffers(); // Sets up VAO, VBO, EBO, and modelBuffer
	void setupMaterial(); // Sets up material by binding it to shader

};