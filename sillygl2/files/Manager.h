#pragma once
#include "Input.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "MenuManager.h"
#include "Renderer.h"
#include "Useful.h"

class Manager {
public:
    static Manager& getInstance();

    // settings
    unsigned int SCR_WIDTH = 1280;
    unsigned int SCR_HEIGHT = 720;
	const float FOV = 60.0f;

    InputManager* getInputManager();
	ObjectManager* getObjectManager();
    TextureManager* getTextureManager();
    Renderer* getRenderer();
    MenuManager* getMenuManager();
    GLFWwindow* getWindow();

    void initialize(InputManager* inputMgr, ObjectManager* objectMgr, TextureManager* textureMgr, Renderer* rend, MenuManager* menuMgr, GLFWwindow* wind);

private:
    Manager();
    ~Manager();

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    InputManager* inputManager = nullptr;
    ObjectManager* objectManager = nullptr;
	TextureManager* textureManager = nullptr;
    Renderer* renderer = nullptr;
    MenuManager* menuManager = nullptr;
    GLFWwindow* window = nullptr;
};
