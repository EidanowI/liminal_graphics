#include "Engine/GameWindow/GameWindow.h"
#include "Engine/ImGuiContext/ImGuiContext.h"



class App {
public:
	App();

	App(const App&) = delete;
	App& operator=(const App&) = delete;
	App(App&&) = delete;
	App& operator=(App&&) = delete;

public:
	void Run();

private:
	Engine::GameWindow m_gameWindow;
	Engine::ImGuiContext m_imguiContext;
};