
#pragma once
#include "Camera.h"
#include <vector>
#include <string>

class GameObject;

extern bool anyTransformationsThisFrame;

class GameObject {
public:
	bool transformedThisFrame = false; // If object has been moved, rotated, or scaled this frame, this is true. Useful for quickly determining if an object needs to be re-rendered.
    glm::vec3 position;
    glm::vec3 rotation;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
	std::vector<GameObject*> children;
	Camera* attachedCamera;
	bool rendered; // If object should be visibly rendered

    // Handles
    std::string name;
    int id;

    GameObject(glm::vec3 _position, std::string _name, bool _rendered);

    void setTransformedThisFrame();

    void move(glm::vec3 change);
    void rotate(glm::mat4 rotationMatrix);

    void scaleInPlace(glm::vec3 scale);
    void scale(glm::vec3 scale);

    void getAABB(glm::vec3& min, glm::vec3& max);

	void attachCamera(Camera* camera);
	void attachChild(GameObject* child);
};
