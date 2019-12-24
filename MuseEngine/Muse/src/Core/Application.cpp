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

#include <glad/glad.h>

namespace Muse
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        ASSERT(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;


        window = std::unique_ptr<Window>(Window::Create());
        window->WindowCloseEvent.Subscribe(this, std::bind(&Application::OnWindowCloseEvent, this));
        window->WindowResizeEvent.Subscribe(this, std::bind(&Application::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
        window->KeyPressedEvent.Subscribe(this, std::bind(&Application::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
        window->KeyReleasedEvent.Subscribe(this, std::bind(&Application::OnKeyReleasedEvent, this, std::placeholders::_1));
        window->MouseButtonPressedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonPressedEvent, this, std::placeholders::_1));
        window->MouseButtonReleasedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonReleasedEvent, this, std::placeholders::_1));
        window->MouseScrolledEvent.Subscribe(this, std::bind(&Application::OnMouseScrolledEvent, this, std::placeholders::_1, std::placeholders::_2));
        window->MouseMovedEvent.Subscribe(this, std::bind(&Application::OnMouseMovedEvent, this, std::placeholders::_1, std::placeholders::_2));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        systemManager = new SystemManager();
        systemManager->CreateSystem<ResourceSystem>();
        systemManager->CreateSystem<SoundSystem>();
        systemManager->CreateSystem<SceneSystem>(*this);

        // Vertex Array
        // Vertex Buffer
        // Index Buffer

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        glGenBuffers(1, &m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

        //Triangle
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

        //Create index buffer
        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

        unsigned int indices[3] = { 0, 1, 2 };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    Application::~Application()
    {
        window->WindowCloseEvent.Unsubscribe(this);
        window->WindowResizeEvent.Unsubscribe(this);
        window->KeyPressedEvent.Unsubscribe(this);
        window->KeyReleasedEvent.Unsubscribe(this);
        window->MouseButtonPressedEvent.Unsubscribe(this);
        window->MouseButtonReleasedEvent.Unsubscribe(this);
        window->MouseScrolledEvent.Unsubscribe(this);
        window->MouseMovedEvent.Unsubscribe(this);

        delete systemManager;
    }

    SystemManager* Application::GetSystemManager() const
    {
        return systemManager;
    }

	void Application::Start()
	{
        OnStart();

        while (running)
        {
            Update();
            FixedUpdate();
            Render();
        }
	}

	void Application::Update()
	{
        for (Layer* layer : layerStack)
        {
            layer->OnUpdate(0.16f);
        }

        systemManager->UpdateSystems(0);
        OnUpdate(0.016f);
	}

    void Application::FixedUpdate()
    {
        OnFixedUpdate();
    }

	void Application::Render()
	{
        m_ImGuiLayer->Begin();
        for (Layer* layer : layerStack)
        {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        window->OnUpdate();

        OnRender();
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(m_VertexArray);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}

    void Application::PushLayer(Layer* layer)
    {
        layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        layerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnWindowCloseEvent()
    {
        running = false;
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
