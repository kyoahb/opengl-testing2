#define GLM_FORCE_SSE2         // Enable SSE2
#define GLM_FORCE_SSE3         // Enable SSE3
#define GLM_FORCE_SSSE3        // Enable SSSE3
#define GLM_FORCE_SSE4_1       // Enable SSE4.1
#define GLM_FORCE_SSE4_2       // Enable SSE4.2
#define GLM_FORCE_AVX          // Enable AVX
#define GLM_FORCE_AVX2         // Enable AVX2
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES  // Align types to 16 bytes

#define GLM_ENABLE_EXPERIMENTAL

#include "MenuManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Useful.h"
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Input.h"
#include "Renderer.h"
#include "MeshManager.h"
#include "ScriptManager.h"
#include "Manager.h"
// Game Scripts
#include "control.h"
#include "engine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_focus_callback(GLFWwindow* window, int focused);

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(Manager::getInstance().SCR_WIDTH, Manager::getInstance().SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    TextureManager textureManager;
    MeshManager meshManager(&textureManager);
    InputManager inputManager(window);
    ScriptManager scriptManager;
    MenuManager menuManager(window);
    Renderer renderer(&meshManager);

    Manager::getInstance().initialize(&inputManager, &meshManager, &textureManager, &renderer, &menuManager, window);

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
        renderer.renderTest((float)deltaTime);
        

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
	Manager::getInstance().SCR_WIDTH = width;
	Manager::getInstance().SCR_HEIGHT = height;
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