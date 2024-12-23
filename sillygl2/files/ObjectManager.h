#pragma once
#include "Object.h"
#include <vector>
#include <string>
#include <algorithm>

class ObjectManager {
public:
    ObjectManager();

    void addObject(GameObject* object);
    void destroyObject(GameObject* object);
    bool checkCollision(GameObject* object1, GameObject* object2);
    GameObject* getObjectByName(std::string name);
    std::vector<GameObject*> getObjectListByName(std::string name);
    void rotateObjectsR(std::vector<GameObject*> objects, glm::vec3 rotation);
    std::vector<GameObject*>* getObjects();
    void addCube(float width, float height, float depth, glm::vec3 bottomLeft, std::string name);
    void addTriangle(glm::vec3 left, glm::vec3 right, glm::vec3 top, std::string name);

private:
    std::vector<GameObject*> objects;
    glm::vec3 storedRotation;
    glm::mat4 storedRMatrix;
};
