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

void Manager::initialize(InputManager* inputMgr, ObjectManager* objectMgr, TextureManager* textureMgr, Renderer* rend, MenuManager* menuMgr, GLFWwindow* wind) {
    inputManager = inputMgr;
    textureManager = textureMgr;
    objectManager = objectMgr;
    renderer = rend;
    menuManager = menuMgr;
    window = wind;
}

Manager::Manager() : inputManager(nullptr), objectManager(nullptr), textureManager(nullptr), renderer(nullptr), menuManager(nullptr), window(nullptr) {}

Manager::~Manager() = default;
