#include <glad/glad.h>
#include <GLFW/glfw3.h>

//std
#include <string>
#include <vector>
#ifdef DEBUG
#include <iostream>
#endif




namespace Engine {
	class ShaderProgram {
	public:
		ShaderProgram();
		~ShaderProgram();

		void Bind();

	private:
		unsigned int m_shaderProgramID;
	};
}