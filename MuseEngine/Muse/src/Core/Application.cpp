#include "MusePCH.h"

#include "Core/Application.h"
#include "Window.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Utilities/Defines.h"

#include "GLFW/glfw3.h"

namespace Muse
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        ASSERT(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());

        m_Window->WindowCloseEvent.Subscribe( SUB_FN(Application::WindowCloseEvent));
        m_Window->WindowResizeEvent.Subscribe(SUB_FN(Application::WindowResizeEvent, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyPressedEvent.Subscribe(SUB_FN(Application::WindowResizeEvent, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyReleasedEvent.Subscribe(SUB_FN(Application::KeyReleasedEvent, std::placeholders::_1));
        m_Window->MouseButtonPressedEvent.Subscribe(SUB_FN(Application::MouseButtonPressedEvent, std::placeholders::_1));
        m_Window->MouseButtonReleasedEvent.Subscribe(SUB_FN(Application::MouseButtonReleasedEvent, std::placeholders::_1));
        m_Window->MouseScrolledEvent.Subscribe(SUB_FN(Application::MouseScrolledEvent, std::placeholders::_1, std::placeholders::_2));
        m_Window->MouseMovedEvent.Subscribe(SUB_FN(Application::MouseMovedEvent, std::placeholders::_1, std::placeholders::_2));

        Renderer::Init();

        m_SceneManager = std::make_shared<SceneManager>();
        m_ResourceManager = std::make_shared<ResourceManager>();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        SceneManager::DestroyAllGameObjects();

        m_Window->WindowCloseEvent.Unsubscribe(this);
        m_Window->WindowResizeEvent.Unsubscribe(this);
        m_Window->KeyPressedEvent.Unsubscribe(this);
        m_Window->KeyReleasedEvent.Unsubscribe(this);
        m_Window->MouseButtonPressedEvent.Unsubscribe(this);
        m_Window->MouseButtonReleasedEvent.Unsubscribe(this);
        m_Window->MouseScrolledEvent.Unsubscribe(this);
        m_Window->MouseMovedEvent.Unsubscribe(this);
    }

    void Application::Start()
    {
        OnStart();

        while (m_Running)
        {
            Update();
            //FixedUpdate();
            LateUpdate();
            ImGuiRender();
            Render();
        }
    }

    void Application::Update()
    {
        float time = static_cast<float>(glfwGetTime()); //Replace with: Platform::GetTime
        m_DeltaTime = time - m_LastFrameTime;
        m_LastFrameTime = time;


        for (Layer* layer : m_LayerStack)
        {
            layer->OnUpdate(m_DeltaTime);
        }

        m_UpdateEvent.Dispatch(m_DeltaTime);
        OnUpdate(m_DeltaTime);
    }

    void Application::FixedUpdate()
    {
        m_FixedUpdateEvent.Dispatch();
        OnFixedUpdate();
    }

    void Application::LateUpdate()
    {
        m_LateUpdateEvent.Dispatch(m_DeltaTime);
    }

    void Application::ImGuiRender()
    {
        m_ImGuiLayer->Begin();

        m_ImGuiRenderEvent.Dispatch();
        OnImGuiRender();
        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }

        m_ImGuiLayer->End();
    }

    void Application::Render()
    {
        m_Window->OnUpdate();

        m_RenderEvent.Dispatch();
        OnRender();
    }

    void Application::PushLayer(Layer* a_Layer)
    {
        m_LayerStack.PushLayer(a_Layer);
        a_Layer->OnAttach();
    }

    void Application::PushOverlay(Layer* a_Layer)
    {
        m_LayerStack.PushOverlay(a_Layer);
        a_Layer->OnAttach();
    }

    void Application::WindowCloseEvent()
    {
        m_Running = false;
        OnWindowCloseEvent();
    }

    void Application::WindowResizeEvent(int a_Width, int a_Height)
    {
        OnWindowResizeEvent(a_Width, a_Height);
    }

    void Application::KeyPressedEvent(int a_KeyCode, int a_RepeatCount)
    {
        OnKeyPressedEvent(a_KeyCode, a_RepeatCount);
    }

    void Application::KeyReleasedEvent(int a_KeyCode)
    {
        OnKeyReleasedEvent(a_KeyCode);
    }

    void Application::MouseButtonPressedEvent(int a_Button)
    {
        OnMouseButtonPressedEvent(a_Button);
    }

    void Application::MouseButtonReleasedEvent(int a_Button)
    {
        OnMouseButtonReleasedEvent(a_Button);
    }

    void Application::MouseScrolledEvent(float a_XOffset, float a_YOffset)
    {
        OnMouseScrolledEvent(a_XOffset, a_YOffset);
    }

    void Application::MouseMovedEvent(float a_X, float a_Y)
    {
        OnMouseMovedEvent(a_X, a_Y);
    }
}
