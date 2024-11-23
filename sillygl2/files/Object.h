
#pragma once
#include "Useful.h"
#include <vector>
#include <string>

extern bool objectsUpdated; // Declare the global variable

class GameObject {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    std::vector<glm::vec3> vertices;
    std::string name;
    std::vector<unsigned int> indices;

    GameObject(glm::vec3 pos, std::string handle);

    void move(glm::vec3 change);
    void rotate(glm::mat4 rotationMatrix);
    void scaleInPlace(glm::vec3 scale);
    void scale(glm::vec3 scale);
    void getAABB(glm::vec3& min, glm::vec3& max) const;
};
