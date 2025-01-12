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
#include <glm/gtx/quaternion.hpp>
class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

	std::vector<Mesh*> children;
    Camera* attachedCamera;


    std::string name;
    unsigned int id;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;

    Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture*> _textures, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, std::string _name); // Automatically assigned name and position

	void move(glm::vec3 change);
	void rotateEuler(glm::vec3 _rotation);
	void rotateQuat(glm::quat _rotation);
	void addScale(glm::vec3 _scale);

	void attachCamera(Camera* camera);

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