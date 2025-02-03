#include "Object.h"
/* Object */
Object::Object(const std::string& _name) : name(_name), transform(TransformComponent()) {
};

// getters
const std::string& Object::getName() const {
	return name;
};

unsigned int Object::getId() const
{
	return id;
};

std::shared_ptr<Camera> Object::getCamera() {
	return camera;
};

RenderComponent* Object::getRenderComponent() {
	if (renderComponent == nullptr) {
		// This is a workaround to reduce nullptr exceptions. The render component is made but remains invisible.
		createRenderComponent();
		renderComponent->setVisibility(false);
	}
	return renderComponent.get();
};

// setters
void Object::setName(const std::string& _name) {
	name = _name;
};


void Object::setId(unsigned int _id)
{
	id = _id;
};

// methods
void Object::addChild(std::shared_ptr<Object> child) {
	children.push_back(child);
}

void Object::attachCamera(std::shared_ptr<Camera> _camera) {
	camera = _camera;
}

void Object::attachRenderComponent(std::unique_ptr<RenderComponent> _renderComponent) {
	renderComponent = std::move(_renderComponent);
}

void Object::createRenderComponent() {
	if (renderComponent == nullptr) {
		renderComponent = std::make_unique<RenderComponent>();
	}
	else {
		renderComponent->setVisibility(true);
	}
}

bool Object::hasRenderComponent() const {
	// Returns true, even if renderComponent is invisible
	return renderComponent != nullptr;
}

bool Object::hasCamera() const {
	return camera != nullptr;
}

void Object::draw() {
	// Update transform
	update();

	// Draw
	if (hasRenderComponent()) {
		renderComponent->draw();
		
	}
}

// If transform is modified, recalculate model matrix
void Object::update() {
	if (transform.transformModified) {
		// Recalculate model matrix
		transform.calculateModelMatrix();

		if (hasRenderComponent()) {
			// Send matrix as "group" to shader
			renderComponent->getShader()->setMat4("group", transform.getModelMatrix());
		}
	}
}

/* Transform overrides */
// getters
const glm::vec3& Object::getPosition() const {
	return transform.getPosition();
}

const glm::quat& Object::getQuatRotation() const {
	return transform.getQuatRotation();
}

const glm::vec3& Object::getEulerRotation() const {
	return transform.getEulerRotation();
}

const glm::vec3& Object::getScale() const {
	return transform.getScale();
}

const glm::mat4& Object::getModelMatrix() const {
	return transform.getModelMatrix();
}

std::vector<std::shared_ptr<Object>>& Object::getChildren() {
	return children;
}

// setters
void Object::setPosition(const glm::vec3& _position) {
	transform.setPosition(_position);
}

void Object::setQuatRotation(const glm::quat& _rotation) {
	transform.setQuatRotation(_rotation);
}

void Object::setEulerRotation(const glm::vec3& _rotation) {
	transform.setEulerRotation(_rotation);
}

void Object::setScale(const glm::vec3& _scale) {
	transform.setScale(_scale);
}

// relative transformations (with child object propagation)
void Object::move(const glm::vec3& change) {
	transform.move(change);
	for (std::shared_ptr<Object> child : children) {
		child->move(change);
	}
	if (hasCamera()) {
		camera->move(change);
	}
}

void Object::rotateEuler(const glm::vec3& _rotation) {
	transform.rotateEuler(_rotation);
	for (std::shared_ptr<Object> child : children) {
		child->rotateEuler(_rotation);
	}
	if (hasCamera()) {
		camera->rotateEuler(_rotation);
	}
}

void Object::rotateQuat(const glm::quat& _rotation) {
	transform.rotateQuat(_rotation);
	for (std::shared_ptr<Object> child : children) {
		child->rotateQuat(_rotation);
	}
	if (hasCamera()) {
		camera->rotateQuat(_rotation);
	}
}

void Object::addScale(const glm::vec3& _scale) {
	transform.addScale(_scale);
	for (std::shared_ptr<Object> child : children) {
		child->addScale(_scale);
	}
}


