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
#include "Core/ECS/Entity/EntityDebugger.h"

#include "BulletHell/Job/PlayerJob.h"
#include "BulletHell/Job/EnemyJob.h"
#include "BulletHell/Job/DestroyOutOfBoundsJob.h"
#include "BulletHell/Job/ProjectileJob.h"
#include "BulletHell/Job/ObstacleCollisionJob.h"
#include "BulletHell/Job/HealthJob.h"
#include "BulletHell/Job/ScrollingJob.h"
#include "BulletHell/Job/ObstacleSpawnerJob.h"
#include "BulletHell/Job/BackgroundParticleSpawner.h"
#include "BulletHell/Component/Components.h"

#if GAME_BULLETHELL
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new BulletHell();
}
#endif

std::shared_ptr<Muse::Texture> BulletHell::s_PlayerTexture = nullptr;
std::shared_ptr<Muse::Texture> BulletHell::s_EnemyTexture = nullptr;
std::shared_ptr<Muse::Texture> BulletHell::s_ProjectileRedTexture = nullptr;
std::shared_ptr<Muse::Texture> BulletHell::s_ProjectileBlueTexture = nullptr;
std::shared_ptr<Muse::Texture> BulletHell::s_MeteorTexture = nullptr;
std::shared_ptr<Muse::Texture> BulletHell::s_Star1 = nullptr;
std::shared_ptr<Muse::Texture> BulletHell::s_Star2 = nullptr;
std::shared_ptr<Muse::Texture> BulletHell::s_Star3 = nullptr;

void BulletHell::OnStart()
{
    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr<Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("Game2DTestScene", scene);
    Muse::SceneManager::SwitchScene(scene);

    Muse::CameraComponent* cameraComponent = Muse::CameraComponent::GetMain();
    cameraComponent->SetZoomLevel(5);

    s_PlayerTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/playerShip3_blue.png");
    s_EnemyTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Enemies/enemyBlack3.png");
    s_MeteorTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Meteors/meteorBrown_med1.png");
    s_ProjectileRedTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Lasers/laserRed04.png");
    s_ProjectileBlueTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Lasers/laserBlue04.png");
    s_Star1 = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Effects/star1.png");
    s_Star2 = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Effects/star2.png");
    s_Star3 = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Effects/star3.png");

    //BulletHell
    Muse::ComponentManager<PlayerComponent>::Register("PlayerComponent");
    Muse::ComponentManager<ObstacleComponent>::Register("ObstacleComponent");
    Muse::ComponentManager<ProjectileComponent>::Register("ProjectileComponent");
    Muse::ComponentManager<DestroyOutOfBoundsComponent>::Register("DestroyOutOfBoundsComponent");
    Muse::ComponentManager<EnemyComponent>::Register("EnemyComponent");
    Muse::ComponentManager<ScrollingComponent>::Register("ScrollingComponent");

    GetJobManager()->Add<Muse::Render2DJob>(Muse::JobType::Renderer);
    GetJobManager()->Add<PlayerJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<HealthJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<ProjectileJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<DestroyOutOfBoundsJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<ObstacleCollisionJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<EnemyJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<ScrollingJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<ObstacleSpawnerJob>(Muse::JobType::Gameplay);
    GetJobManager()->Add<BackgroundParticleSpawner>(Muse::JobType::Gameplay);

    CreatePlayer();
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
    const std::unordered_map<int, std::vector<std::string>>& entityData = Muse::EntityDebugger::GetEntityData();

    for (const auto& pair : entityData)
    {
        ImGui::Spacing();
        Muse::EntityDebugger::GetEntityData();

        if (Muse::EntityDebugger::HasEntityName(pair.first))
        {
            ImGui::Text("Entity: %s", Muse::EntityDebugger::GetEntityName(pair.first).c_str());
        }
        else
        {
            ImGui::Text("Entity: unregistered");
        }

        ImGui::Text("Id: %d", pair.first);

        for (const auto& componentName : pair.second)
        {
            ImGui::Text("Component: %s", componentName.c_str());
        }
    }

    Muse::EntityDebugger::ClearEntityData();

    ImGui::End();
}

int BulletHell::CreatePlayer(const glm::vec2& a_Position)
{
    auto playerEntity = Muse::Entity::Create("Player");

    Muse::Render2DComponent render2DComponent
    {
        s_PlayerTexture,
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

int BulletHell::CreateMeteor(const glm::vec2& a_Position, const glm::vec2& a_Scale)
{
    auto meteorEntity = Muse::Entity::Create("Meteor");

    Muse::Render2DComponent render2DComponent
    {
        s_MeteorTexture,
    };

    Muse::TransformComponent transformComponent
    {
        glm::vec3(a_Position.x, a_Position.y, 0),
        glm::vec3(a_Scale.x, a_Scale.y, 0)
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(meteorEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(meteorEntity, transformComponent);
    Muse::ComponentManager<Muse::Collider2DComponent>::Add(meteorEntity, { });

    Muse::ComponentManager<MeteorComponent>::Add(meteorEntity, { });
    Muse::ComponentManager<ObstacleComponent>::Add(meteorEntity, { });
    Muse::ComponentManager<ScrollingComponent>::Add(meteorEntity, { });
    Muse::ComponentManager<DestroyOutOfBoundsComponent>::Add(meteorEntity, { });

    return meteorEntity;
}

int BulletHell::CreateEnemy(const glm::vec2& a_Position, const glm::vec2& a_Scale)
{
    auto enemyEntity = Muse::Entity::Create("Enemy");

    Muse::Render2DComponent render2DComponent
    {
        s_EnemyTexture,
    };

    Muse::TransformComponent transformComponent
    {
        glm::vec3(a_Position.x, a_Position.y, 0),
        glm::vec3(a_Scale.x, a_Scale.y, 0),
        glm::vec3(0, 0, glm::radians(180.0f))
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(enemyEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(enemyEntity, transformComponent);
    Muse::ComponentManager<Muse::Collider2DComponent>::Add(enemyEntity, { });

    Muse::ComponentManager<ObstacleComponent>::Add(enemyEntity, { });
    Muse::ComponentManager<EnemyComponent>::Add(enemyEntity, { });
    Muse::ComponentManager<DestroyOutOfBoundsComponent>::Add(enemyEntity, { });
    Muse::ComponentManager<ScrollingComponent>::Add(enemyEntity, { });

    return enemyEntity;
}

int BulletHell::CreateProjectile(Muse::TransformComponent& a_TransformComponent, const std::shared_ptr<Muse::Texture> a_Texture, float a_Speed)
{
    auto projectileEntity = Muse::Entity::Create("Projectile");

    Muse::Render2DComponent render2DComponent
    {
        a_Texture,
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(projectileEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(projectileEntity, a_TransformComponent);
    Muse::ComponentManager<Muse::Collider2DComponent>::Add(projectileEntity, { });

    Muse::ComponentManager<ObstacleComponent>::Add(projectileEntity, { });
    Muse::ComponentManager<ProjectileComponent>::Add(projectileEntity, { a_Speed });
    Muse::ComponentManager<DestroyOutOfBoundsComponent>::Add(projectileEntity, { });
    Muse::ComponentManager<ScrollingComponent>::Add(projectileEntity, { });

    return projectileEntity;
}

int BulletHell::CreateBackgroundParticle1(const glm::vec2& a_Position)
{
    Muse::TransformComponent transformComponent
    {
        glm::vec3(a_Position.x, a_Position.y, -0.1f),
        glm::vec3(0.2f, 0.2f, 0.2f)
    };

    return CreateBackgroundParticle(transformComponent, s_Star1, 1);
}

int BulletHell::CreateBackgroundParticle2(const glm::vec2& a_Position)
{
    Muse::TransformComponent transformComponent
    {
        glm::vec3(a_Position.x, a_Position.y, -0.1f),
        glm::vec3(0.2f, 0.2f, 0.2f)
    };

    return CreateBackgroundParticle(transformComponent, s_Star2, 1);
}

int BulletHell::CreateBackgroundParticle3(const glm::vec2& a_Position)
{
    Muse::TransformComponent transformComponent
    {
        glm::vec3(a_Position.x, a_Position.y, -0.1f),
        glm::vec3(0.15f, 0.15f, 0.15f)
    };

    return CreateBackgroundParticle(transformComponent, s_Star3, 1);
}

int BulletHell::CreateBackgroundParticle(Muse::TransformComponent& a_TransformComponent, const std::shared_ptr<Muse::Texture>& a_Texture, float a_ScrollingSpeed)
{
    auto particleEntity = Muse::Entity::Create("Particle");

    Muse::Render2DComponent render2DComponent
    {
        a_Texture,
    };

    Muse::ComponentManager<Muse::Render2DComponent>::Add(particleEntity, render2DComponent);
    Muse::ComponentManager<Muse::TransformComponent>::Add(particleEntity, a_TransformComponent);

    Muse::ComponentManager<DestroyOutOfBoundsComponent>::Add(particleEntity, { });
    Muse::ComponentManager<ScrollingComponent>::Add(particleEntity, { a_ScrollingSpeed });

    return particleEntity;
}
