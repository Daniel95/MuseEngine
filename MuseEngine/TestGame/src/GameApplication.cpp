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
#include "Core/Gameplay/Component/MeshComponent.h"
#include "PlayerComponent.h"
#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Muse::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{ }

	void OnUpdate(Muse::Timestep ts) override
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
    m_Scene = &Muse::SystemManager::Get().GetSystem<Muse::SceneSystem>().NewScene();

    /////////////////////////////////////////////////////////////////
    //// Player Red Square ////////////////////////////////////////////////
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

        Muse::GameObject& gameObject = m_Scene->AddGameObject();
        gameObject.AddComponent<PlayerComponent>();
        gameObject.AddComponent<Muse::MeshComponent>().SetMesh(vertices,
            3 * 4,
            indices,
            6,
            layout);
        gameObject.GetTransform()->SetPosition({ 0, 0, 0 });
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

        Muse::GameObject& gameObject = m_Scene->AddGameObject();
        gameObject.AddComponent<Muse::MeshComponent>().SetMesh(vertices, 
            3 * 7, 
            indices, 
            3, 
            layout);
        gameObject.GetTransform()->SetPosition({ -1, 0, 0 });
    }

    m_VertexColorShader.reset(Muse::Shader::Create(Muse::s_VertexSrc, Muse::s_FragmentSrc));
    m_FlatColorShader.reset(Muse::Shader::Create(Muse::s_FlatColorVertexSrc, Muse::s_FlatColorFragmentSrc));

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
    Muse::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Muse::RenderCommand::Clear();

    Muse::Renderer::BeginScene(*Muse::CameraComponent::GetMain());

    std::dynamic_pointer_cast<Muse::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_FlatShaderColor);

    for (auto& gameObject : m_Scene->GetGameObjects())
    {
        Muse::MeshComponent* meshComponent = gameObject->GetComponent<Muse::MeshComponent>();
        if(meshComponent != nullptr)
        {
            Muse::Renderer::Submit(m_FlatColorShader, meshComponent->GetVA(), gameObject->GetTransform()->GetModelMatrix());
        }
    }

	Muse::Renderer::EndScene();
}

void GameApplication::OnImGUIRender()
{
    Muse::GameObject* playerGameObject = m_Scene->FindGameObjectOfType<PlayerComponent>();

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
