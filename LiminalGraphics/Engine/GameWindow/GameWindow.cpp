#include "GameWindow.h"

#ifdef DEBUG
#include <iostream>
#endif // DEBUG



extern "C" {
    _declspec(dllexport) unsigned long NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}



Engine::GameWindow::GameWindow() {
    InitGLFWWindow();
    InitOpenGL();
    ConfigOpenGL();
}
Engine::GameWindow::~GameWindow() {
    glfwDestroyWindow(m_glfw_window);
    glfwTerminate();
}

bool Engine::GameWindow::IsShouldCloseWindow() {
    return glfwWindowShouldClose(m_glfw_window);
}
void Engine::GameWindow::SwapBuffers() {
    glfwSwapBuffers(m_glfw_window);
}

void Engine::GameWindow::InitGLFWWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_glfw_window = glfwCreateWindow(m_width, m_height, "Liminal Graphics", NULL, NULL);

#ifdef DEBUG
    if (m_glfw_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
#endif // DEBUG
}
void Engine::GameWindow::InitOpenGL() {
    glfwMakeContextCurrent(m_glfw_window);

#ifdef DEBUG
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
#else
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif // DEBUG

    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_glfw_window, Engine::FrameBufferResize_callback);

#ifdef DEBUG
    const char* renderer = (const char*)glGetString(GL_RENDERER);
    const char* version = (const char*)glGetString(GL_VERSION);

    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;
#endif // DEBUG
}
void Engine::GameWindow::ConfigOpenGL() {
    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

GLFWwindow* Engine::GameWindow::GetGLFWWindow() {
    return m_glfw_window;
}


void Engine::FrameBufferResize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
#ifdef DEBUG
    std::cout << "Frame buffer resize calllback!" << std::endl;
#endif // DEBUG
}