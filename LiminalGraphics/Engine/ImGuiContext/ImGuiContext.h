#ifndef IMGUICONTEXT_H
#define IMGUICONTEXT_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>



namespace Engine {
	class ImGuiContext {
	public:
		ImGuiContext(GLFWwindow* pGLFWWindow);
		~ImGuiContext();

		ImGuiContext(const ImGuiContext&) = delete;
		ImGuiContext& operator=(const ImGuiContext&) = delete;
		ImGuiContext(ImGuiContext&&) = delete;
		ImGuiContext& operator=(ImGuiContext&&) = delete;

	public:
		void BeginFrame();
		void EndFrame();
	};
}

#endif // GAMEWINDOW_H