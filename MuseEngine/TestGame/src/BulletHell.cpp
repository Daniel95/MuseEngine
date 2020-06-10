#include "BulletHell.h"

#include "Muse.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include "Mode.h"

#if GAME_BULLETHELL
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
    return new BulletHell();
}
#endif

void BulletHell::OnStart()
{
    Muse::RenderCommand::Init();
    Muse::Renderer2D::Init();

    std::shared_ptr <Muse::Scene> scene = Muse::Scene::Create();
    Muse::ResourceManager::Add("Game2DTestScene", scene);
    Muse::SceneManager::SwitchScene(scene);

    {
        auto gameObject = scene->AddGameObject();
        auto renderComponent = gameObject->AddComponent<Muse::RenderComponent>();
    }

    {
        auto gameObject = scene->AddGameObject();
        auto renderComponent = gameObject->AddComponent<Muse::RenderComponent>();
        gameObject->GetTransform()->SetLocalPosition({ 1.1f, 0, 0 });
    }

    {
        auto gameObject = scene->AddGameObject();
        auto renderComponent = gameObject->AddComponent<Muse::RenderComponent>();
        gameObject->GetTransform()->SetLocalPosition({ -1, -1, 0 });
        gameObject->GetTransform()->SetLocalScale({ 2, 1 });
    }

    m_RaymanTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/rayman.png");
    m_CheckerboardTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Checkerboard.png");

    std::string path = "assets/scenes/testscene.txt";
    scene->Save(path);

    Muse::ResourceManager::UnloadResource<Muse::Scene>(path);

    std::shared_ptr<Muse::Scene> loadedScene = Muse::ResourceManager::Load<Muse::Scene>(path);
    Muse::SceneManager::SwitchScene(loadedScene);
}

void BulletHell::OnRender()
{
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::ResetStatistics();

    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());

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

    Muse::Renderer2D::EndScene();
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