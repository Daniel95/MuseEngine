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

        m_Window->WindowCloseEvent.Subscribe( SUB_FN(Application::OnWindowCloseEvent));
        m_Window->WindowResizeEvent.Subscribe(SUB_FN(Application::OnWindowResizeEvent, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyPressedEvent.Subscribe(SUB_FN(Application::OnWindowResizeEvent, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyReleasedEvent.Subscribe(SUB_FN(Application::OnKeyReleasedEvent, std::placeholders::_1));
        m_Window->MouseButtonPressedEvent.Subscribe(SUB_FN(Application::OnMouseButtonPressedEvent, std::placeholders::_1));
        m_Window->MouseButtonReleasedEvent.Subscribe(SUB_FN(Application::OnMouseButtonReleasedEvent, std::placeholders::_1));
        m_Window->MouseScrolledEvent.Subscribe(SUB_FN(Application::OnMouseScrolledEvent, std::placeholders::_1, std::placeholders::_2));
        m_Window->MouseMovedEvent.Subscribe(SUB_FN(Application::OnMouseMovedEvent, std::placeholders::_1, std::placeholders::_2));

        Renderer::Init();

        m_SceneManager = std::make_shared<SceneManager>();
        m_ResourceManager = std::make_shared<ResourceManager>();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
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
