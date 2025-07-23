#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


//std
#include <vector>



namespace Engine {
	struct Vertex {
		Vertex(float pos_x, float pos_y, float pos_z, float uv_x, float uv_y) : position(pos_x, pos_y, pos_z), normal{}, uv(uv_x, uv_y), color{} {}
		Vertex(float pos_x, float pos_y, float pos_z) : Vertex(pos_x, pos_y, pos_z, 0, 0){}

	public:
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec4 color;
	};


	class Mesh {
	public:
		Mesh(const std::vector<Vertex> vertecies, const std::vector<unsigned int> indecies);
		~Mesh();

		void Bind();
		void Draw();

	private:
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_EBO;

		unsigned int m_indexies_count;
	};
}
