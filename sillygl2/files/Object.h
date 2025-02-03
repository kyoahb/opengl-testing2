#pragma once
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Camera.h"
#include "Buffer.h"
class Object {
protected:
	unsigned int id;
	std::string name;
	std::unique_ptr<RenderComponent> renderComponent; // Default: nullptr
	std::shared_ptr<Camera> camera; // Default: nullptr
	TransformComponent transform;

	std::vector<std::shared_ptr<Object>> children;

	void update(); // Resends group matrix if transform has been updated
public:
	Object(const std::string& _name = "Unnamed Object");

	// getters
	const std::string& getName() const;
	unsigned int getId() const;
	std::shared_ptr<Camera> getCamera();
	RenderComponent* getRenderComponent(); // dangerous
	// setters
	void setName(const std::string& _name);
	void setId(unsigned int _id);

	// methods
	void addChild(std::shared_ptr<Object> child);
	void attachCamera(std::shared_ptr<Camera> camera);
	void attachRenderComponent(std::unique_ptr<RenderComponent> _renderComponent);
	void createRenderComponent();

	bool hasRenderComponent() const;
	bool hasCamera() const;

	void draw(); // Update object if necessary, then draw if render component exists.

	// transform overrides (necessary to propagate effects to children)
	// getters
	const glm::vec3& getPosition() const;
	const glm::quat& getQuatRotation() const;
	const glm::vec3& getEulerRotation() const;
	const glm::vec3& getScale() const;
	const glm::mat4& getModelMatrix() const;
	std::vector<std::shared_ptr<Object>>& getChildren();

	// setters
	void setPosition(const glm::vec3& _position);
	void setQuatRotation(const glm::quat& _rotation);
	void setEulerRotation(const glm::vec3& _rotation);
	void setScale(const glm::vec3& _scale);

	// relative transformations
	void move(const glm::vec3& change);
	void rotateEuler(const glm::vec3& _rotation);
	void rotateQuat(const glm::quat& _rotation);
	void addScale(const glm::vec3& _scale);
};