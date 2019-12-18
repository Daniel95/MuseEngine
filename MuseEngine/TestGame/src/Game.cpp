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

	void OnEvent(Muse::EventOld& event) override
	{
		LOG_INFO("{0}", event.ToString());
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