#include "Manager.h"

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

InputManager* Manager::getInputManager() {
    return inputManager;
}

MeshManager* Manager::getMeshManager() {
	return meshManager;
}

TextureManager* Manager::getTextureManager() {
	return textureManager;
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

void Manager::initialize(InputManager* inputMgr, MeshManager* meshMgr, TextureManager* textureMgr, Renderer* rend, MenuManager* menuMgr, GLFWwindow* wind) {
    inputManager = inputMgr;
    textureManager = textureMgr;
    meshManager = meshMgr;
    renderer = rend;
    menuManager = menuMgr;
    window = wind;
}

Manager::Manager() : inputManager(nullptr), meshManager(nullptr), textureManager(nullptr), renderer(nullptr), menuManager(nullptr), window(nullptr) {}

Manager::~Manager() = default;
