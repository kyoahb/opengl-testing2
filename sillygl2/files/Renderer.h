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
#include "TextureManager.h"
#include "Vertex.h"
#include "MeshManager.h"

class Manager;

class Renderer {
public:
	Texture texture;
	Renderer(MeshManager* _meshManager);
	void setCamera(Camera* camera); // Attach camera to renderer

	void renderTest(float deltaTime);
	void preRenderTest();
	Texture createTestTexture();

private:
    float const vecSize = sizeof(float) * 3;
	unsigned int SCR_WIDTH = 1280; // Width of screen
	unsigned int SCR_HEIGHT = 720; // Height of screen
	Camera* globalCamera; // Camera to render from (if exists)
	MeshManager* meshManager; // Object Manager to get objects from
	unsigned int UBOmatrices; // Uniform Buffer Object containing view and perspective matrices, shared between all shaders
	glm::mat4 projection, model; // Projection and Model matrices
    glm::mat4* view; // only initialised if camera is set
	bool* updatedView;
};