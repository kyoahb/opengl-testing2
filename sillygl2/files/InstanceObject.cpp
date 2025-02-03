#include "InstanceObject.h"

/* Instance */
InstanceObject::InstanceObject(const std::string& _name) :
	Object(_name), modelBufferIndex(0) {
}

void InstanceObject::update(GLuint modelBuffer) {
	if (transform.transformModified) {
		// Reset flag
		transform.transformModified = false;

		// Recalculate model matrix
		transform.calculateModelMatrix();

		// Buffer single piece of data over
		singleBuffer(GL_ARRAY_BUFFER, modelBuffer, modelBufferIndex * sizeof(glm::mat4), transform.getModelMatrix(), GL_STATIC_DRAW);
	}
}

const glm::mat4& InstanceObject::updateGetModel() {
	if (transform.transformModified) {
		// Reset flag
		transform.transformModified = false;

		// Recalculate model matrix
		transform.calculateModelMatrix();
	}
	return transform.getModelMatrix();
}


/* Overrides */
void InstanceObject::draw() {
	// Do nothing
	spdlog::error("Attempted to draw instance object {}, but instances are not individually renderable", name);
}
void InstanceObject::update() {
	// Do nothing
	spdlog::error("Attempted to update (without parameters) instance object {}, but instances are not individually renderable", name);
}
void InstanceObject::attachRenderComponent(std::unique_ptr<RenderComponent> _renderComponent) {
	// Do nothing
	spdlog::error("Attempted to attach render component to instance object {}, but instances are not individually renderable", name);
}
void InstanceObject::createRenderComponent() {
	// Do nothing
	spdlog::error("Attempted to create render component for instance object {}, but instances are not individually renderable", name);
}
RenderComponent* InstanceObject::getRenderComponent() {
	spdlog::error("Attempted to get render component from instance object {}, but instances are not individually renderable", name);
	return nullptr;
}
