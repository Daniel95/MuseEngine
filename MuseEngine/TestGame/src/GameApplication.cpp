#include "GameApplication.h"
#include "Core/Engine.h"
#include "Core/Layer.h"
#include "Game.h"

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
	return new GameApplication();
}

void GameApplication::OnStart()
{
	PushLayer(new Game());
}

void GameApplication::OnUpdate(float deltaTime)
{
}

void GameApplication::OnFixedUpdate()
{
}

void GameApplication::OnRender()
{
}

void GameApplication::OnWindowCloseEvent()
{
}

void GameApplication::OnWindowResizeEvent(int a_Width, int a_Height)
{
}

void GameApplication::OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount)
{
}

void GameApplication::OnKeyReleasedEvent(int a_KeyCode)
{
}

void GameApplication::OnMouseButtonPressedEvent(int a_Button)
{
}

void GameApplication::OnMouseButtonReleasedEvent(int a_Button)
{
}

void GameApplication::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
{
}

void GameApplication::OnMouseMovedEvent(float a_X, float a_Y)
{
}
