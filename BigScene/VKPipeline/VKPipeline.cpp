#include "VKPipeline.h"



VKPipeline::VKPipeline(VKEngineDevice& device, const std::string& vertexShader_name, const std::string& fragmentShader_name, const PipelineConfigInfo& configInfo) : m_engineDevice(device){
	CreatePipeline(vertexShader_name, fragmentShader_name, configInfo);
}
VKPipeline::~VKPipeline(){
	vkDestroyPipeline(m_engineDevice.GetDevice(), m_vkGraphicsPipeline, nullptr);

	vkDestroyShaderModule(m_engineDevice.GetDevice(), m_vkVertex_shaderModule, nullptr);
	vkDestroyShaderModule(m_engineDevice.GetDevice(), m_vkFragment_shaderModule, nullptr);
}

PipelineConfigInfo VKPipeline::CreateDefaultPipelineConfigInfo(unsigned int width, unsigned int height) {
	PipelineConfigInfo configInfo{};

	configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	configInfo.viewport.x = 0.0f;
	configInfo.viewport.y = 0.0f;
	configInfo.viewport.width = static_cast<float>(width);
	configInfo.viewport.height = static_cast<float>(height);
	configInfo.viewport.minDepth = 0.0f;
	configInfo.viewport.maxDepth = 1.0f;

	configInfo.scissor.offset = { 0, 0 };
	configInfo.scissor.extent = { width, height };

	configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
	configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	configInfo.rasterizationInfo.lineWidth = 1.0f;
	configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
	configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
	configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
	configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

	configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
	configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
	configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
	configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
	configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

	configInfo.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
	configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
	configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

	configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
	configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
	configInfo.colorBlendInfo.attachmentCount = 1;
	configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
	configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
	configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
	configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
	configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

	configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
	configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
	configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
	configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
	configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
	configInfo.depthStencilInfo.front = {};  // Optional
	configInfo.depthStencilInfo.back = {};   // Optional

	return configInfo;
}

std::vector<char> VKPipeline::ReadFromFile(const std::string& file_name){
	std::ifstream ifs(file_name, std::ios::ate | std::ios::binary);

	if (!ifs.is_open()) {
		std::cout << "ERROR: Fail to open file " << file_name << '\n';
	}

	int file_size = ifs.tellg();
	ifs.seekg(0);

	std::vector<char> buffer = std::vector<char>(file_size);

	ifs.read(buffer.data(), file_size);

	ifs.close();

	return buffer;
}

void VKPipeline::CreatePipeline(const std::string& vertexShader_name, const std::string& fragmentShader_name, const PipelineConfigInfo& configInfo){
	auto vert_sourceCode = ReadFromFile(vertexShader_name);
	auto frag_sourceCode = ReadFromFile(fragmentShader_name);
	
	CreateShaderModule(vert_sourceCode, &m_vkVertex_shaderModule);
	CreateShaderModule(frag_sourceCode, &m_vkFragment_shaderModule);

	VkPipelineShaderStageCreateInfo shader_stages[2]{};

	shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shader_stages[0].module = m_vkVertex_shaderModule;
	shader_stages[0].pName = "main";
	shader_stages[0].flags = 0;
	shader_stages[0].pNext = nullptr;
	shader_stages[0].pSpecializationInfo = nullptr;

	shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shader_stages[1].module = m_vkFragment_shaderModule;
	shader_stages[1].pName = "main";
	shader_stages[1].flags = 0;
	shader_stages[1].pNext = nullptr;
	shader_stages[1].pSpecializationInfo = nullptr;

	VkPipelineVertexInputStateCreateInfo vertexInputState_createInfo{};
	vertexInputState_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputState_createInfo.pVertexBindingDescriptions = nullptr;
	vertexInputState_createInfo.vertexBindingDescriptionCount = 0;
	vertexInputState_createInfo.pVertexAttributeDescriptions = nullptr;
	vertexInputState_createInfo.vertexAttributeDescriptionCount = 0;

	VkPipelineViewportStateCreateInfo viewport_createInfo{};
	viewport_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_createInfo.viewportCount = 1;
	viewport_createInfo.pViewports = &configInfo.viewport;
	viewport_createInfo.scissorCount = 1;
	viewport_createInfo.pScissors = &configInfo.scissor;


	VkGraphicsPipelineCreateInfo pipeline_createInfo{};
	pipeline_createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_createInfo.stageCount = 2;
	pipeline_createInfo.pStages = shader_stages;
	pipeline_createInfo.pVertexInputState = &vertexInputState_createInfo;
	pipeline_createInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
	pipeline_createInfo.pViewportState = &viewport_createInfo;
	pipeline_createInfo.pRasterizationState = &configInfo.rasterizationInfo;
	pipeline_createInfo.pMultisampleState = &configInfo.multisampleInfo;
	pipeline_createInfo.pColorBlendState = &configInfo.colorBlendInfo;
	pipeline_createInfo.pDepthStencilState = &configInfo.depthStencilInfo;
	pipeline_createInfo.pDynamicState = nullptr;

	pipeline_createInfo.layout = configInfo.pipelineLayout;
	pipeline_createInfo.renderPass = configInfo.renderPass;
	pipeline_createInfo.subpass = configInfo.subpass;

	pipeline_createInfo.basePipelineIndex = -1;
	pipeline_createInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(m_engineDevice.GetDevice(), VK_NULL_HANDLE, 1, &pipeline_createInfo, nullptr, &m_vkGraphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create graphics pipeline!");
	}
}

void VKPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
	VkShaderModuleCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = code.size();
	//create_info.pCode = (const unsigned int*)code.data();
	create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(m_engineDevice.GetDevice(), &create_info, nullptr, shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("Fail to create shader module");
	}
}