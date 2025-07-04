#include "MainWindow.h"



MainWindow::MainWindow() noexcept {
	InitWindow();
}
MainWindow::~MainWindow() noexcept {
	glfwDestroyWindow(m_pGLFWwindow);

	glfwTerminate();
}

bool MainWindow::IsShouldClose() noexcept {
	return glfwWindowShouldClose(m_pGLFWwindow);
}

void MainWindow::InitWindow() noexcept {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	int width = 1920;
	int height = 1080;

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (mode) {
		width = mode->width;
		height = mode->height;
	}

	m_pGLFWwindow = glfwCreateWindow(width, height, "BigScene", nullptr, nullptr);
}