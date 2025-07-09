#include "MainWindow.h"



MainWindow::MainWindow(unsigned int width, unsigned int height) noexcept {
	InitWindow(width, height);
}
MainWindow::~MainWindow() noexcept {
	glfwDestroyWindow(m_pGLFWwindow);

	glfwTerminate();
}

bool MainWindow::IsShouldClose() noexcept {
	return glfwWindowShouldClose(m_pGLFWwindow);
}

void MainWindow::InitWindow(unsigned int width, unsigned int height) noexcept {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_pGLFWwindow = glfwCreateWindow(width, height, "BigScene", nullptr, nullptr);
}