#include "GameApplication.h"
#include "Core/Engine.h"
#include "Game.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Buffer/BufferLayout.h"
#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Core/Renderer/Buffer/IndexBuffer.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Gameplay/Component/MeshComponent.h"

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

        Muse::GameObject& triangleGameObject = m_Scene->AddGameObject();
        triangleGameObject.AddComponent<Muse::MeshComponent>().SetMesh(vertices, 
            3 * 7, 
            indices, 
            3, 
            layout);
        triangleGameObject.GetTransform()->SetPosition({ 3, 0, 0 });
    }

    /////////////////////////////////////////////////////////////////
    //// Blue Square ////////////////////////////////////////////////
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

        Muse::GameObject& squareGameObject = m_Scene->AddGameObject();
        squareGameObject.AddComponent<Muse::MeshComponent>().SetMesh(vertices,
            3 * 4,
            indices,
            6,
            layout);
        squareGameObject.GetTransform()->SetPosition({ -1, 0, 0 });
    }

    m_Shader = std::make_unique<Muse::Shader>(Muse::s_VertexSrc, Muse::s_FragmentSrc);
    m_BlueShader = std::make_unique<Muse::Shader>(Muse::s_BlueVertexSrc, Muse::s_BlueFragmentSrc);

	//PushLayer(new Game());
}

void GameApplication::OnUpdate(float a_DeltaTime)
{
    if(Muse::Input::IsKeyPressed(MUSE_KEY_A))
    {
        Muse::CameraComponent::GetMain()->GetTransform()->Move(glm::vec2(-m_CameraMoveSpeed * a_DeltaTime, 0.0f));
    }
    else if (Muse::Input::IsKeyPressed(MUSE_KEY_D))
    {
        Muse::CameraComponent::GetMain()->GetTransform()->Move(glm::vec2(m_CameraMoveSpeed * a_DeltaTime, 0.0f));
    }

    if (Muse::Input::IsKeyPressed(MUSE_KEY_S))
    {
        Muse::CameraComponent::GetMain()->GetTransform()->Move(glm::vec2(0.0f, -m_CameraMoveSpeed * a_DeltaTime));
    }
    else if (Muse::Input::IsKeyPressed(MUSE_KEY_W))
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

    for (auto& gameObject : m_Scene->GetGameObjects())
    {
        Muse::MeshComponent* meshComponent = gameObject->GetComponent<Muse::MeshComponent>();
        if(meshComponent != nullptr)
        {
            Muse::Renderer::Submit(m_Shader, meshComponent->GetVA(), gameObject->GetTransform()->GetModelMatrix());
        }
    }

	//Muse::Renderer::Submit(m_BlueShader, m_SquareVA);
	//Muse::Renderer::Submit(m_Shader, m_TriangleVA);

	Muse::Renderer::EndScene();
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
