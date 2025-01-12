#pragma once
#include "Input.h"
#include "ObjectManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "MenuManager.h"
#include "Renderer.h"
#include "Useful.h"

class Manager {
public:
    static Manager& getInstance();

    // settings
    const unsigned int SCR_WIDTH = 1280;
    const unsigned int SCR_HEIGHT = 720;
	const float FOV = 60.0f;

    InputManager* getInputManager();
    ObjectManager* getObjectManager();
	MeshManager* getMeshManager();
    TextureManager* getTextureManager();
    Renderer* getRenderer();
    MenuManager* getMenuManager();
    GLFWwindow* getWindow();

    void initialize(InputManager* inputMgr, ObjectManager* objectMgr, MeshManager* meshMgr, TextureManager* textureMgr, Renderer* rend, MenuManager* menuMgr, GLFWwindow* wind);

private:
    Manager();
    ~Manager();

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    InputManager* inputManager = nullptr;
    ObjectManager* objectManager = nullptr;
    MeshManager* meshManager = nullptr;
	TextureManager* textureManager = nullptr;
    Renderer* renderer = nullptr;
    MenuManager* menuManager = nullptr;
    GLFWwindow* window = nullptr;
};
