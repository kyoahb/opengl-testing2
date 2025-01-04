#pragma once
#include "Object.h"
#include <vector>
#include <string>
#include <algorithm>

enum Events {
	Object_Transformation = 0,
	Added_Object = 1,
	Removed_Object = 2,
	Object_Vertices_Changed = 3,
};

class ObjectManager {
public:
    ObjectManager();

	void addObject(GameObject* object); // Adds object to list of objects
	void destroyObject(GameObject* object); // Removes object from list of objects
    bool checkCollision(GameObject* object1, GameObject* object2); 

	GameObject* getObjectByName(std::string name); // Returns the first object with the given name
	std::vector<GameObject*> getObjectListByName(std::string name); // Returns a list of objects with the given name
	bool anyTransformationsHappened(); // Returns true if any object has been moved, rotated, or scaled this frame

	void rotateObjectsR(std::vector<GameObject*>& objects, glm::vec3 rotation); // Rotates objects by the given rotation, faster because it only computes rotation once
	std::vector<GameObject*>* getObjects(); // Returns the list of objects
	void addCube(float width, float height, float depth, glm::vec3 bottomLeft, std::string name); // Adds a cube to the list of objects
	void addTriangle(glm::vec3 left, glm::vec3 right, glm::vec3 top, std::string name); // Adds a triangle to the list of objects

private:
	int nextID = 0;
    std::vector<GameObject*> objects;
    glm::vec3 storedRotation;
    glm::mat4 storedRMatrix;
};
