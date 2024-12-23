#pragma once
#include "Input.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "MenuManager.h"
#include "Useful.h"

class Manager {
public:
    static Manager& getInstance();

    InputManager* getInputManager();
    ObjectManager* getObjectManager();
    Renderer* getRenderer();
    MenuManager* getMenuManager();
    GLFWwindow* getWindow();

    void initialize(InputManager* inputMgr, ObjectManager* objectMgr, Renderer* rend, MenuManager* menuMgr, GLFWwindow* wind);

private:
    Manager();
    ~Manager();

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    InputManager* inputManager;
    ObjectManager* objectManager;
    Renderer* renderer;
    MenuManager* menuManager;
    GLFWwindow* window;
};
