#include "InstanceGroup.h"


InstanceGroup::InstanceGroup(Shader* _shader, std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture*> _textures, std::string _name) 
	: shader(_shader), vertices(_vertices), indices(_indices), textures(_textures), name(_name) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &modelBuffer);

	glBindVertexArray(VAO);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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

	shader->use();
	shader->setMat4("group", glm::mat4(10.0f));
}

void InstanceGroup::draw() {
	shader->use();

	glBindTexture(GL_TEXTURE_2D, textures[0]->id);
	glBindVertexArray(VAO);
	updateInstances();
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instances.size());
	glBindVertexArray(0);
}

void InstanceGroup::rotateEuler(glm::vec3 _rotation) {
	glm::vec3 filledRot = vec3Overfill(_rotation, 0.0f, 360.0f);
	rotation = glm::quat(glm::radians(filledRot)) * rotation;
	calcAndSendModel();

}

void InstanceGroup::rotateQuat(glm::quat _rotation) {
	rotation = _rotation * rotation;
	calcAndSendModel();
}

void InstanceGroup::move(glm::vec3 change) {
	position += change;
	calcAndSendModel();
}

void InstanceGroup::addScale(glm::vec3 _scale) {
	scale += _scale;
	calcAndSendModel();
}

void InstanceGroup::calcAndSendModel() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = model * glm::toMat4(rotation);
	model = glm::scale(model, scale);
	shader->use();
	shader->setMat4("group", model);
}

std::vector<Instance*> InstanceGroup::getInstancesByName(std::string name) {
	std::vector<Instance*> foundInstances;
	for (Instance* instance : instances) {
		if (instance->name == name) {
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

	glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
	for (Instance* instance : instances) {;
		if (instance->requestingUpdate) {
			instancesToUpdate.push_back(instance);
			instance->requestingUpdate = false;
			instance->formModelMatrix();
		}
	}

	if (instancesToUpdate.size() * 2 > instances.size()) {
		// Batch update call
		std::vector<glm::mat4> matrices;
		for (Instance* instance : instances) {
			matrices.push_back(instance->model);
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, matrices.size() * sizeof(glm::mat4), matrices.data());
	}
	else {
		// Individual update calls
		for (Instance* instance : instancesToUpdate) {
			glBufferSubData(GL_ARRAY_BUFFER, instance->instanceID * sizeof(glm::mat4), sizeof(glm::mat4), &(instance->model));
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, nextInstanceID * sizeof(glm::mat4));

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
	glBindVertexArray(0);
}

void InstanceGroup::addInstance(Instance* instance) {
	if (instances.size() >= modelBufferSize) {
		resizeModelBuffer();
	}
	instances.push_back(instance);
	instance->instanceID = nextInstanceID;
	nextInstanceID++;
	instance->requestingUpdate = true;
}