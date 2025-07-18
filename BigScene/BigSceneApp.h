#include "MainWindow/MainWindow.h"
#include "VKPipeline/VKPipeline.h"
#include "VKEngineDevice/VKEngineDevice.h"
#include "VKSwapChain/VKSwapChain.h"



#define WIDTH 1280
#define HEIGHT 720


class BigSceneApp {
public:
	BigSceneApp() {
		m_pMainWindow = new MainWindow(WIDTH, HEIGHT);
		m_pEngineDevice = new VKEngineDevice(*m_pMainWindow);
		m_pSwapChain = new VKSwapChain(*m_pEngineDevice, m_pMainWindow->GetExtent());
		CreatePipelineLayout();
		CreatePipeline();
		//CreateCommandBuffer();
	}
	~BigSceneApp() {
		delete m_pVkpipeline;

		vkDestroyPipelineLayout(m_pEngineDevice->GetDevice(), m_pipelineLayout, nullptr);

		delete m_pSwapChain;
		delete m_pEngineDevice;
		delete m_pMainWindow;
	}

	void Run() noexcept {
		while (!m_pMainWindow->IsShouldClose()) {
			glfwPollEvents();
		}
	}

private:
	void CreatePipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayout_createInfo{};
		pipelineLayout_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayout_createInfo.setLayoutCount = 0;
		pipelineLayout_createInfo.pSetLayouts = nullptr;
		pipelineLayout_createInfo.pushConstantRangeCount = 0;
		pipelineLayout_createInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(m_pEngineDevice->GetDevice(), &pipelineLayout_createInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout!"); 
		}
	}
	void CreatePipeline() {
		auto pipeline_config = VKPipeline::CreateDefaultPipelineConfigInfo(m_pSwapChain->GetWidth(), m_pSwapChain->GetHeight());
		pipeline_config.renderPass = m_pSwapChain->GetRenderPass();
		pipeline_config.pipelineLayout = m_pipelineLayout;
		m_pVkpipeline = new VKPipeline(*m_pEngineDevice, "Shaders/test.vert.spv", "Shaders/test.frag.spv", pipeline_config);
	}
	void CreateCommandBuffer();
	void DrawFrame();

private:
	MainWindow* m_pMainWindow = nullptr;
	VKEngineDevice* m_pEngineDevice = nullptr;
	VKSwapChain* m_pSwapChain = nullptr;
	VKPipeline* m_pVkpipeline = nullptr;
	VkPipelineLayout m_pipelineLayout;
	std::vector<VkCommandBuffer> m_commandBuffers;
};