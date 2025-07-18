#include "MainWindow.h"



MainWindow::MainWindow(unsigned int width, unsigned int height) noexcept : m_width(width), m_height(height) {
	InitWindow(width, height);
}
MainWindow::~MainWindow() noexcept {
	glfwDestroyWindow(m_pGLFWwindow);

	glfwTerminate();
}


bool MainWindow::IsShouldClose() noexcept {
	return glfwWindowShouldClose(m_pGLFWwindow);
}

void MainWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
	if (glfwCreateWindowSurface(instance, m_pGLFWwindow, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface");
	}
}


void MainWindow::InitWindow(unsigned int width, unsigned int height) noexcept {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_pGLFWwindow = glfwCreateWindow(width, height, "BigScene", nullptr, nullptr);
}