#pragma once
#include "Object.h"
#include <vector>
#include <string>
#include <algorithm>

class ObjectManager {
public:
	std::vector<GameObject*> addedObjects; // Objects added in the last frame
	std::vector<GameObject*> removedObjects; // Objects removed in the last frame
    ObjectManager();

	void addObject(GameObject* object); // Adds object to list of objects
	void destroyObject(GameObject* object); // Removes object from list of objects
    bool checkCollision(GameObject* object1, GameObject* object2); 

	GameObject* getObjectByName(std::string name); // Returns the first object with the given name
	std::vector<GameObject*> getObjectListByName(std::string name); // Returns a list of objects with the given name
	std::vector<GameObject*> getObjectsByEvent(ObjectEvent e); // Returns true if any object has been moved, rotated, or scaled this frame
	void clearEvent(ObjectEvent e); // Clears the event for all objects

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
