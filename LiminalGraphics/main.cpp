#include "glad/glad.h"

#include <GLFW/glfw3.h>


#ifdef WIN
#include <Windows.h>
#endif

#include <iostream>
#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include "Engine/Graphics/Mesh/Mesh.h"
#include "Engine/Graphics/ShaderProgram/ShaderProgram.h"
#include "Engine/Graphics/Textures/Textures.h"

GLFWwindow* window;
bool doit = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    glfwSetWindowTitle(window, std::to_string(width).c_str());
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        doit = true;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left mouse button pressed\n";
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    //std::cout << "Cursor position: (" << xpos << ", " << ypos << ")\n";
}

void monic_callback(GLFWmonitor* monitor, int event) {
    if (event == 262145) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    std::cout << "monic << " << event << '\n';
}


#ifdef WIN
int main()
//int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#elif LINUX
int main()
#elif MACOS
int main()
#endif
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(800, 600, "Liminal Graphics", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMonitorCallback(monic_callback);

   // glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
   // ImGuiStyle& style = ImGui::GetStyle();
    //style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    //style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init("#version 130");

    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);


    


    glUniform4f(0, 0.3f, 1.0f, 1.0f, 1.0f);

    std::vector<Engine::Vertex> vertices = { Engine::Vertex(0.5f,  0.5f, 0.0f, 1.0f, 1.0), Engine::Vertex(0.5f, -0.5f, 0.0f, 1.0f, 0.0f), Engine::Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f), Engine::Vertex(-0.5f,  0.5f, 0.0f, 0.0f, 1.0f) };

    std::vector<unsigned int> indices = {  
        3, 1, 0,
        3, 2, 1 
    };

    Engine::Mesh mesh = Engine::Mesh(vertices, indices);

    Engine::ShaderProgram shader_program = Engine::ShaderProgram();

    Engine::Textures ground_tex = Engine::Textures("ground.png");
    Engine::Textures test_tex = Engine::Textures("test.png");
    Engine::Textures perlin_tex = Engine::Textures("perlin.png");


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        /*if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }*/
        shader_program.Bind();
        ground_tex.Bind(0);
        test_tex.Bind(1);
        perlin_tex.Bind(2);
        mesh.Bind();
        mesh.Draw();



        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool isChoto;
        if (true)
            ImGui::ShowDemoWindow(&isChoto);

        ImGui::Render();

       

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}