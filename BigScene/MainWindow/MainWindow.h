#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



class MainWindow {
public:
	MainWindow(unsigned int width, unsigned int height) noexcept;
	~MainWindow() noexcept;

	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;

public:
	bool IsShouldClose() noexcept;

private:
	void InitWindow(unsigned int width, unsigned int height) noexcept;

private:
	GLFWwindow* m_pGLFWwindow = nullptr;
};