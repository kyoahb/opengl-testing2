#define GLM_FORCE_SSE2         // Enable SSE2
#define GLM_FORCE_SSE3         // Enable SSE3
#define GLM_FORCE_SSSE3        // Enable SSSE3
#define GLM_FORCE_SSE4_1       // Enable SSE4.1
#define GLM_FORCE_SSE4_2       // Enable SSE4.2
#define GLM_FORCE_AVX          // Enable AVX
#define GLM_FORCE_AVX2         // Enable AVX2
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES  // Align types to 16 bytes

#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION
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

void GLAPIENTRY debugCallback(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);

int main() {
    Log::initialize();

	spdlog::info("Starting main()");

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // Allow debug context

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(Manager::getInstance().SCR_WIDTH, Manager::getInstance().SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
		spdlog::error("Failed to create GLFW window");
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
        spdlog::error("Failed to initialize GLAD");
        return -1;
    }

    // load globals
    ObjectManager objectManager;
    InputManager inputManager(window);
    ScriptManager scriptManager;
    MenuManager menuManager(window);
    Renderer renderer(&objectManager);
    spdlog::info("Loading globals successful.");

    Manager::getInstance().initialize(&inputManager, &objectManager, &renderer, &menuManager, window);


    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    // debug callback
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

	// load scripts
    scriptManager.registerScript(new ControlScript());
    scriptManager.registerScript(new EngineScript());

    // Start scripts
    scriptManager.startScripts();
    spdlog::info("Scripts started.");

    // Enable debug output

    double deltaTime = 0.0f;
    double lastFrame = 0.0f;
    double lastSecond = glfwGetTime();
    int frames = 0;
    // render loop
    // -----------
    spdlog::info("Render loop begun.");
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
    spdlog::info("Terminating window.");
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
        spdlog::info("Window gained focus");
    }
    else
    {
        // Window lost focus
        spdlog::info("Window lost focus");
    }
} 

void GLAPIENTRY debugCallback(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}