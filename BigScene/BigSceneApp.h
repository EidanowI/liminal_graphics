#include "MainWindow/MainWindow.h"



class BigSceneApp {
public:
	void Run() noexcept {
		while (!m_mainWindow.IsShouldClose()) {
			glfwPollEvents();
		}
	}

private:
	MainWindow m_mainWindow;
};