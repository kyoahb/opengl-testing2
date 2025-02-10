#include "RenderComponent.h"

/* RenderComponent */

RenderComponent::RenderComponent(
	const std::vector<Vertex>& _vertices,
	const std::vector<unsigned int>& _indices,
	std::shared_ptr<Material> _material,
	std::unique_ptr<Shader> _shader) : vertices(_vertices), indices(_indices), material(_material)
{
	if (shader != nullptr) {
		shader = std::move(_shader);
	}
	else {
		shader = std::make_unique<Shader>("shaders/material_shader.vert", "shaders/material_shader.frag");
	}
	setupMaterial();
	setupBuffers();
}

/* Getters */
const std::vector<Vertex>& RenderComponent::getVertices() const { return vertices; };
const std::vector<unsigned int>& RenderComponent::getIndices() const { return indices; };
bool RenderComponent::isVisible() const { return visible; };
std::shared_ptr<Material> RenderComponent::getMaterial() { return material; };
Shader* RenderComponent::getShader() { return shader.get(); };

/* Setters */
void RenderComponent::setVertices(const std::vector<Vertex>& _vertices) {
	vertices = _vertices;

	// send data over
	VAO.bind();
	VBO.bufferData(vertices, GL_STATIC_DRAW);
}

void RenderComponent::setIndices(const std::vector<unsigned int>& _indices) {
	std::vector<unsigned int> newIndices;
	unsigned int min = *std::min_element(_indices.begin(), _indices.end());
	unsigned int verticesSize = vertices.size();
	for (unsigned int index : _indices) {
		unsigned int newIndex = index - min;
		if (newIndex >= verticesSize) {
			spdlog::warn("Attempted to set index {} in object, but index is out of bounds", index);
			return;
		}
		indices.push_back(index - min);
	}

	VAO.bind();
	//batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
	EBO.bufferData(indices, GL_STATIC_DRAW);
}

void RenderComponent::setMaterial(std::shared_ptr<Material> _material) {
	material = _material;
	setupMaterial();
}

void RenderComponent::setVertex(const Vertex& vertex, unsigned int index) {
	if (index >= vertices.size()) {
		spdlog::warn("Attempted to set vertex at index {} in object, but index is out of bounds", index);
		return;
	}
	vertices[index] = vertex;

	// send data over
	VAO.bind();
	VBO.bufferSubDataIndex(index, &vertex); // TEST THIS
}

void RenderComponent::setShader(std::unique_ptr<Shader> _shader) {
	shader = std::move(_shader);
}

/* Other methods */
void RenderComponent::setVisibility(bool _visible) {
	visible = _visible;
}


void RenderComponent::removeVertex(unsigned int index) {
	if (index >= vertices.size()) {
		spdlog::warn("Attempted to remove vertex at index {} in object, but index is out of bounds", index);
		return;
	}
	vertices.erase(vertices.begin() + index);

	// send data over
	VAO.bind();
	VBO.bufferData(vertices, GL_STATIC_DRAW);
}

void RenderComponent::addVertices(const std::vector<Vertex>& _vertices) {
	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());

	// send data over
	
	VAO.bind();
	VBO.bufferData(vertices, GL_STATIC_DRAW);
}

void RenderComponent::addIndices(const std::vector<unsigned int>& _indices) {
	for (unsigned int index : _indices) {
		if (index >= vertices.size()) {
			spdlog::warn("Attempted to add index {} in object, but index is out of bounds", index);
			return;
		}
		indices.push_back(index);
	}

	VAO.bind();
	EBO.bufferData(indices, GL_STATIC_DRAW);
}

void RenderComponent::addVerticesIndices(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices) {


	/* Ensure all indices begin with the smallest index at 0, linking to the 1st vertex*/

	// Find smallest index in list of _indices
	unsigned int min = *std::min_element(_indices.begin(), _indices.end());

	// Adjust all indices to be relative to the smallest index, but also relative to largest size of current indices
	// For example: suppose _indices is {3, 4, 5}, current indices is {1, 2, 3}
	// First: {3, 4, 5} (min = 3) becomes -> {0, 1, 2}
	// Second: {0, 1, 2} (vertices.size() aka max(indices) = 3) becomes -> {4, 5, 6}
	// Third: Push back new index to indices list

	unsigned int verticesSize = vertices.size(); // Aka the max(indices) supposing all indices are correct
	for (unsigned int index : _indices) {
		indices.push_back(index - min + verticesSize);
	}

	// Add new vertices
	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());

	VAO.bind();
	VBO.bufferData(vertices, GL_STATIC_DRAW);
	EBO.bufferData(indices, GL_STATIC_DRAW);

}

void RenderComponent::draw() const {
	if (!visible) {
		return;
	}
	shader->use();
	material->use();
	
	VAO.bind();
	VAO.bindBuffers();

	modelBuffer.bind();
	VBO.bind();
	EBO.bind();

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderComponent::setupBuffers() {
	VAO.bind();
	VAO.addBuffer(VBO);
	VAO.addBuffer(EBO);
	VAO.addBuffer(modelBuffer);

	VBO.bufferData(vertices, GL_STATIC_DRAW);

	// Split VBO into 3 parts: Position, Normal, TexCoords
	VBO.bind();
	// Position

	unsigned int positionSize = (unsigned int)sizeof(Vertex::position);
	unsigned int normalSize = (unsigned int)sizeof(Vertex::normal);
	unsigned int texCoordsSize = (unsigned int)sizeof(Vertex::texCoords);

	unsigned int normalOffset = (unsigned int)offsetof(Vertex, normal);
	unsigned int texCoordsOffset = (unsigned int)offsetof(Vertex, texCoords);
	VAO.addAttribPointers({ 0, normalOffset, texCoordsOffset }, sizeof(Vertex), GL_FLOAT, GL_FALSE);

	// Indices setup
	EBO.bufferData(indices, GL_STATIC_DRAW);


	/*
	The code below is for the model matrix buffer.
	This remains UNUSED for an object, but is used for instance groups.
	The point of having them setup here is that it makes it so objects can have the same shaders as instance groups.
	*/
	modelBuffer.bind();
	glm::mat4 identity = glm::mat4(1.0f);
	modelBuffer.bufferData({ identity }, GL_STATIC_DRAW); // Send in identity matrix

	// setup matrix as location 3,4,5,6
	unsigned int vec4Size = (unsigned int)sizeof(glm::vec4);

	VAO.addAttribPointers({ 0, vec4Size, 2*vec4Size, 3*vec4Size }, 4 * vec4Size, GL_FLOAT, GL_FALSE);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void RenderComponent::setupMaterial() {
	material->apply(shader.get());
}
