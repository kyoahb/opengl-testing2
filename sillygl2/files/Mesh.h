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
#include "Vertex.h"
#include "Texture.h"
#include "Shader_l.h"
#include "Camera.h"
#include "Object.h"
#include <glm/gtx/quaternion.hpp>
class Mesh : public VertexObject {
public:
	std::vector<Mesh*> children;
    Camera* attachedCamera;

    Mesh(const std::string& _name = "Unnamed Mesh",
        const std::vector<Vertex>& _vertices = {},
        const std::vector<unsigned int>& _indices = {},
        const std::vector<Texture*>& _textures = {},
        Shader* _shader = nullptr,
        const glm::vec3& _position = glm::vec3(0.0f),
        const glm::quat& _rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
        const glm::vec3& _scale = glm::vec3(1.0f));

	void attachCamera(Camera* camera);

    void move(const glm::vec3& change);
    void rotateEuler(const glm::vec3& _rotation);
    void rotateQuat(const glm::quat& _rotation);
    void setRotation(glm::quat _rotation);
    void addScale(const glm::vec3& _scale);

    void setId(unsigned int _id);

    void draw(Shader& shader);
    void drawSingleTexture();

    void resendVerticesIndices();
    void resendModelMatrix();
    void formModelMatrix();
private:
    //  render data
    unsigned int VAO, VBO, EBO, modelBuffer;
    glm::mat4 model;
	bool modifiedThisFrame = false; // Whether any transformations have been applied to the mesh this frame, which should be reflected in the model matrix.

    void setupMesh();
};