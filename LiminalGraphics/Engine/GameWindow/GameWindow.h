#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>



namespace Engine {
	void FrameBufferResize_callback(GLFWwindow* window, int width, int height);

	class GameWindow {
	public:
		GameWindow();
		~GameWindow();

		GameWindow(const GameWindow&) = delete;
		GameWindow& operator=(const GameWindow&) = delete;
		GameWindow(GameWindow&&) = delete;
		GameWindow& operator=(GameWindow&&) = delete;		

	public:
		bool IsShouldCloseWindow();
		void SwapBuffers();

	public:
		GLFWwindow* GetGLFWWindow();

	private:
		void InitGLFWWindow();
		void InitOpenGL();
		//void InitImGui();
		void ConfigOpenGL();

	public:
		unsigned int m_width = 800;
		unsigned int m_height = 600;

		GLFWwindow* m_glfw_window;
	};
}
#endif // GAMEWINDOW_H