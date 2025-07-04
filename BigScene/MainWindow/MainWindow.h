#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



class MainWindow {
public:
	MainWindow() noexcept;
	~MainWindow() noexcept;

	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;

public:
	bool IsShouldClose() noexcept;

private:
	void InitWindow() noexcept;

private:
	GLFWwindow* m_pGLFWwindow = nullptr;
};