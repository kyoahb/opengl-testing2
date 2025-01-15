
#pragma once
#define GLM_FORCE_SSE2         // Enable SSE2
#define GLM_FORCE_SSE3         // Enable SSE3
#define GLM_FORCE_SSSE3        // Enable SSSE3
#define GLM_FORCE_SSE4_1       // Enable SSE4.1
#define GLM_FORCE_SSE4_2       // Enable SSE4.2
#define GLM_FORCE_AVX          // Enable AVX
#define GLM_FORCE_AVX2         // Enable AVX2
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES  // Align types to 16 bytes
#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <string>
#include "Useful.h"
#include <glm/gtx/quaternion.hpp>
#include "Vertex.h"
#include "Shader_l.h"
#include "Texture.h"

class GameObject {
protected:
    // Handles
    int id;
    std::string name;
    // Positioning
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

public:

    GameObject(
        const std::string& _name = "Unnamed Object",
        const glm::vec3& _position = glm::vec3(0.0f), 
        const glm::quat& _rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), 
        const glm::vec3& _scale = glm::vec3(1.0f));
    virtual ~GameObject() = default;

    const std::string& getName() const;
    const glm::vec3& getPosition() const;
    const glm::quat& getQuatRotation() const;
    const glm::vec3& getEulerRotation() const;
    const glm::vec3& getScale() const;
    int getId() const;

    void move(const glm::vec3& change);
    void rotateEuler(const glm::vec3& _rotation);
    void rotateQuat(const glm::quat& _rotation);
	void setRotation(const glm::quat& _rotation);
    void addScale(const glm::vec3& _scale);
    glm::mat4 calculateModelMatrix() const;

private:

};


class VertexObject : public GameObject {
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;
	Shader* shader;


public:
    VertexObject(
        const std::string& _name = "Unnamed VertexObject",
        const std::vector<Vertex>& _vertices = {},
        const std::vector<unsigned int>& _indices = {},
        const std::vector<Texture*>& _textures = {},
        Shader* _shader = nullptr,
        const glm::vec3& _position = glm::vec3(0.0f),
        const glm::quat& _rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
        const glm::vec3& _scale = glm::vec3(1.0f));
    virtual ~VertexObject() = default;

    const std::vector<Vertex>& getVertices() const;
    const std::vector<unsigned int>& getIndices() const;
    const std::vector<Texture*>& getTextures() const;
    Shader* getShader() const;

};