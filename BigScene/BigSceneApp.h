#include "MainWindow/MainWindow.h"
#include "VKPipeline/VKPipeline.h"
#include "VKEngineDevice/VKEngineDevice.h"



#define WIDTH 1280
#define HEIGHT 720


class BigSceneApp {
public:
	BigSceneApp() {
		m_pMainWindow = new MainWindow(WIDTH, HEIGHT);
		m_pEngineDevice = new VKEngineDevice(*m_pMainWindow);
		m_pVkpipeline = new VKPipeline(*m_pEngineDevice, "Shaders/test.vert.spv", "Shaders/test.frag.spv", VKPipeline::CreateDefaultPipelineConfigInfo(WIDTH, HEIGHT));
	}
	~BigSceneApp() {
		delete m_pVkpipeline;
		delete m_pEngineDevice;
		delete m_pMainWindow;
	}

	void Run() noexcept {
		while (!m_pMainWindow->IsShouldClose()) {
			glfwPollEvents();
		}
	}

private:
	MainWindow* m_pMainWindow = nullptr;
	VKEngineDevice* m_pEngineDevice = nullptr;
	VKPipeline* m_pVkpipeline = nullptr;
};