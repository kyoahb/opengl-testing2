#include "GameObject.h"

/* Transform */
Transform::Transform(
	const glm::vec3& _position,
	const glm::quat& _rotation,
	const glm::vec3& _scale)
	: position(_position), scale(_scale), rotation(_rotation)
{
	if (scale == glm::vec3(0.0f)) {
		spdlog::warn("Scale of object transform is 0.0f, setting to 1.0f");
		scale = glm::vec3(1.0f);
	}

	calculateModelMatrix();
}

/* Getters */

const glm::vec3& Transform::getPosition() const {
	return position;
}

const glm::quat& Transform::getQuatRotation() const {
	return rotation;
}

const glm::vec3& Transform::getEulerRotation() const {
	return glm::degrees(glm::eulerAngles(rotation));
}

const glm::vec3& Transform::getScale() const {
	return scale;
}

const glm::mat4& Transform::getModelMatrix() const {
	return modelMatrix;
}

/* Setters */

void Transform::setPosition(const glm::vec3& _position) {
	position = _position;
	transformModified = true;
}

void Transform::setQuatRotation(const glm::quat& _rotation) {
	rotation = _rotation;
	transformModified = true;
}

void Transform::setEulerRotation(const glm::vec3& _rotation) {
	rotation = glm::quat(glm::radians(_rotation));
	transformModified = true;
}

void Transform::setScale(const glm::vec3& _scale) {
	scale = _scale;
	transformModified = true;
}

/* Relative transformations */

void Transform::move(const glm::vec3& change) {
	position += change;
	transformModified = true;
}

void Transform::rotateEuler(const glm::vec3& _rotation) {
	glm::vec3 filledRot = vec3Overfill(_rotation, -180.0f, 180.0f);
	rotation = glm::quat(glm::radians(filledRot)) * rotation;
	rotation = glm::normalize(rotation);
	transformModified = true;
}

void Transform::rotateQuat(const glm::quat& _rotation) {
	rotation = _rotation * rotation;
	rotation = glm::normalize(rotation);
	transformModified = true;
}

void Transform::addScale(const glm::vec3& _scale) {
	scale += _scale;
	transformModified = true;
}

/* Calculate model matrix */

void Transform::update() {
	if (transformModified) {
		calculateModelMatrix();
		transformModified = false;
	}
}

void Transform::calculateModelMatrix() {
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix *= glm::toMat4(rotation);
	modelMatrix = glm::scale(modelMatrix, scale);
}

/* RenderComponent */

RenderComponent::RenderComponent(
	const std::vector<Vertex>& _vertices,
	const std::vector<unsigned int>& _indices,
	std::shared_ptr<Material> _material,
	std::unique_ptr<Shader> _shader) : vertices(_vertices), indices(_indices), material(_material)
{
	shader = std::move(_shader);
	setupBuffers();
}

/* Getters */
const std::vector<Vertex>& RenderComponent::getVertices() const { return vertices; };
const std::vector<unsigned int>& RenderComponent::getIndices() const { return indices; };
const std::shared_ptr<Material>& RenderComponent::getMaterial() const { return material; };
Shader* RenderComponent::getShader() const { return shader.get(); };

/* Setters */
void RenderComponent::setVertices(const std::vector<Vertex>& _vertices) {
	vertices = _vertices;
	
	// send data over
	glBindVertexArray(VAO);
	batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
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
	batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
}

void RenderComponent::setMaterial(std::shared_ptr<Material> _material) {
	material = std::move(_material);
	material->apply(shader.get());
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
void RenderComponent::removeVertex(unsigned int index) {
	if (index >= vertices.size()) {
		spdlog::warn("Attempted to remove vertex at index {} in object, but index is out of bounds", index);
		return;
	}
	vertices.erase(vertices.begin() + index);

	// send data over
	glBindVertexArray(VAO);
	batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
}

void RenderComponent::addVertices(const std::vector<Vertex>& _vertices) {
	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());

	// send data over
	glBindVertexArray(VAO);
	batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
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
	batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
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
	batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
	batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);

}

void RenderComponent::draw() const {
	shader->use();
	material->use();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderComponent::setupBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// TexCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void RenderComponent::setupMaterial() {
	material->apply(shader.get());
}

/* Object */
Object::Object(const std::string& _name) : name(_name), transform(Transform()) {
	//renderComponent = std::make_unique<RenderComponent>(std::make_shared<Object>(this));
};

void Object::addRenderComponent() {
	renderComponent = std::make_unique<RenderComponent>();
}

// Draw object, update if necessary
void Object::draw() {

	if (renderComponent) {
		// Update transform
		update();

		// Draw
		renderComponent->draw();
	}
}

// If transform is modified, recalculate and update model matrix
void Object::update() {
	if (transform.transformModified) {
		// Recalculate model matrix, send back over
		transform.calculateModelMatrix();
		renderComponent->getShader()->setMat4("group", transform.getModelMatrix());
	}
}

/* Instance */
Instance2::Instance2(const std::string& _name) : 
	name(_name), transform(Transform()) {

}

/* InstanceGroup */
InstanceGroup2::InstanceGroup2(const std::string& _name) : Object(_name) {
	//renderComponent = std::make_unique<RenderComponent>();
	transform.transformModified = true;
}

void InstanceGroup2::addRenderComponent() {
	renderComponent = std::make_unique<RenderComponent>();
	renderComponent->setupMaterial();
	setupModelBuffer();
}

void InstanceGroup2::setupModelBuffer() {

	// Ensure render component exists
	if (renderComponent == nullptr) {
		spdlog::error("Attempted to setup model buffer for instance group {}, but no render component is set", name);
		return;
	}

	// Create model buffer
	glGenBuffers(1, &modelBuffer);
	// Bind vao
	glBindVertexArray(renderComponent->VAO);
	// Model matrix buffer
	glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
	glBufferData(GL_ARRAY_BUFFER, modelBufferSize * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW); // reserve matrixBufferSize matrices of space

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
}

void InstanceGroup2::draw() {
	// Ensure render component exists
	if (renderComponent == nullptr) {
		spdlog::error("Attempted to draw instance group {}, but no render component is set", name);
		return;
	}
	// Update transform
	update();
	
	// Collect updated instances
	updateInstances();

	// Draw
	glBindVertexArray(renderComponent->VAO);

	renderComponent->getShader()->use();
	renderComponent->getMaterial()->use();

	if (instances.size() > 0) {
		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)renderComponent->getIndices().size(), GL_UNSIGNED_INT, 0, instances.size());
	}
	
}

void InstanceGroup2::updateInstances() {
	// TODO: implement a check where if amount of modified instances is bigger than some relative amount, do a batch call instead of
	// one-by-one
	
	for (std::shared_ptr<Instance2> instance : instances) {
		if (instance->transform.transformModified) {
			// Reset flag
			instance->transform.transformModified = false;

			// Recalculate model matrix
			instance->transform.calculateModelMatrix();

			// Buffer single piece of data over
			singleBuffer(GL_ARRAY_BUFFER, modelBuffer, instance->modelBufferIndex * sizeof(glm::mat4), instance->transform.getModelMatrix(), GL_STATIC_DRAW);
		}
	}
	/*
	std::vector<glm::mat4> matrices;
	for (std::shared_ptr<Instance2> instance : instances) {
		instance->transform.calculateModelMatrix();
		matrices.push_back(instance->transform.getModelMatrix());
	}
	batchBuffer(GL_ARRAY_BUFFER, modelBuffer, matrices, GL_STATIC_DRAW);*/

}

void InstanceGroup2::addInstance(std::shared_ptr<Instance2> instance) {
	if (instances.size() >= modelBufferSize) {
		resizeModelBuffer();
	}
	instances.push_back(instance);
	instance->modelBufferIndex = nextModelBufferIndex;
	nextModelBufferIndex++;
	
	instance->relativeInstanceId = nextInstanceId;
	nextInstanceId++;
	
	// Workaround that forces instance to appear on next immediate frame
	instance->transform.transformModified = true; 
}

std::shared_ptr<Instance2> InstanceGroup2::addInstance(const std::string& name) {
	std::shared_ptr<Instance2> instance = std::make_shared<Instance2>(name);
	addInstance(instance);
	return instance;
}

void InstanceGroup2::resizeModelBuffer() {
	// Ensure render component exists
	if (renderComponent == nullptr) {
		spdlog::error("Attempted to resize model buffer for instance group {}, but no render component is set", name);
		return;
	}
	
	glBindVertexArray(renderComponent->VAO);
	modelBufferSize += modelBufferIncrement;

	// Generate new buffer
	unsigned int newModelBuffer;
	glGenBuffers(1, &newModelBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, newModelBuffer);
	glBufferData(GL_ARRAY_BUFFER, modelBufferSize * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW); // Reserve new data space

	// Copy data from the old buffer to the new buffer
	glBindBuffer(GL_COPY_READ_BUFFER, modelBuffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, newModelBuffer);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, nextModelBufferIndex * sizeof(glm::mat4));

	// Update VAO with new buffer
	glBindBuffer(GL_ARRAY_BUFFER, newModelBuffer);
	GLsizei vec4Size = (GLsizei)sizeof(glm::vec4);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	// Delete the old buffer and update the modelBuffer to the new buffer
	glDeleteBuffers(1, &modelBuffer);
	modelBuffer = newModelBuffer;

	// Unbind the buffers
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}