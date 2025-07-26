#include "App.h"

#include "Engine/Graphics/Mesh/Mesh.h"
#include "Engine/Graphics/ShaderProgram/ShaderProgram.h"
#include "Engine/Graphics/Textures/Textures.h"



App::App() : m_gameWindow(), m_imguiContext(m_gameWindow.GetGLFWWindow()){

}

void App::Run() {
	std::vector<Engine::Vertex> vertices = { Engine::Vertex(0.5f,  0.5f, 0.0f, 1.0f, 1.0), Engine::Vertex(0.5f, -0.5f, 0.0f, 1.0f, 0.0f), Engine::Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f), Engine::Vertex(-0.5f,  0.5f, 0.0f, 0.0f, 1.0f) };

	std::vector<unsigned int> indices = {
		3, 1, 0,
		3, 2, 1
	};

	Engine::Mesh mesh = Engine::Mesh(vertices, indices);

	Engine::ShaderProgram shader_program = Engine::ShaderProgram();

	Engine::Textures ground_tex = Engine::Textures("ground.png");
	Engine::Textures test_tex = Engine::Textures("test.png");
	Engine::Textures perlin_tex = Engine::Textures("perlin.png");

	glfwSwapInterval(1);

	while (!m_gameWindow.IsShouldCloseWindow()) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		///

		shader_program.Bind();
        ground_tex.Bind(0);
        test_tex.Bind(1);
        perlin_tex.Bind(2);
        mesh.Bind();
        mesh.Draw();

		m_imguiContext.BeginFrame();

		ImGui::Begin("aaga");
		ImGui::BulletText("Hellow world");
		ImGui::BulletText((const char*)glGetString(GL_RENDERER));
		ImGui::End();
		m_imguiContext.EndFrame();

		m_gameWindow.SwapBuffers();
	}
}
