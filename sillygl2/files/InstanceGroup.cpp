#include "InstanceGroup.h"

/* InstanceGroup */
InstanceGroup::InstanceGroup(const std::string& _name) : Object(_name) {
	createRenderComponent();
	setupModelBuffer();

	transform.transformModified = true;
}

void InstanceGroup::setupModelBuffer() {

	// Ensure render component exists
	if (renderComponent == nullptr) {
		spdlog::error("Attempted to setup model buffer for instance group {}, but no render component is set", name);
		return;
	}

	// Bind vao
	glBindVertexArray(renderComponent->VAO);
	// Model matrix buffer
	renderComponent->modelBuffer.bind();
	glBufferData(GL_ARRAY_BUFFER, modelBufferSize * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW); // reserve matrixBufferSize matrices of space
}

void InstanceGroup::draw() {
	// Ensure render component exists
	if (renderComponent == nullptr) {
		spdlog::error("Attempted to draw instance group {}, but no render component is set", name);
		return;
	}
	// Update self's transform
	update();

	// Collect updated instances
	updateInstances();

	// Draw
	glBindVertexArray(renderComponent->VAO);

	renderComponent->getShader()->use();
	renderComponent->getMaterial()->use();

	renderComponent->VBO.bind();
	renderComponent->EBO.bind();
	renderComponent->modelBuffer.bind();

	if (instances.size() > 0) {
		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)renderComponent->getIndices().size(), GL_UNSIGNED_INT, 0, instances.size());
	}

}

void InstanceGroup::removeInstance(std::shared_ptr<InstanceObject> instance) {
	instances.erase(std::remove(instances.begin(), instances.end(), instance), instances.end()); // Remove instance from list
	nextModelBufferIndex--; // Reduce the next available index
	for (std::shared_ptr<InstanceObject> listInstance : instances) {
		if (listInstance->modelBufferIndex > instance->modelBufferIndex) {
			listInstance->modelBufferIndex--; // Reduce the index of all instances after the removed instance
		}
	}
	batchResendModels();
}

std::vector<std::shared_ptr<InstanceObject>>* InstanceGroup::getInstances() {
	return &instances;
}

std::shared_ptr<InstanceObject> InstanceGroup::getInstanceById(unsigned int id) {
	for (std::shared_ptr<InstanceObject> instance : instances) {
		if (instance->getId() == id) {
			return instance;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<InstanceObject>> InstanceGroup::getInstancesByName(std::string name) {
	std::vector<std::shared_ptr<InstanceObject>> foundInstances;
	for (std::shared_ptr<InstanceObject> instance : instances) {
		if (instance->getName() == name) {
			foundInstances.push_back(instance);
		}
	}
	return foundInstances;
}

void InstanceGroup::updateInstances() {
	// TODO: implement a check where if amount of modified instances is bigger than some relative amount, do a batch call instead of
	// one-by-one

	for (std::shared_ptr<InstanceObject> instance : instances) {
		instance->update(renderComponent->modelBuffer.bufferId);
	}
	/*
	std::vector<glm::mat4> matrices;
	for (std::shared_ptr<Instance2> instance : instances) {
		instance->transform.calculateModelMatrix();
		matrices.push_back(instance->transform.getModelMatrix());
	}
	batchBuffer(GL_ARRAY_BUFFER, modelBuffer, matrices, GL_STATIC_DRAW);*/

}

void InstanceGroup::batchResendModels() {
	std::vector<glm::mat4> matrices;
	for (std::shared_ptr<InstanceObject> instance : instances) {
		matrices.push_back(instance->updateGetModel());
	}
	
	// Resend
	//batchBuffer(GL_ARRAY_BUFFER, renderComponent->modelBuffer, matrices, GL_STATIC_DRAW);
	renderComponent->modelBuffer.bufferData(matrices.size(), matrices, GL_STATIC_DRAW);
}

void InstanceGroup::addInstance(std::shared_ptr<InstanceObject> instance) {
	if (instances.size() >= modelBufferSize) {
		resizeModelBuffer();
	}
	instances.push_back(instance);
	instance->modelBufferIndex = nextModelBufferIndex;
	nextModelBufferIndex++;

	instance->setId(nextInstanceId++);

	// Workaround that forces instance to appear on next immediate frame
	instance->move(glm::vec3(0.0f));
}

std::shared_ptr<InstanceObject> InstanceGroup::addInstance(const std::string& name) {
	std::shared_ptr<InstanceObject> instance = std::make_shared<InstanceObject>(name);
	addInstance(instance);
	return instance;
}

void InstanceGroup::resizeModelBuffer() {
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
	//glBindBuffer(GL_COPY_READ_BUFFER, renderComponent->modelBuffer);
	renderComponent->modelBuffer.bind();
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
	//glDeleteBuffers(1, &renderComponent->modelBuffer);
	renderComponent->modelBuffer.bufferId = newModelBuffer;

	// Unbind the buffers
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}