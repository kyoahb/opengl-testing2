#pragma once
#include "Input.h"
#include "ObjectManager.h"
#include "MeshManager.h"
#include "Renderer.h"
#include "MenuManager.h"
#include "Useful.h"

class Manager {
public:
    static Manager& getInstance();

    InputManager* getInputManager();
    ObjectManager* getObjectManager();
	MeshManager* getMeshManager();
    Renderer* getRenderer();
    MenuManager* getMenuManager();
    GLFWwindow* getWindow();

    void initialize(InputManager* inputMgr, ObjectManager* objectMgr, MeshManager* meshMgr, Renderer* rend, MenuManager* menuMgr, GLFWwindow* wind);

private:
    Manager();
    ~Manager();

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    InputManager* inputManager;
    ObjectManager* objectManager;
    MeshManager* meshManager;
    Renderer* renderer;
    MenuManager* menuManager;
    GLFWwindow* window;
};
