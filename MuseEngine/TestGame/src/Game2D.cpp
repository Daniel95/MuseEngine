#include "Game2D.h"

#include "Muse.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "PlayerComponent.h"

#include "GameDefines.h"

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

    std::shared_ptr<Muse::Scene> scene = Muse::ResourceManager::Create<Muse::Scene>("NewScene");

    {
        Muse::GameObject& gameObject = scene->AddGameObject();
        Muse::RenderComponent& renderComponent = gameObject.AddComponent<Muse::RenderComponent>();
    }

    {
        Muse::GameObject& gameObject = scene->AddGameObject();
        Muse::RenderComponent& renderComponent = gameObject.AddComponent<Muse::RenderComponent>();
        gameObject.GetTransform()->SetPosition({ 1.1f, 0, 0 });
    }

    {
        Muse::GameObject& gameObject = scene->AddGameObject();
        Muse::RenderComponent& renderComponent = gameObject.AddComponent<Muse::RenderComponent>();
        gameObject.GetTransform()->SetPosition({ -1, -1, 0 });
        //BUG: setting scale will move the object.
        //gameObject.GetTransform()->SetScale({ 3, 1, 1 });

    }

    m_Texture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/rayman.png");
}

void Game2D::OnRender()
{
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());

    for (auto& gameObject : Muse::SceneManager::GetActiveScene()->GetGameObjects())
    {
        Muse::RenderComponent* meshComponent = gameObject->GetComponent<Muse::RenderComponent>();
        if(meshComponent != nullptr)
        {

            Muse::Renderer2D::DrawQuad(gameObject->GetTransform()->GetModelMatrix(), { 0.8f, 0.2f, 0.3f, 1.0f });
        }
    }

    Muse::Renderer2D::DrawQuad({ 0, 0, 0.1f }, {1, 1}, m_Texture);

	Muse::Renderer2D::EndScene();
}

void Game2D::OnImGuiRender()
{

} 