#include "BulletHell.h"

#include "imgui/imgui.h"

#include "Muse.h"
#include "Mode.h"

#include "Core/ECS/Job/Job.h"
#include "Core/ECS/Job/JobManager.h"
#include "Core/ECS/Job/Render2DJob.h"
#include "Core/ECS/Component/Render2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"

#include "BulletHell/Job/PlayerJob.h"
#include "BulletHell/Job/HealthJob.h"
#include "BulletHell/Component/Components.h"

#if GAME_BULLETHELL
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new BulletHell();
}
#endif
#include <Core\Gameplay\Component\CameraComponent.cpp>

void BulletHell::OnStart()
{
    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("Game2DTestScene", scene);
    Muse::SceneManager::SwitchScene(scene);

    Muse::CameraComponent* cameraComponent = Muse::CameraComponent::GetMain();
    cameraComponent->SetZoomLevel(5);

    m_RaymanTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/rayman.png");
    m_CheckerboardTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Checkerboard.png");

    GetJobManager()->Add<Muse::Render2DJob>(Muse::JobType::Renderer);
    GetJobManager()->Add<PlayerJob>(Muse::JobType::Gameplay);

    auto playerEntity = Muse::Entity::Create();

    Muse::Render2DComponent render2DComponent
    {
        m_RaymanTexture,
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(playerEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(playerEntity, { });
    Muse::ComponentManager<PlayerComponent>::Add(playerEntity, { });

    auto transforms = Muse::ComponentManager<Muse::TransformComponent>::GetComponents();
    auto render2DComponents = Muse::ComponentManager<Muse::Render2DComponent>::GetComponents();
}

void BulletHell::OnRender()
{
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::ResetStatistics();

    Muse::Renderer2D::DrawQuad(
        { 1, 1, 0 },
        { 0.5f, 0.5f },
        0,
        { 0, 1, 0, 1 }
    );

    Muse::Renderer2D::DrawQuad(
        { 1, -1, 0 },
        { 0.5f, 0.5f },
        0,
        m_CheckerboardTexture
    );

    Muse::Renderer2D::DrawQuad(
        { -1, 1, 0 },
        { 0.5f, 0.5f },
        45,
        { 1, 0.5f, 0, 1.0f }
    );

    Muse::Renderer2D::DrawQuad(
        { -1.0f, -1.0f, 0 },
        { 0.5f, 0.5f },
        45,
        m_RaymanTexture
    );

    /*
    Muse::Renderer2D::DrawQuad(
        { 0, 1, 0 },
        glm::vec2(1),
        { 1.0f, 0.0f, 0.0f, 1.0f }
    );
    */
}


void BulletHell::OnImGuiRender()
{
    ImGui::Begin("Settings");

    auto stats = Muse::Renderer2D::GetStatistics();

    ImGui::Text("Renderer2D Statistics:");
    ImGui::Text("FPS: %f", 1 / GetDeltaTime());
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertex Count: %d", stats.GetTotalVertex());
    ImGui::Text("Index Count: %d", stats.GetTotalIndex());


    ImGui::End();
}