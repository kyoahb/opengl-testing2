#include "InstanceGroup.h"


InstanceGroup::InstanceGroup(
	const std::string& _name,
	const std::vector<Vertex>& _vertices,
	const std::vector<unsigned int>& _indices,
	const Material& _material,
	const glm::vec3& _position,
	const glm::quat& _rotation,
	const glm::vec3& _scale)
	: VertexObject(_name, _vertices, _indices, _material, _position, _rotation, _scale) {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &modelBuffer);

	glBindVertexArray(VAO);
	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);
	// Model matrix buffer
	glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
	glBufferData(GL_ARRAY_BUFFER, modelBufferSize * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW); // reserve 3000 matrices of space

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

	material.use();
	material.shader->setMat4("group", glm::mat4(1.0f));
}

Camera* InstanceGroup::getAttachedCamera() {
	return attachedCamera;
}

void InstanceGroup::attachCamera(Camera* camera) {
	attachedCamera = camera;
}

bool InstanceGroup::isCameraAttached() {
	return attachedCamera != nullptr;
}

void InstanceGroup::draw() {
	material.use();

	glBindVertexArray(VAO);
	updateInstances();
	glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0, instances.size());
	glBindVertexArray(0);
}

void InstanceGroup::setMaterial(const Material& _material) {
	material = _material;

	// New shader setup
	material.use();
	material.shader->setMat4("group", glm::mat4(1.0f));
	calcAndSendModel(); // Send actual model to shader
}

void InstanceGroup::rotateEuler(const glm::vec3& _rotation) {
	GameObject::rotateEuler(_rotation);

	if (isCameraAttached()) {
		attachedCamera->rotateEuler(_rotation);
	}
	calcAndSendModel();

}

void InstanceGroup::rotateQuat(const glm::quat& _rotation) {
	GameObject::rotateQuat(_rotation);

	if (isCameraAttached()) {
		attachedCamera->rotateQuat(_rotation);
	}
	calcAndSendModel();
}

void InstanceGroup::setRotation(const glm::quat& _rotation) {
	GameObject::setRotation(_rotation);
	calcAndSendModel();
}

void InstanceGroup::move(const glm::vec3& change) {
	GameObject::move(change);

	if (isCameraAttached()) {
		attachedCamera->move(change);
	}
	calcAndSendModel();
}

void InstanceGroup::addScale(const glm::vec3& _scale) {
	GameObject::addScale(_scale);
	calcAndSendModel();
}

void InstanceGroup::setVertex(const Vertex& vertex, unsigned int index) {
	if (index >= vertices.size()) {
		spdlog::warn("Attempted to set vertex at index {} in object {}, but index is out of bounds", index, name);
		return;
	}
	VertexObject::setVertex(vertex, index);

	// send new data to GPU
	glBindVertexArray(VAO);
	singleBuffer(GL_ARRAY_BUFFER, VBO, index * sizeof(vertex), vertex, GL_STATIC_DRAW);
}

void InstanceGroup::removeVertex(unsigned int index) {
	if (index >= vertices.size()) {
		spdlog::warn("Attempted to remove vertex at index {} in object {}, but index is out of bounds", index, name);
		return;
	}
	//VertexObject::removeVertex(index);

	// remove vertex from vertices
	vertices.erase(vertices.begin() + index);
	// remove references to vertex in indices
	indices.erase(std::remove(indices.begin(), indices.end(), index), indices.end());
	// decrement indices after removed index
	for (unsigned int& i : indices) {
		if (i > index) {
			--i;
		}
	}

	glBindVertexArray(VAO);
	batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
}

void InstanceGroup::addVertices(const std::vector<Vertex>& _vertices) {
	// Add the new vertices to current
	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
	
	glBindVertexArray(VAO);
	batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
}

void InstanceGroup::setVertices(const std::vector<Vertex>& _vertices) {
	vertices = _vertices;
	// Some thought about doing VBO / EBO buffer changes in a batch before draw but for now its okay
	glBindVertexArray(VAO);
	batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
}

void InstanceGroup::setIndices(const std::vector<unsigned int>& _indices) {
	std::vector<unsigned int> newIndices;
	unsigned int min = *std::min_element(_indices.begin(), _indices.end());
	unsigned int verticesSize = vertices.size();
	for (unsigned int index : _indices) {
		unsigned int newIndex = index - min;
		if (newIndex >= verticesSize) {
			spdlog::warn("Attempted to set index {} in object {}, but index is out of bounds", index, name);
			return;
		}
		indices.push_back(index - min );
	}

	glBindVertexArray(VAO);
	batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
}

void InstanceGroup::addIndices(const std::vector<unsigned int>& _indices) {
	// Add the new indices to current
	for (unsigned int index : _indices) {
		if (index >= vertices.size()) {
			spdlog::warn("Attempted to add index {} in object {}, but index is out of bounds", index, name);
			return;
		}
		indices.push_back(index);
	}

	glBindVertexArray(VAO);
	batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);
}

void InstanceGroup::addVerticesIndices(const std::vector<Vertex>& _vertices, const std::vector<unsigned int>& _indices) {
	// Add the new indices and vertices to current
	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
	

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

	glBindVertexArray(VAO);
	batchBuffer(GL_ARRAY_BUFFER, VBO, vertices, GL_STATIC_DRAW);
	batchBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, indices, GL_STATIC_DRAW);

}

void InstanceGroup::calcAndSendModel() {
	// Problem here:
	// In instance shader, the group model is multiplied before the instance model
	// This fixes a position issue, but it means that rotations are applied as a result of the group position, rather than the individual instance positions
	// To fix this, it would require group_rotation_and_scale_matrix * instance * group_position_matrix, but this is long

	glm::mat4 model = GameObject::calculateModelMatrix();
	material.use();
	material.shader->setMat4("group", model);
}

const std::vector<Instance*>& InstanceGroup::getInstances() {
	return instances;
}

std::vector<Instance*> InstanceGroup::getInstancesByName(std::string name) {
	std::vector<Instance*> foundInstances;
	for (Instance* instance : instances) {
		if (instance->getName() == name) {
			foundInstances.push_back(instance);
		}
	}
	return foundInstances;
}

Instance* InstanceGroup::getInstanceById(unsigned int id) {
	for (Instance* instance : instances) {
		if (instance->instanceID == id) {
			return instance;
		}
	}
	return nullptr;
}

void InstanceGroup::updateInstances() {
	// Expect VAO is bound already
	std::vector<Instance*> instancesToUpdate;

	for (Instance* instance : instances) {;
		if (instance->requestingUpdate) {
			instancesToUpdate.push_back(instance);
			instance->requestingUpdate = false;
			instance->formModelMatrix();
		}
	}

	if (instancesToUpdate.size() * 2 > instances.size()) {
		// Batch update call of ALL instances, not just modified ones
		std::vector<glm::mat4> matrices;
		for (Instance* instance : instances) {
			matrices.push_back(instance->model);
		}
		batchBuffer(GL_ARRAY_BUFFER, modelBuffer, matrices, GL_STATIC_DRAW);
	}
	else {
		// Individual update calls
		for (Instance* instance : instancesToUpdate) {
			// May be inefficient as VAO is bound and rebound several times
			//glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
			//glBufferSubData(GL_ARRAY_BUFFER, instance->modelBufferIndex * sizeof(glm::mat4), sizeof(glm::mat4), &(instance->model));
			singleBuffer(GL_ARRAY_BUFFER, modelBuffer, instance->modelBufferIndex * sizeof(glm::mat4), instance->model, GL_STATIC_DRAW);
		}
	}

}

void InstanceGroup::batchResendModelMatrices() {
	std::vector<glm::mat4> matrices;
	for (Instance* instance : instances) {
		matrices.push_back(instance->model);
	}
	glBindVertexArray(VAO);
	batchBuffer(GL_ARRAY_BUFFER, modelBuffer, matrices, GL_STATIC_DRAW);
}

void InstanceGroup::resizeModelBuffer() {
	glBindVertexArray(VAO);
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

void InstanceGroup::addInstance(Instance* instance) {
	if (instances.size() >= modelBufferSize) {
		resizeModelBuffer();
	}
	instances.push_back(instance);
	instance->modelBufferIndex = nextModelBufferIndex;
	nextModelBufferIndex++;
	instance->instanceID = nextID;
	nextID++;
	instance->requestingUpdate = true;
}

void InstanceGroup::removeInstance(Instance* instance) {
	instances.erase(std::remove(instances.begin(), instances.end(), instance), instances.end()); // Remove instance from list
	nextModelBufferIndex--; // Reduce the next available index
	for (Instance* listInstance : instances) {
		if (listInstance->modelBufferIndex > instance->modelBufferIndex) {
			listInstance->modelBufferIndex--; // Reduce the index of all instances after the removed instance
		}
	}
	batchResendModelMatrices();
}