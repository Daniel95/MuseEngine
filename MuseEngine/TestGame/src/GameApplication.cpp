#include "GameApplication.h"
#include "Core/Engine.h"
#include "Game.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Buffer/BufferLayout.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Gameplay/Component/RenderComponent.h"
#include "PlayerComponent.h"
#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Scene.h"
#include "Core/Resource/ResourceManager.h"

class ExampleLayer : public Muse::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{ }

	void OnUpdate(float a_DeltaTime) override
	{
		//LOG_INFO("ExampleLayer::Update");
	}
};

Muse::Application* Muse::CreateApplication()
{
	return new GameApplication();
}

void GameApplication::OnStart()
{
    std::shared_ptr<Muse::Scene> scene = Muse::SceneManager::NewScene();

    std::shared_ptr<Muse::Shader> textureShader = Muse::ResourceManager::Load<Muse::Shader>("assets/shaders/Texture.glsl");
    std::shared_ptr<Muse::Shader> vertexColorShader = Muse::ResourceManager::Load<Muse::Shader>("assets/shaders/VertexColor.glsl");
    std::shared_ptr<Muse::Shader> flatColorShader = Muse::ResourceManager::Load<Muse::Shader>("assets/shaders/FlatColor.glsl");

    std::shared_ptr<Muse::Texture> checkerboardTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Checkerboard.png");
    std::shared_ptr<Muse::Texture> raymanTexture = Muse::ResourceManager::Load<Muse::Texture>("assets/textures/Rayman.png");

    /////////////////////////////////////////////////////////////////
    //// Checkerboard ///////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {
        float vertices[5 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        };
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        const Muse::BufferLayout layout =
        {
            { Muse::ShaderDataType::Float3, "a_Position" },
            { Muse::ShaderDataType::Float2, "a_TexCoord" },
        };

        Muse::GameObject& gameObject = scene->AddGameObject();
        Muse::RenderComponent& renderComponent = gameObject.AddComponent<Muse::RenderComponent>();

        renderComponent.SetMesh(vertices,
            5 * 4,
            indices,
            6,
            layout);
        renderComponent.SetShader(textureShader);
        renderComponent.SetTexture(checkerboardTexture);

        std::dynamic_pointer_cast<Muse::OpenGLShader>(textureShader)->Bind();
        std::dynamic_pointer_cast<Muse::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
    }

    /////////////////////////////////////////////////////////////////
    //// Dynamic Color Square ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {
        float vertices[3 * 4] =
        {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
        };
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        const Muse::BufferLayout layout =
        {
            { Muse::ShaderDataType::Float3, "a_Position" },
        };

        Muse::GameObject& gameObject = scene->AddGameObject();
        Muse::RenderComponent& renderComponent = gameObject.AddComponent<Muse::RenderComponent>();

        renderComponent.SetMesh(vertices,
            3 * 4,
            indices,
            6,
            layout);
        renderComponent.SetShader(flatColorShader);

        gameObject.GetTransform()->SetPosition({ 1.1f, 0, 0 });
    }

    /////////////////////////////////////////////////////////////////
    //// Triangle ///////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////    
    {
        float vertices[3 * 7]
        {
            -0.5f, -0.5f, -0.0f, 0.9f, 0.9f, 0.1f, 0.6f,
            0.5f, -0.5f, -0.0f, 0.1f, 0.6f, 0.2f, 0.8f,
            0.0f, 0.5f, -0.0f, 0.2, 0.3, 0.8, 1.0,
        };
        uint32_t indices[3] = { 0, 1, 2 };

        const Muse::BufferLayout layout =
        {
            { Muse::ShaderDataType::Float3, "a_Position" },
            { Muse::ShaderDataType::Float4, "a_Color" }
        };

        Muse::GameObject& gameObject = scene->AddGameObject();
        Muse::RenderComponent& renderComponent = gameObject.AddComponent<Muse::RenderComponent>();

        renderComponent.SetMesh(vertices,
            3 * 7, 
            indices, 
            3, 
            layout);

        renderComponent.SetShader(vertexColorShader);


        gameObject.GetTransform()->SetPosition({ -1, 0, 0 });
    }

    /////////////////////////////////////////////////////////////////
    //// Player Textured Square /////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    {
        float vertices[5 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        };
        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        const Muse::BufferLayout layout =
        {
            { Muse::ShaderDataType::Float3, "a_Position" },
            { Muse::ShaderDataType::Float2, "a_TexCoord" },
        };

        Muse::GameObject& gameObject = scene->AddGameObject();
        gameObject.AddComponent<PlayerComponent>();
        Muse::RenderComponent& renderComponent = gameObject.AddComponent<Muse::RenderComponent>();

        renderComponent.SetMesh(vertices,
            5 * 4,
            indices,
            6,
            layout);
        renderComponent.SetShader(textureShader);
        renderComponent.SetTexture(raymanTexture);

        std::dynamic_pointer_cast<Muse::OpenGLShader>(textureShader)->Bind();
        std::dynamic_pointer_cast<Muse::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
        gameObject.GetTransform()->SetPosition({ 0, 0, 1 });
    }

	//PushLayer(new Game());
}

void GameApplication::OnUpdate(float a_DeltaTime)
{
    if(Muse::Input::IsKeyPressed(MUSE_KEY_LEFT))
    {
        Muse::CameraComponent::GetMain()->GetTransform()->Move(glm::vec2(-m_CameraMoveSpeed * a_DeltaTime, 0.0f));
    }
    else if (Muse::Input::IsKeyPressed(MUSE_KEY_RIGHT))
    {
        Muse::CameraComponent::GetMain()->GetTransform()->Move(glm::vec2(m_CameraMoveSpeed * a_DeltaTime, 0.0f));
    }

    if (Muse::Input::IsKeyPressed(MUSE_KEY_DOWN))
    {
        Muse::CameraComponent::GetMain()->GetTransform()->Move(glm::vec2(0.0f, -m_CameraMoveSpeed * a_DeltaTime));
    }
    else if (Muse::Input::IsKeyPressed(MUSE_KEY_UP))
    {
        Muse::CameraComponent::GetMain()->GetTransform()->Move(glm::vec2(0.0f, m_CameraMoveSpeed * a_DeltaTime));
    }
}

void GameApplication::OnFixedUpdate()
{
    
}

void GameApplication::OnRender()
{
    Muse::Renderer::Init();

    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer::BeginScene(*Muse::CameraComponent::GetMain());

    std::shared_ptr<Muse::Shader> flatColorShader = Muse::ResourceManager::Get<Muse::Shader>("assets/shaders/FlatColor.glsl");
    flatColorShader->Bind();
    std::dynamic_pointer_cast<Muse::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_FlatShaderColor);

    for (auto& gameObject : Muse::SceneManager::GetActiveScene()->GetGameObjects())
    {
        Muse::RenderComponent* meshComponent = gameObject->GetComponent<Muse::RenderComponent>();
        if(meshComponent != nullptr)
        {
            if(meshComponent->GetTexture() != nullptr)
            {
                meshComponent->GetTexture()->Bind();
            }
            Muse::Renderer::Submit(meshComponent->GetShader(), meshComponent->GetVA(), gameObject->GetTransform()->GetModelMatrix());
        }
    }

	Muse::Renderer::EndScene();
}

void GameApplication::OnImGuiRender()
{
    Muse::GameObject* playerGameObject = Muse::SceneManager::GetActiveScene()->FindGameObjectOfType<PlayerComponent>();

    if (playerGameObject == nullptr) { return; }

    PlayerComponent* playerComponent = playerGameObject->GetComponent<PlayerComponent>();

    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_FlatShaderColor));
    ImGui::End();
}

void GameApplication::OnWindowCloseEvent()
{
}

void GameApplication::OnWindowResizeEvent(int a_Width, int a_Height)
{
}

void GameApplication::OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount)
{
}

void GameApplication::OnKeyReleasedEvent(int a_KeyCode)
{
}

void GameApplication::OnMouseButtonPressedEvent(int a_Button)
{
}

void GameApplication::OnMouseButtonReleasedEvent(int a_Button)
{
}

void GameApplication::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
{
}

void GameApplication::OnMouseMovedEvent(float a_X, float a_Y)
{
}
