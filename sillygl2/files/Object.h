
#pragma once
#include "Camera.h"
#include <vector>
#include <string>

class GameObject;

extern bool anyTransformationsThisFrame;

class GameObject {
public:
	bool transformedThisFrame = false; // If object has been moved, rotated, or scaled this frame, this is true. Useful for quickly determining if an object needs to be re-rendered.
	bool isCameraAttached = false;
	bool visible; // If object should be visibly rendered

    // Handles
    std::string name;
    int id;

    GameObject(glm::vec3 _position, std::string _name, bool _rendered);

    void setTransformedThisFrame();

    glm::vec3& getPosition();
	glm::vec3& getRotation();
	std::vector<GameObject*>& getChildren();
	std::vector<glm::vec3>& getVertices();
    std::vector<unsigned int>& getIndices();
	Camera* getAttachedCamera();

	void addVerticesIndices(std::vector<glm::vec3>& _vertices, std::vector<unsigned int>& _indices);

    void move(glm::vec3& change);
    void rotate(glm::vec3& _rotation);
    void rotateWithMatrix(glm::mat4& rotationMatrix, glm::vec3 _rotation);
    void scaleInPlace(glm::vec3& scale);
    void scale(glm::vec3& scale);

    void getAABB(glm::vec3& min, glm::vec3& max);

	void attachCamera(Camera* camera);
	void attachChild(GameObject* child);
private:
    glm::vec3 position;
    glm::vec3 rotation;
    Camera* attachedCamera;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<GameObject*> children;
};
