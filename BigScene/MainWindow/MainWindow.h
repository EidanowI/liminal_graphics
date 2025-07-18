#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>



class MainWindow {
public:
	MainWindow(unsigned int width, unsigned int height) noexcept;
	~MainWindow() noexcept;

	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;

public:
	bool IsShouldClose() noexcept;

	void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	VkExtent2D GetExtent() { return { m_width, m_height }; }

private:
	void InitWindow(unsigned int width, unsigned int height) noexcept;

private:
	GLFWwindow* m_pGLFWwindow = nullptr;
	unsigned int m_width = 0, m_height = 0;
};