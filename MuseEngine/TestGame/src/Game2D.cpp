#include "Game2D.h"

#include "Muse.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "PlayerComponent.h"

#include "Mode.h"

#if GAME_2D
#include "EntryPoint.h"
Muse::Application* Muse::CreateApplication()
{
	return new Game2D();
}
#endif

void Game2D::OnStart()
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

void Game2D::OnRender()
{
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::ResetStatistics();

    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());

    float gridWidth = 250;
    float gridHeight = 250;

    float margin = 1.5f;
    float size = 1.0f;

    m_GridCellRotation += 30 * Application::GetDeltaTime();

    int counter = 0;

    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {

            if (counter % 2 == 0)
            {
                Muse::Renderer2D::DrawQuad(
                    { x * margin, y * margin, 0 },
                    { size, size },
                    m_GridCellRotation,
                    { 1, 0.5f, 0, 1.0f }
                );
            }
            else
            {
                Muse::Renderer2D::DrawQuad(
                    { x * margin, y * margin, 0 },
                    { size, size },
                    m_GridCellRotation,
                    m_RaymanTexture
                );
            }

            counter++;
        }
        counter++;

    }

    /*
    const int amount = 50000;

    for (size_t i = 0; i < amount; i++)
    {
        Muse::Renderer2D::DrawQuad(
            { -1, 1, 0 },
            { 0.5f, 0.5f },
            45,
            { 1, 0.5f, 0, 1.0f }
        );
    }
    */

    /*
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
    */

    /*
    Muse::Renderer2D::DrawQuad(
        { 0, 1, 0 },
        glm::vec2(1),
        { 1.0f, 0.0f, 0.0f, 1.0f }
    );
    */

    Muse::Renderer2D::EndScene();
}

/*
void Game2D::OnRender()
{
    //Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::SetClearColor({ 0.9f, 0.9f, 0.9f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());

    std::shared_ptr<Muse::Scene> scene = Muse::SceneManager::GetActiveScene();

    for (auto& gameObject : Muse::SceneManager::GetActiveScene()->GetGameObjects())
    {
        auto meshComponent = gameObject->GetComponent<Muse::RenderComponent>();
        if(meshComponent != nullptr)
        {
            const Muse::Renderer2D::QuadPropertiesTransform quadPropertiesTransform(
                gameObject->GetTransform()->GetWorldModelMatrix(),
                { 0.8f, 0.2f, 0.3f, 1.0f });

            Muse::Renderer2D::DrawQuad(quadPropertiesTransform);
        }
    }

    const Muse::Renderer2D::QuadProperties quadProperties(
        { 0, 0, 0.1f },
        glm::vec2(1),
        45,
        //{ 0.5f, 0.5f, 0.5f, 0.5f },
        m_Texture);

    Muse::Renderer2D::DrawQuad(quadProperties);

	Muse::Renderer2D::EndScene();
}
*/

void Game2D::OnImGuiRender()
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