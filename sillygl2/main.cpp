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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


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
        frames++;
        if (currentFrame - lastSecond > 1) {
            // A full second has passed. Return all the frames that have passed between that time.
            lastSecond = currentFrame;
            std::cout << "FPS: " << frames << "\n";
            frames = 0;
        }
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