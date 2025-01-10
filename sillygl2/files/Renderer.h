#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Useful.h"
#include <random>
#include <iostream>
#include "stbimage/stb_image.h"
#include "Camera.h"
#include "Shader_l.h"
#include "Object.h"
#include "Texture.h"
#include "Vertex.h"
#include "../MeshManager.h"

class Renderer {
public:
    Shader shader;
	Texture texture;
	Renderer(MeshManager* _meshManager, unsigned int scr_width, unsigned int scr_height);
	void setCamera(Camera* camera); // Attach camera to renderer

	void renderTest();
	Texture createTestTexture();

private:
    float const vecSize = sizeof(float) * 3;
	unsigned int SCR_WIDTH; // Width of screen
	unsigned int SCR_HEIGHT; // Height of screen
	Camera* globalCamera; // Camera to render from (if exists)
	MeshManager* meshManager; // Object Manager to get objects from
	unsigned int VAO, VBO, EBO; // Vertex Array Object, Vertex Buffer Object, Element Buffer Object
	glm::mat4 projection, model; // Projection and Model matrices
    glm::mat4* view; // only initialised if camera is set
	bool* updatedView;
};