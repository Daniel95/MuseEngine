#include "Game.h"
#include "Core/Engine.h"
#include "Core/Layer.h"
#include "Core/ImGuiLayer.h"

class ExampleLayer : public Muse::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{ }

	void OnUpdate(Muse::Timestep ts) override
	{
		//LOG_INFO("ExampleLayer::Update");
	}
};



Muse::Application* Muse::CreateApplication()
{
	return new Game();
}

void Game::OnStart()
{
	PushLayer(new ExampleLayer());
	PushOverlay(new Muse::ImGuiLayer());
}

void Game::OnUpdate(float deltaTime)
{
}

void Game::OnFixedUpdate()
{
}

void Game::OnRender()
{
}

void Game::OnWindowCloseEvent()
{
}

void Game::OnWindowResizeEvent(int a_Width, int a_Height)
{
}

void Game::OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount)
{
}

void Game::OnKeyReleasedEvent(int a_KeyCode)
{
}

void Game::OnMouseButtonPressedEvent(int a_Button)
{
}

void Game::OnMouseButtonReleasedEvent(int a_Button)
{
}

void Game::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
{
}

void Game::OnMouseMovedEvent(float a_X, float a_Y)
{
}
