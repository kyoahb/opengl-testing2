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
	glBindVertexArray(VAO);
	//batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
	VBO.bufferData(vertices.size(), vertices, GL_STATIC_DRAW);
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

	glBindVertexArray(VAO);
	//batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
	EBO.bufferData(indices.size(), indices, GL_STATIC_DRAW);
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
	glBindVertexArray(VAO);
	singleBuffer(GL_ARRAY_BUFFER, VAO, index * sizeof(Vertex), vertex, GL_STATIC_DRAW);
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
	glBindVertexArray(VAO);
	//batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
	VBO.bufferData(vertices.size(), vertices, GL_STATIC_DRAW);
}

void RenderComponent::addVertices(const std::vector<Vertex>& _vertices) {
	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());

	// send data over
	glBindVertexArray(VAO);
	//batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
	VBO.bufferData(vertices.size(), vertices, GL_STATIC_DRAW);
}

void RenderComponent::addIndices(const std::vector<unsigned int>& _indices) {
	for (unsigned int index : _indices) {
		if (index >= vertices.size()) {
			spdlog::warn("Attempted to add index {} in object, but index is out of bounds", index);
			return;
		}
		indices.push_back(index);
	}

	glBindVertexArray(VAO);
	//batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
	EBO.bufferData(indices.size(), indices, GL_STATIC_DRAW);
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

	glBindVertexArray(VAO);
	//batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
	//batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
	VBO.bufferData(vertices.size(), vertices, GL_STATIC_DRAW);
	EBO.bufferData(indices.size(), indices, GL_STATIC_DRAW);

}

void RenderComponent::draw() const {
	if (!visible) {
		return;
	}
	shader->use();
	material->use();

	glBindVertexArray(VAO);

	modelBuffer.bind();

	// testing
	// get data from buffers and print out
	glm::mat4 modelMatrix;
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &modelMatrix);
	mat4Print(modelMatrix);

	for (unsigned int i = 0; i < vertices.size(); i++) {
		glDrawArrays(GL_POINTS, i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO.bufferId);

	// Map the buffer to get a pointer to its data
	Vertex* vertexData = static_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));
	if (vertexData) {
		// Print the vertex data
		for (size_t i = 0; i < vertices.size(); ++i) {
			const Vertex& vertex = vertexData[i];
			spdlog::info("Vertex {}: Position({}, {}, {}), Normal({}, {}, {}), TexCoords({}, {})",
				i,
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.normal.x, vertex.normal.y, vertex.normal.z,
				vertex.texCoords.x, vertex.texCoords.y);
		}
	}

	EBO.bind();
	// Map the buffer to get a pointer to its data
	unsigned int* intsData = static_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY));
	if (intsData) {
		// Print the vertex data
		for (size_t i = 0; i < indices.size(); ++i) {
			const unsigned int& index = intsData[i];
			spdlog::info("{} Index: {}", i, index);
		}
	}



	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderComponent::setupBuffers() {
	// VAO setup
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
	VBO.bufferData(vertices.size(), vertices, GL_STATIC_DRAW);

	// Split VBO into 3 parts: Position, Normal, TexCoords
	VBO.bind();
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); // Normal
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords)); // TexCoords


	// Indices setup
	//batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
	EBO.bufferData(indices.size(), indices, GL_STATIC_DRAW);


	/*
	The code below is for the model matrix buffer.
	This remains UNUSED for an object, but is used for instance groups.
	The point of having them setup here is that it makes it so objects can have the same shaders as instance groups.
	*/
	modelBuffer.bind();
	glm::mat4 identity = glm::mat4(1.0f);
	modelBuffer.bufferData(sizeof(glm::mat4), { identity }, GL_STATIC_DRAW); // Send in identity matrix

	// setup matrix as location 3,4,5,6
	GLsizei vec4Size = (GLsizei)sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void RenderComponent::setupMaterial() {
	material->apply(shader.get());
}
