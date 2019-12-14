#include "Game.h"
#include "Core/Engine.h"
#include "Core/System/Manager/SystemManager.h"

Muse::Application* Muse::CreateApplication()
{
	return new Game();
}

Game::Game()
{
}

Game::~Game()
{
}

void Game::OnStart()
{
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