#include "MusePCH.h"

#include "Core/Application.h"

#include "Core/System/ResourceSystem.h"
#include "Core/System/SceneSystem.h"
#include "Core/System/SoundSystem.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Core/Input/Input.h"
#include "Core/Input/MouseButtonCodes.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Renderer/Shader.h"
#include "Renderer/Buffer/VertexBuffer.h"
#include "Renderer/Buffer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/RendererAPI.h"

#include "Renderer/Buffer/BufferLayout.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

namespace Muse
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        ASSERT(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->WindowCloseEvent.Subscribe(this, std::bind(&Application::OnWindowCloseEvent, this));
        m_Window->WindowResizeEvent.Subscribe(this, std::bind(&Application::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyPressedEvent.Subscribe(this, std::bind(&Application::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyReleasedEvent.Subscribe(this, std::bind(&Application::OnKeyReleasedEvent, this, std::placeholders::_1));
        m_Window->MouseButtonPressedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonPressedEvent, this, std::placeholders::_1));
        m_Window->MouseButtonReleasedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonReleasedEvent, this, std::placeholders::_1));
        m_Window->MouseScrolledEvent.Subscribe(this, std::bind(&Application::OnMouseScrolledEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->MouseMovedEvent.Subscribe(this, std::bind(&Application::OnMouseMovedEvent, this, std::placeholders::_1, std::placeholders::_2));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_SystemManager = new SystemManager();
        m_SystemManager->CreateSystem<ResourceSystem>();
        m_SystemManager->CreateSystem<SoundSystem>();
        m_SystemManager->CreateSystem<SceneSystem>(*this);

        /////////////////////////////////////////////////////////////////
        //// Triangle ///////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////    

        m_VertexArray.reset(VertexArray::Create());

        //Triangle vertices
        float vertices[3 * 7] =
        {
            -0.5f, -0.5f, -0.0f, 0.9f, 0.9f, 0.1f, 0.6f,
            0.5f, -0.5f, -0.0f, 0.1f, 0.6f, 0.2f, 0.8f,
            0.0f, 0.5f, -0.0f, 0.2, 0.3, 0.8, 1.0,
        };
        uint32_t indices[3] = { 0, 1, 2 };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        m_VertexBuffer->Bind();

        const BufferLayout layout =
        {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
        };
        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        const uint32_t count = sizeof(indices) / sizeof(uint32_t);
        m_IndexBuffer.reset(IndexBuffer::Create(indices, count));
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        /////////////////////////////////////////////////////////////////
        //// Blue Square ////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////

        float squareVertices[3 * 4] =
        {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f, 
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,

        };
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

        m_SquareVA.reset(VertexArray::Create());

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout(
        {
            { ShaderDataType::Float3, "a_Position" },
        });
        m_SquareVA->AddVertexBuffer(squareVB);

        
        std::shared_ptr<IndexBuffer> squareIB;
        const uint32_t squareCount = sizeof(squareIndices) / sizeof(uint32_t);
        squareIB.reset(IndexBuffer::Create(squareIndices, squareCount));
        m_SquareVA->SetIndexBuffer(squareIB);

        m_Shader = std::make_unique<Shader>(s_VertexSrc, s_FragmentSrc);
        m_BlueShader = std::make_unique<Shader>(s_BlueVertexSrc, s_BlueFragmentSrc);
    }

    Application::~Application()
    {
        m_Window->WindowCloseEvent.Unsubscribe(this);
        m_Window->WindowResizeEvent.Unsubscribe(this);
        m_Window->KeyPressedEvent.Unsubscribe(this);
        m_Window->KeyReleasedEvent.Unsubscribe(this);
        m_Window->MouseButtonPressedEvent.Unsubscribe(this);
        m_Window->MouseButtonReleasedEvent.Unsubscribe(this);
        m_Window->MouseScrolledEvent.Unsubscribe(this);
        m_Window->MouseMovedEvent.Unsubscribe(this);

        delete m_SystemManager;
    }

    void Application::Start()
    {
        OnStart();

        while (m_Running)
        {
            Update();
            FixedUpdate();
            Render();
        }
    }

    void Application::Update()
    {
        for (Layer* layer : m_LayerStack)
        {
            layer->OnUpdate(0.16f);
        }

        m_SystemManager->UpdateSystems(0);
        OnUpdate(0.016f);
    }

    void Application::FixedUpdate()
    {
        OnFixedUpdate();
    }

    void Application::Render()
    {
        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        m_Window->OnUpdate();

        OnRender();

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();

        Renderer::BeginScene();

        m_BlueShader->Bind();
        Renderer::Submit(m_SquareVA);

        m_Shader->Bind();
        Renderer::Submit(m_VertexArray);

        Renderer::EndScene();
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnWindowCloseEvent()
    {
        m_Running = false;
    }

    void Application::OnWindowResizeEvent(int a_Width, int a_Height)
    {
        //LOG_ENGINE_INFO("OnWindowResize: {0}, {1}", a_Width, a_Height);
    }

    void Application::OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount)
    {
        //LOG_ENGINE_INFO("OnKeyPressed: {0}, {1}", a_KeyCode, a_RepeatCount);
    }

    void Application::OnKeyReleasedEvent(int a_KeyCode)
    {
        //LOG_ENGINE_INFO("OnKeyReleased: {0}", a_KeyCode);
    }

    void Application::OnMouseButtonPressedEvent(int a_Button)
    {
        //LOG_ENGINE_INFO("OnMouseButtonPressed: {0}", a_Button);
    }

    void Application::OnMouseButtonReleasedEvent(int a_Button)
    {
        //LOG_ENGINE_INFO("OnMouseButtonReleased: {0}", a_Button);
    }

    void Application::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
    {
        //LOG_ENGINE_INFO("OnMouseScrolled: {0}, {1}", a_XOffset, a_YOffset);
    }

    void Application::OnMouseMovedEvent(float a_X, float a_Y)
    {
        //LOG_ENGINE_INFO("OnMouseMoved: {0}, {1}", a_X, a_Y);
    }
}
