#include "Manager.h"

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

InputManager* Manager::getInputManager() {
    return inputManager;
}

ObjectManager* Manager::getObjectManager() {
    return objectManager;
}

MeshManager* Manager::getMeshManager() {
	return meshManager;
}

Renderer* Manager::getRenderer() {
    return renderer;
}

MenuManager* Manager::getMenuManager() {
    return menuManager;
}

GLFWwindow* Manager::getWindow() {
	return window;
}

void Manager::initialize(InputManager* inputMgr, ObjectManager* objectMgr, MeshManager* meshMgr, Renderer* rend, MenuManager* menuMgr, GLFWwindow* wind) {
    inputManager = inputMgr;
    objectManager = objectMgr;
    meshManager = meshMgr;
    renderer = rend;
    menuManager = menuMgr;
    window = wind;
}

Manager::Manager() : inputManager(nullptr), objectManager(nullptr), meshManager(nullptr), renderer(nullptr), menuManager(nullptr), window(nullptr) {}

Manager::~Manager() = default;
