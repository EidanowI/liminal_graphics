#include <string>
#include <fstream>
#include <vector>
#include <iostream>



class VKPipeline {
public:
	VKPipeline(const std::string& vertexShader_name, const std::string& fragmentShader_name) noexcept;
	~VKPipeline() noexcept;

private:
	static std::vector<char> ReadFromFile(const std::string& file_name) noexcept;
	
	static void CreatePipeline(const std::string& vertexShader_name, const std::string& fragmentShader_name) noexcept;


};