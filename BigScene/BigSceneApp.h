#include "MainWindow/MainWindow.h"
#include "VKPipeline/VKPipeline.h"



class BigSceneApp {
public:
	void Run() noexcept {
		while (!m_mainWindow.IsShouldClose()) {
			glfwPollEvents();
		}
	}

private:
	MainWindow m_mainWindow;
	VKPipeline m_vkpipeline{"Shaders/test.vert.spv", "Shaders/test.frag.spv"};
};