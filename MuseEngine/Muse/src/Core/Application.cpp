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

#include <glad/glad.h>

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

        // Vertex Array
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        // Vertex Buffer
        glGenBuffers(1, &m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

        //Triangle vertices
        float vertices[3 * 3] =
        {
            -0.5f, -0.5f, -0.0f,
            0.5f, -0.5f, -0.0f,
            0.0f, 0.5f, -0.0f,
        };

        //GL STATIC DRAW: vertices won't change.
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        // Index Buffer
        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

        //Triangle indices
        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            out vec3 v_Position;
            
            void main()
            {
                v_Position = a_Position;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";


        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
            }
        )";

        m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
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
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Shader->Bind();
        glBindVertexArray(m_VertexArray);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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
