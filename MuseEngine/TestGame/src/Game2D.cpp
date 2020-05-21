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

    m_Texture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/rayman.png");

    std::string path = "assets/scenes/testscene.txt";
    scene->Save(path);

    Muse::ResourceManager::UnloadResource<Muse::Scene>(path);

    std::shared_ptr<Muse::Scene> loadedScene = Muse::ResourceManager::Load<Muse::Scene>(path);
    Muse::SceneManager::SwitchScene(loadedScene);
}

void Game2D::OnRender()
{
    //Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());

    //Muse::Renderer2D::DrawQuad(quadProperties);

    const Muse::Renderer2D::QuadProperties quadProperties(
        { 0, 0, 0.1f },
        glm::vec2(1),
        45,
        { 0.5f, 0.5f, 0.5f, 0.5f }
        //m_Texture
    );

    Muse::Renderer2D::DrawQuad(
        { 0.0f, 0.0f, 0 },
        { 0.5f, 0.5f },
        { 0.0f, 1.0f, 0.0f, 1.0f }
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
    ImGui::Begin("Test 1");
    ImGui::End();

    ImGui::Begin("Test 2");
    ImGui::End();

    ImGui::Begin("Test 3");
    ImGui::End();
} 