#include "ObjectManager.h"

ObjectManager::ObjectManager() : storedRMatrix(glm::mat4(1.0f)), storedRotation(false) {
    GameObject* origin = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), "origin");
    origin->vertices = { glm::vec3(0.0f, 0.0f, 0.0f) };
    origin->indices = { 0 };
    objects.push_back(origin);
}

void ObjectManager::addObject(GameObject* object) {
    object->id = (int)objects.size();
    objects.push_back(object);
}

void ObjectManager::destroyObject(GameObject* object) {
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

bool ObjectManager::checkCollision(GameObject* object1, GameObject* object2) {
    glm::vec3 min1, max1, min2, max2;
    object1->getAABB(min1, max1);
    object2->getAABB(min2, max2);

    bool collisionX = (min1.x <= max2.x && max1.x >= min2.x);
    bool collisionY = (min1.y <= max2.y && max1.y >= min2.y);
    bool collisionZ = (min1.z <= max2.z && max1.z >= min2.z);

    return collisionX && collisionY && collisionZ;
}

GameObject* ObjectManager::getObjectByName(std::string name) {
    for (auto& object : objects) {
        if (object->name == name) {
            return object;
        }
    }
    return nullptr;
}

std::vector<GameObject*> ObjectManager::getObjectListByName(std::string name) {
    std::vector<GameObject*> objectsWithName = {};
    for (auto& object : objects) {
        if (object->name == name) {
            objectsWithName.push_back(object);
        }
    }
    return objectsWithName;
}

void ObjectManager::rotateObjectsR(std::vector<GameObject*> objects, glm::vec3 rotation) {
    if (rotation != storedRotation) {
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        storedRMatrix = rotationZ * rotationY * rotationX;
        storedRotation = rotation;
    }

    for (auto& object : objects) {
        object->rotate(storedRMatrix);
    }
}

std::vector<GameObject*>* ObjectManager::getObjects() {
    return &objects;
}

void ObjectManager::addCube(float width, float height, float depth, glm::vec3 bottomLeft, std::string name) {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 v0 = bottomLeft;
    glm::vec3 v1 = bottomLeft + glm::vec3(width, 0.0f, 0.0f);
    glm::vec3 v2 = bottomLeft + glm::vec3(width, height, 0.0f);
    glm::vec3 v3 = bottomLeft + glm::vec3(0.0f, height, 0.0f);
    glm::vec3 v4 = bottomLeft + glm::vec3(0.0f, 0.0f, depth);
    glm::vec3 v5 = bottomLeft + glm::vec3(width, 0.0f, depth);
    glm::vec3 v6 = bottomLeft + glm::vec3(width, height, depth);
    glm::vec3 v7 = bottomLeft + glm::vec3(0.0f, height, depth);

    vertices.insert(vertices.end(), { v0, v1, v2, v3, v4, v5, v6, v7 });

    indices.insert(indices.end(), {
        0, 1, 2, 2, 3, 0, // Front face
        4, 5, 6, 6, 7, 4, // Back face
        0, 3, 7, 7, 4, 0, // Left face
        1, 2, 6, 6, 5, 1, // Right face
        3, 2, 6, 6, 7, 3, // Top face
        0, 1, 5, 5, 4, 0  // Bottom face
        });

    glm::vec3 center = bottomLeft + glm::vec3(width / 2, height / 2, depth / 2);

    GameObject* cube = new GameObject(center, name);
    cube->vertices = vertices;
    cube->indices = indices;
    addObject(cube);
}

void ObjectManager::addTriangle(glm::vec3 left, glm::vec3 right, glm::vec3 top, std::string name) {
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	vertices.insert(vertices.end(), { left, right, top });

	indices.insert(indices.end(), {
		0, 1, 2
		});

	glm::vec3 center = (left + right + top) / 3.0f;

	GameObject* triangle = new GameObject(center, name);
	triangle->vertices = vertices;
	triangle->indices = indices;
	addObject(triangle);
}