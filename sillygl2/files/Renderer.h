#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Useful.h"
#include <random>
#include <iostream>
#include "Camera.h"
#include "Shader_l.h"
#include "Object.h"
#include "ObjectManager.h"

class Renderer {
public:
    Shader shader;
	Renderer(ObjectManager* objManager, unsigned int scr_width, unsigned int scr_height);
	void setCamera(Camera* camera);
	void render();

private:
	void old_render1();
    std::vector<GameObject*>* objects;
	std::vector<GameObject*> renderedObjects;

    float const vecSize = sizeof(float) * 3;
    std::vector<GLint> firstIndices; // Flat Vector storing position of first index of each object in the combinedIndices Vector
	// firstIndices[i] is the first index of the i+1th object, inside combinedIndices
	// eg: firstIndices[0] = 0, means that the 1st object's indices start at index 0 in combinedIndices

	std::vector<GLint> firstVertices; // Flat Vector storing position of first index of each object in the combinedVertices Vector
	// firstVertices[i] is the first index of the i+1th object, inside combinedVertices
	// eg: firstVertices[0] = 0, means that the 1st object's vertices start at index 0 in combinedVertices

	std::vector<GLsizei> numIndices; // Flat Vector storing the number of indices each object has
	// numIndices[i] is the number of indices of the i+1th object
	// eg: numIndices[0] = 3, means that the 1st object has 3 indices

	std::vector<GLsizei> numVertices; // Flat Vector storing the number of vertices each object has
	// numVertices[i] is the number of vertices of the i+1th object
	// eg: numVertices[0] = 3, means that the 1st object has 3 vertices

	std::vector<glm::vec3> combinedVertices; // Flat Vector storing all vertices of all objects
	// combinedVertices is a vector of all vertices of all objects
	// eg: combinedVertices[0] to combinedVertices[2] are the 3 vertices of the 1st object, with 3 indices.

	std::vector<unsigned int> combinedIndices; // Flat Vector storing all indices of all objects
	// combinedIndices is a vector of all indices of all objects, including the offset from previous objects.
	// if A->indices = {0, 1, 2}, B->indices = {0, 1, 2}, and they are rendered A->B, then combinedIndices = {0, 1, 2, (INDEX OFFSET: 3) 3, 4, 5}

	unsigned int indexOffset; // Offset for each object's indices; sum of number of all previous objects' vertices

    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    Camera* globalCamera;
    ObjectManager* objectManager;
    unsigned int VAO, VBO, EBO;
    glm::mat4 projection, model;
    glm::mat4* view; // only initialised if camera is set
};