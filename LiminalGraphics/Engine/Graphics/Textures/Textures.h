#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <lodepng/lodepng.h>


//std
#include <string>
#include <vector>



namespace Engine {
	class Textures {
	public:
		Textures(const std::string texture_name);
		~Textures();

		void Bind(int index);

	private:
		static void FlipYTexture(std::vector<unsigned char>& data, unsigned int width, unsigned int height);

	private:
		unsigned int m_textureID;
	};
}