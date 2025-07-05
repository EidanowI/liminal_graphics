#include "VKPipeline.h"



VKPipeline::VKPipeline(const std::string& vertexShader_name, const std::string& fragmentShader_name) noexcept {
	CreatePipeline(vertexShader_name, fragmentShader_name);
}
VKPipeline::~VKPipeline() noexcept {

}

std::vector<char> VKPipeline::ReadFromFile(const std::string& file_name) noexcept {
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

void VKPipeline::CreatePipeline(const std::string& vertexShader_name, const std::string& fragmentShader_name) noexcept {
	auto vert_sourceCode = ReadFromFile(vertexShader_name);
	auto frag_sourceCode = ReadFromFile(fragmentShader_name);

	std::cout << "Vertex code size " << vert_sourceCode.size();
	std::cout << "Fragment code size " << frag_sourceCode.size();
}