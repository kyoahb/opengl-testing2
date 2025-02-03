#pragma once
#include "Object.h"

class InstanceObject : public Object {
public:
	unsigned int modelBufferIndex;

	InstanceObject(const std::string& _name = "Unnamed Instance");

	void update(GLuint modelBuffer); // Update model matrix + buffer over to given modelBuffer if necessary.

	const glm::mat4& updateGetModel(); // Update model matrix and return it


	// Overrides: replacing illegal methods (InstanceObjects are not individually renderable)
	void draw();
	void update();
	void attachRenderComponent(std::unique_ptr<RenderComponent> _renderComponent);
	void createRenderComponent();
	RenderComponent* getRenderComponent(); // dangerous
};