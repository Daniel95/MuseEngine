#include "MusePCH.h"

#include "Core/Application.h"
#include "Window.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Buffer/FrameBuffer.h"
#include "Scene/SceneManagerOld.h"
#include "Resource/ResourceManager.h"
#include "Utilities/Defines.h"
#include "Core/ECS/Job/JobManager.h"
#include "Core/ECS/Entity/EntityDebugger.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/ECS/Component/ComponentManager.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Collider2DComponent.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "glad/glad.h"
#include "Renderer/RenderCommand.h"
#include "Utilities/HardCodedMesh.h"
#include "Input/Input.h"


namespace Muse
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& a_Name)
    {
        MUSE_PROFILE_FUNCTION();

        std::srand(0);

        ASSERT_ENGINE(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;

        //Create Window
        {
            WindowProperties windowProperties;
            windowProperties.Width = 1280;
            windowProperties.Height = 720;
            windowProperties.Title = a_Name;
            m_Window = Window::Create(windowProperties);
        }

        //Subscribe to events
        {
            m_Window->WindowCloseEvent.Subscribe(SUB_FN(Application::WindowCloseEvent));
            m_Window->WindowResizeEvent.Subscribe(SUB_FN(Application::WindowResizeEvent, std::placeholders::_1, std::placeholders::_2));
            m_Window->KeyPressedEvent.Subscribe(SUB_FN(Application::KeyPressedEvent, std::placeholders::_1, std::placeholders::_2));
            m_Window->KeyReleasedEvent.Subscribe(SUB_FN(Application::KeyReleasedEvent, std::placeholders::_1));
            m_Window->MouseButtonPressedEvent.Subscribe(SUB_FN(Application::MouseButtonPressedEvent, std::placeholders::_1));
            m_Window->MouseButtonReleasedEvent.Subscribe(SUB_FN(Application::MouseButtonReleasedEvent, std::placeholders::_1));
            m_Window->MouseScrolledEvent.Subscribe(SUB_FN(Application::MouseScrolledEvent, std::placeholders::_1, std::placeholders::_2));
            m_Window->MouseMovedEvent.Subscribe(SUB_FN(Application::MouseMovedEvent, std::placeholders::_1, std::placeholders::_2));
        }

        //Initializing systems
        {
            Renderer::Init();


            m_SceneManager = std::make_shared<SceneManagerOld>();
            m_ResourceManager = std::make_shared<ResourceManager>();
            m_JobManager = std::make_shared<JobManager>();
            m_Input = std::make_shared<Input>();

            m_ImGuiLayer = new ImGuiLayer();
            PushOverlay(m_ImGuiLayer);
        }

        //FrameBufferProperties frameBufferSpecification;

        //frameBufferSpecification.Width = m_Window->GetWidth();
        //frameBufferSpecification.Height = m_Window->GetWidth();
        //m_ViewportFramebuffer = FrameBuffer::Create(frameBufferSpecification);

        //Engine Components
        ComponentManager<TransformComponent>::Register("TransformComponent");
        //ComponentManager<Render2DComponent>::Register("Render2DComponent");
        ComponentManager<Collider2DComponent>::Register("Collider2DComponent");
    }

    Application::~Application()
    {
        MUSE_PROFILE_FUNCTION();

        SceneManagerOld::DestroyAllGameObjects();

        m_Window->WindowCloseEvent.Unsubscribe(this);
        m_Window->WindowResizeEvent.Unsubscribe(this);
        m_Window->KeyPressedEvent.Unsubscribe(this);
        m_Window->KeyReleasedEvent.Unsubscribe(this);
        m_Window->MouseButtonPressedEvent.Unsubscribe(this);
        m_Window->MouseButtonReleasedEvent.Unsubscribe(this);
        m_Window->MouseScrolledEvent.Unsubscribe(this);
        m_Window->MouseMovedEvent.Unsubscribe(this);
    }

    void Application::Run()
    {
        MUSE_PROFILE_FUNCTION();

        OnStart();

        while (m_Running)
        {
            MUSE_PROFILE_SCOPE("Frame");

            if(!m_Minimized)
            {
                Update();
                //FixedUpdate();
                LateUpdate();
                //Render();
                ImGuiRender();
                m_Window->OnUpdate();
            }
            else
            {
                ImGuiRender();
                m_Window->OnUpdate();
            }
        }
    }

    void Application::Update()
    {
        MUSE_PROFILE_FUNCTION();

        float time = static_cast<float>(glfwGetTime()); //Replace with: Platform::GetTime
        m_DeltaTime = time - m_LastFrameTime;
        m_LastFrameTime = time;

        for (Layer* layer : m_LayerStack)
        {
            layer->OnUpdate(m_DeltaTime);
        }

        m_UpdateEvent.Dispatch(m_DeltaTime);
        m_JobManager->Update(JobType::Gameplay);
        OnUpdate(m_DeltaTime);
    }

    void Application::FixedUpdate()
    {
        MUSE_PROFILE_FUNCTION();
         
        m_FixedUpdateEvent.Dispatch();
        OnFixedUpdate();
    }

    void Application::LateUpdate()
    {
        MUSE_PROFILE_FUNCTION();

        m_LateUpdateEvent.Dispatch(m_DeltaTime);

        m_Input->UpdateMouseScrollDelta(0);
    }

    void Application::ImGuiRender()
    {
        MUSE_PROFILE_FUNCTION();

        m_ImGuiLayer->Begin();

        m_ImGuiRenderEvent.Dispatch();
        OnImGuiRender();
        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }

        /*
        Editor::StartDockSpace();


        FileBrowser::Render();

        m_ImGuiRenderEvent.Dispatch();
        OnImGuiRender();
        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }

        Editor::EndDockSpace();
        */

        m_ImGuiLayer->End();
    }

    void Application::Render()
    {
        MUSE_PROFILE_FUNCTION();

        //If Editor...
        //m_ViewportFramebuffer->Resize(GetWindow().GetWidth(), GetWindow().GetHeight());
        //m_ViewportFramebuffer->Resize(ViewPort::GetWidth(), GetWindow()->GetHeight());

        //m_ViewportFramebuffer->Bind();

        //Muse::Renderer2D::BeginScene(*Muse::CameraComponent::GetMain());
        //m_JobManager->Update(JobType::Renderer);

        //OnRender();

        //Muse::Renderer2D::EndScene();

        //m_ViewportFramebuffer->Unbind();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::PushLayer(Layer* a_Layer)
    {
        MUSE_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(a_Layer);
        a_Layer->OnAttach();
    }

    void Application::PushOverlay(Layer* a_Layer)
    {
        MUSE_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(a_Layer);
        a_Layer->OnAttach();
    }

    void Application::WindowCloseEvent()
    {
        MUSE_PROFILE_FUNCTION();

        m_Running = false;
        OnWindowCloseEvent();
    }

    void Application::WindowResizeEvent(int a_Width, int a_Height)
    {
        MUSE_PROFILE_FUNCTION();

        m_Minimized = a_Width == 0 || a_Height == 0;

        Renderer::OnWindowResize(a_Width, a_Height);

        OnWindowResizeEvent(a_Width, a_Height);
    }

    void Application::KeyPressedEvent(int a_KeyCode, int a_RepeatCount)
    {
        MUSE_PROFILE_FUNCTION();

        OnKeyPressedEvent(a_KeyCode, a_RepeatCount);
    }

    void Application::KeyReleasedEvent(int a_KeyCode)
    {
        MUSE_PROFILE_FUNCTION();

        OnKeyReleasedEvent(a_KeyCode);
    }

    void Application::MouseButtonPressedEvent(int a_Button)
    {
        MUSE_PROFILE_FUNCTION();

        OnMouseButtonPressedEvent(a_Button);
    }

    void Application::MouseButtonReleasedEvent(int a_Button)
    {
        MUSE_PROFILE_FUNCTION();

        OnMouseButtonReleasedEvent(a_Button);
    }

    void Application::MouseScrolledEvent(float a_XOffset, float a_YOffset)
    {
        MUSE_PROFILE_FUNCTION();

        m_Input->UpdateMouseScrollDelta(a_YOffset);
        OnMouseScrolledEvent(a_XOffset, a_YOffset);
    }

    void Application::MouseMovedEvent(float a_X, float a_Y)
    {
        MUSE_PROFILE_FUNCTION();

        OnMouseMovedEvent(a_X, a_Y);
    }
}
