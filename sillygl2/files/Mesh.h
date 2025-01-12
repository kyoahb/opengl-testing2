#pragma once
#include <vector>
#include <string>
#include "Vertex.h"
#include "Texture.h"
#include "Shader_l.h"
#include "Camera.h"

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
    glm::vec3 rotation;

    Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture*> _textures, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, std::string _name); // Automatically assigned name and position

	void move(glm::vec3 change);
	void rotate(glm::vec3 _rotation);
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