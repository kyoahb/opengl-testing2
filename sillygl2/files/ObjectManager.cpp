#include "ObjectManager.h"

ObjectManager::ObjectManager() : storedRMatrix(glm::mat4(1.0f)), storedRotation(false) {
    GameObject* origin = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), "origin", false);
	std::vector<glm::vec3> v = { glm::vec3(0.0f, 0.0f, 0.0f) };
	std::vector<unsigned int> i = {0};
	origin->addVerticesIndices(v, i, false);
    addObject(origin);
}

void ObjectManager::addObject(GameObject* object) {
    object->id = nextID;
    nextID += 1;
    objects.push_back(object);
	addedObjects.push_back(object);
}

void ObjectManager::destroyObject(GameObject* object) {
    if (object == nullptr) return;
	for (auto& obj : object->getChildren()) {
		destroyObject(obj);
	}
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
    //delete(object);
	removedObjects.push_back(object);
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

std::vector<GameObject*> ObjectManager::getObjectsByEvent(ObjectEvent e) {
    switch (e) {
    case ObjectEvent::Added_Object:
		return addedObjects;
    case ObjectEvent::Removed_Object:
		return removedObjects;
	case ObjectEvent::Object_Transformation:
		return transformedObjects;
	case ObjectEvent::Object_Vertices_Changed:
		return vertChangedObjects;
	default:
		return {};
    }
}

void ObjectManager::clearEvent(ObjectEvent e) {
	switch (e) {
	case ObjectEvent::Added_Object:
		addedObjects.clear();
		break;
	case ObjectEvent::Removed_Object:
		removedObjects.clear();
		break;
	case ObjectEvent::Object_Transformation:
		transformedObjects.clear();
		break;
	case ObjectEvent::Object_Vertices_Changed:
		vertChangedObjects.clear();
		break;
	default:
		break;
	}
}

void ObjectManager::rotateObjectsR(std::vector<GameObject*>& objects, glm::vec3 rotation) {
    if (rotation != storedRotation) {
        storedRMatrix = mat4Rotate(rotation);
        storedRotation = rotation;
    }

    for (auto& object : objects) {
        object->rotateWithMatrix(storedRMatrix, rotation);
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

    vertices = { v0, v1, v2, v3, v4, v5, v6, v7 };

    indices = {
        0, 1, 2, 2, 3, 0, // Front face
        4, 5, 6, 6, 7, 4, // Back face
        0, 3, 7, 7, 4, 0, // Left face
        1, 2, 6, 6, 5, 1, // Right face
        3, 2, 6, 6, 7, 3, // Top face
        0, 1, 5, 5, 4, 0  // Bottom face
        };

    glm::vec3 center = bottomLeft + glm::vec3(width / 2, height / 2, depth / 2);

    GameObject* cube = new GameObject(center, name, true);
	cube->addVerticesIndices(vertices, indices, false);
    addObject(cube);
}

void ObjectManager::addTriangle(glm::vec3 left, glm::vec3 right, glm::vec3 top, std::string name) {
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	vertices = { left, right, top };

	indices = {
		0, 1, 2
		};

	glm::vec3 center = (left + right + top) / 3.0f;

	GameObject* triangle = new GameObject(center, name, true);
	triangle->addVerticesIndices(vertices, indices, false);
	addObject(triangle);
}