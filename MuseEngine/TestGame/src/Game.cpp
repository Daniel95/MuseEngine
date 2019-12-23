#include "Game.h"
#include "imgui/imgui.h"

Game::Game()
    : Muse::Layer("Game Layer")
{
}

void Game::OnAttach()
{
}

void Game::OnDetach()
{
}

void Game::OnUpdate(Muse::Timestep ts)
{
}

void Game::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::Text("Hello World");
    ImGui::End();
}
