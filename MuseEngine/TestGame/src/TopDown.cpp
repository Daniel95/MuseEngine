#include "TopDown.h"

#include "imgui/imgui.h"

#include "Muse.h"
#include "Mode.h"

#include "Core/ECS/Job/Job.h"
#include "Core/ECS/Job/JobManager.h"
#include "Core/ECS/Job/Render2DJob.h"
#include "Core/ECS/Component/Render2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/Gameplay/Component/CameraComponent.cpp"
#include "Core/ECS/Entity/EntityDebugger.h"

#if GAME_TOPDOWN
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new TopDown();
}
#endif

void TopDown::OnStart()
{
    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("Game2DTestScene", scene);
    Muse::SceneManager::SwitchScene(scene);

    Muse::CameraComponent* cameraComponent = Muse::CameraComponent::GetMain();
    cameraComponent->SetZoomLevel(5);


    m_SpriteSheet = Muse::ResourceManager::Load<Muse::Texture>("assets/topdown/kenneyrpgpack/Spritesheet/RPGpack_sheet_2X.png");
}

void TopDown::OnRender()
{
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();



    Muse::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 1 }, 0, m_SpriteSheet);

    Muse::Renderer2D::ResetStatistics();
}

void TopDown::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::Spacing();

    auto stats = Muse::Renderer2D::GetStatistics();

    ImGui::Text("Renderer2D Statistics:");
    ImGui::Text("FPS: %f", 1 / GetDeltaTime());
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertex Count: %d", stats.GetTotalVertex());
    ImGui::Text("Index Count: %d", stats.GetTotalIndex());

    ImGui::End();
}