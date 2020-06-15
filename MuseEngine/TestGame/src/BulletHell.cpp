#include "BulletHell.h"

#include "imgui/imgui.h"

#include "Muse.h"
#include "Mode.h"

#include "Core/ECS/Job/Job.h"
#include "Core/ECS/Job/JobManager.h"
#include "Core/ECS/Job/Render2DJob.h"
#include "Core/ECS/Component/Render2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/Gameplay/Component/CameraComponent.cpp"

#include "BulletHell/Job/PlayerJob.h"
#include "BulletHell/Job/EnemyJob.h"
#include "BulletHell/Job/DestroyOutOfBoundsJob.h"
#include "BulletHell/Job/ProjectileJob.h"
#include "BulletHell/Job/ObstacleCollisionJob.h"
#include "BulletHell/Job/HealthJob.h"
#include "BulletHell/Component/Components.h"

#if GAME_BULLETHELL
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new BulletHell();
}
#endif

std::shared_ptr<Muse::Texture> BulletHell::s_RaymanTexture = nullptr;
std::shared_ptr<Muse::Texture> BulletHell::s_CheckerboardTexture = nullptr;

void BulletHell::OnStart()
{
    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("Game2DTestScene", scene);
    Muse::SceneManager::SwitchScene(scene);

    Muse::CameraComponent* cameraComponent = Muse::CameraComponent::GetMain();
    cameraComponent->SetZoomLevel(5);

    s_RaymanTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/rayman.png");
    s_CheckerboardTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Checkerboard.png");

    GetJobManager()->Add<Muse::Render2DJob>(Muse::JobType::Renderer);
    GetJobManager()->Add<PlayerJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<HealthJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<ProjectileJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<DestroyOutOfBoundsJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<ObstacleCollisionJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<EnemyJob>(Muse::JobType::Gameplay);

    CreatePlayer({ 0, 0 });
    CreateEnemy({ 2, 2 });
    CreateObstacle({ -2, 2 });
}

void BulletHell::OnRender()
{
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::ResetStatistics();
}

void BulletHell::OnImGuiRender()
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

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Entity Data:");

    //Muse components:
    ImGui::Spacing();
    ImGui::Text("Muse Components:");

    ImGui::Text("Render2DComponent Count: %d", Muse::ComponentManager<Muse::Render2DComponent>::GetComponents().size());
    ImGui::Text("TransformComponent Count: %d", Muse::ComponentManager<Muse::TransformComponent>::GetComponents().size());
    ImGui::Text("Collider2DComponent Count: %d", Muse::ComponentManager<Muse::Collider2DComponent>::GetComponents().size());

    //BulletHell Components
    ImGui::Spacing();
    ImGui::Text("BulletHell Components:");

    ImGui::Text("PlayerComponent Count: %d", Muse::ComponentManager<PlayerComponent>::GetComponents().size());
    ImGui::Text("ObstacleComponent Count: %d", Muse::ComponentManager<ObstacleComponent>::GetComponents().size());
    ImGui::Text("MoveForwardComponent Count: %d", Muse::ComponentManager<ProjectileComponent>::GetComponents().size());

    ImGui::End();
}

int BulletHell::CreatePlayer(const glm::vec2& a_Position)
{
    auto playerEntity = Muse::Entity::Create();

    Muse::Render2DComponent render2DComponent
    {
        s_RaymanTexture,
    };

    Muse::TransformComponent transformComponent
    {
        glm::vec3(a_Position.x, a_Position.y, 0)
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(playerEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(playerEntity, transformComponent);
    Muse::ComponentManager<Muse::Collider2DComponent>::Add(playerEntity, { });

    Muse::ComponentManager<PlayerComponent>::Add(playerEntity, { });

    return playerEntity;
}

int BulletHell::CreateObstacle(const glm::vec2& a_Position, const glm::vec2& a_Scale)
{
    auto obstacleEntity = Muse::Entity::Create();

    Muse::Render2DComponent render2DComponent
    {
        s_CheckerboardTexture,
    };

    Muse::TransformComponent transformComponent
    {
        glm::vec3(a_Position.x, a_Position.y, 0),
        glm::vec3(a_Scale.x, a_Scale.y, 0)
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(obstacleEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(obstacleEntity, transformComponent);
    Muse::ComponentManager<Muse::Collider2DComponent>::Add(obstacleEntity, { });

    Muse::ComponentManager<ObstacleComponent>::Add(obstacleEntity, { });

    return obstacleEntity;
}

int BulletHell::CreateEnemy(const glm::vec2& a_Position, const glm::vec2& a_Scale)
{
    auto obstacleEntity = Muse::Entity::Create();

    Muse::Render2DComponent render2DComponent
    {
        s_RaymanTexture,
    };

    Muse::TransformComponent transformComponent
    {
        glm::vec3(a_Position.x, a_Position.y, 0),
        glm::vec3(a_Scale.x, a_Scale.y, 0),
        glm::vec3(0, 0, glm::radians(180.0f))
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(obstacleEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(obstacleEntity, transformComponent);
    Muse::ComponentManager<Muse::Collider2DComponent>::Add(obstacleEntity, { });

    Muse::ComponentManager<ObstacleComponent>::Add(obstacleEntity, { });
    Muse::ComponentManager<EnemyComponent>::Add(obstacleEntity, { });

    return obstacleEntity;
}

int BulletHell::CreateProjectile(Muse::TransformComponent& a_TransformComponent, float a_Speed)
{
    auto projectileEntity = Muse::Entity::Create();

    Muse::Render2DComponent render2DComponent
    {
        s_CheckerboardTexture,
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(projectileEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(projectileEntity, a_TransformComponent);
    Muse::ComponentManager<Muse::Collider2DComponent>::Add(projectileEntity, { });

    Muse::ComponentManager<ObstacleComponent>::Add(projectileEntity, { });
    Muse::ComponentManager<ProjectileComponent>::Add(projectileEntity, { 12 });
    Muse::ComponentManager<DestroyOutOfBoundsComponent>::Add(projectileEntity, { });

    return projectileEntity;
}
