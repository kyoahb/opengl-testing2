
#pragma once
#include "Camera.h"
#include <vector>
#include <string>
#include "Vertex.h"
class GameObject;

enum ObjectEvent {
    No_Event = 0,
	Added_Object = 1,
	Removed_Object = 2,
    Object_Transformation = 3,
	Object_Vertices_Changed = 4,
};

extern std::vector<GameObject*> transformedObjects;
extern std::vector<GameObject*> vertChangedObjects;

class GameObject {
public:
	bool isCameraAttached = false;
	bool visible; // If object should be visibly rendered

	ObjectEvent event; // Last event that happened to object

    // Handles
    std::string name;
    int id;

    GameObject(glm::vec3 _position, std::string _name, bool _rendered);

    void setObjectEvent(ObjectEvent e);

    glm::vec3& getPosition();
	glm::vec3& getRotation();
	std::vector<GameObject*>& getChildren();
	std::vector<glm::vec3>& getVertices();
    std::vector<unsigned int>& getIndices();
	Camera* getAttachedCamera();

	void addVerticesIndices(std::vector<glm::vec3>& _vertices, std::vector<unsigned int>& _indices, bool toRerender);

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
    std::vector<Vertex> vertexData;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<GameObject*> children;
};
