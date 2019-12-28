#include "Game.h"
#include "imgui/imgui.h"
#include "Core/System/Scene/Scene.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/System/SceneSystem.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/CameraComponent.h"

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
