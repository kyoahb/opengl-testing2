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
    float const vecSize = sizeof(float) * 3;
    std::vector<GLint> firsts;
    std::vector<GLsizei> counts;
    bool verticesUpdated = true;
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    std::vector<GameObject*>* objects;
    Camera* globalCamera;
    ObjectManager* objectManager;
    unsigned int VAO, VBO, EBO;
    glm::mat4 projection, model;
    glm::mat4* view; // only initialised if camera is set
};