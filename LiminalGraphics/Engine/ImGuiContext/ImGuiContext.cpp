#include "ImGuiContext.h"



Engine::ImGuiContext::ImGuiContext(GLFWwindow* pGLFWWindow) {
    ImGui::CreateContext();
    /*ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;*/

    ImGui_ImplGlfw_InitForOpenGL(pGLFWWindow, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}
Engine::ImGuiContext::~ImGuiContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Engine::ImGuiContext::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void Engine::ImGuiContext::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}