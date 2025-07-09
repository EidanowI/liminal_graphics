#include "VKPipeline.h"



VKPipeline::VKPipeline(VKEngineDevice& device, const std::string& vertexShader_name, const std::string& fragmentShader_name, const PipelineConfigInfo& configInfo) : m_engineDevice(device){
	CreatePipeline(vertexShader_name, fragmentShader_name, configInfo);
}
VKPipeline::~VKPipeline() noexcept {

}

PipelineConfigInfo VKPipeline::CreateDefaultPipelineConfigInfo(unsigned int width, unsigned int height) {
	PipelineConfigInfo config_info{};

	return config_info;
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

	std::cout << "Vertex code size " << vert_sourceCode.size();
	std::cout << "Fragment code size " << frag_sourceCode.size();
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