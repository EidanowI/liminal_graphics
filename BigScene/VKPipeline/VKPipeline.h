///std
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "../VKEngineDevice/VKEngineDevice.h"



struct PipelineConfigInfo {
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};


class VKPipeline {
public:
	VKPipeline(VKEngineDevice& device, const std::string& vertexShader_name, const std::string& fragmentShader_name, const PipelineConfigInfo& configInfo);
	~VKPipeline();

	VKPipeline(const VKPipeline&) = delete;
	void operator=(const VKPipeline&) = delete;
	VKPipeline(VKPipeline&&) = delete;
	VKPipeline& operator=(VKPipeline&&) = delete;

	static PipelineConfigInfo CreateDefaultPipelineConfigInfo(unsigned int width, unsigned int height);

private:
	static std::vector<char> ReadFromFile(const std::string& file_name);
	
	void CreatePipeline(const std::string& vertexShader_name, const std::string& fragmentShader_name, const PipelineConfigInfo& configInfo);

	void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

private:
	VKEngineDevice& m_engineDevice;
	VkPipeline m_vkGraphicsPipeline;
	VkShaderModule m_vkVertex_shaderModule;
	VkShaderModule m_vkFragment_shaderModule;
};