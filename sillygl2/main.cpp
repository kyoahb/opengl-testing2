#include "MenuManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Useful.h"
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Input.h"
#include "Renderer.h"
#include "ObjectManager.h"
#include "ScriptManager.h"
#include "Manager.h"
// Game Scripts
#include "control.h"
#include "engine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_focus_callback(GLFWwindow* window, int focused);
// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    // glad: load all OpenGL function pointers
// ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // load globals
    ObjectManager objectManager;
    InputManager inputManager(window);
    ScriptManager scriptManager;
    MenuManager menuManager(window);
    Renderer renderer(&objectManager, SCR_WIDTH, SCR_HEIGHT);

    Manager::getInstance().initialize(&inputManager, &objectManager, &renderer, &menuManager, window);

    scriptManager.registerScript(new ControlScript());
    scriptManager.registerScript(new EngineScript());

    // Start scripts
    scriptManager.startScripts();

    double deltaTime = 0.0f;
    double lastFrame = 0.0f;
    double lastSecond = glfwGetTime();
    int frames = 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // change deltaTime
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //menu
        menuManager.frameStart();

        // input
        inputManager.update(deltaTime);

        //update scripts
        scriptManager.updateScripts(deltaTime);

        // render
        // ------
        renderer.render();
        

        menuManager.frameEnd();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glFlush();
        
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    menuManager.shutdown();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void window_focus_callback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        // Window gained focus
        std::cout << "Window gained focus" << std::endl;
    }
    else
    {
        // Window lost focus
        std::cout << "Window lost focus" << std::endl;
    }
} 