#include "MusePCH.h"

#include "Core/Application.h"
#include "Window.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Buffer/FrameBuffer.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Utilities/Defines.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "glad/glad.h"
#include "Gameplay/Component/CameraComponent.h"


namespace Muse
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        MUSE_PROFILE_FUNCTION();

        ASSERT_ENGINE(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());

        m_Window->WindowCloseEvent.Subscribe( SUB_FN(Application::WindowCloseEvent));
        m_Window->WindowResizeEvent.Subscribe(SUB_FN(Application::WindowResizeEvent, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyPressedEvent.Subscribe(SUB_FN(Application::KeyPressedEvent, std::placeholders::_1, std::placeholders::_2));
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

        m_Framebuffer = FrameBuffer::Create(m_Window->GetWidth(), m_Window->GetWidth(), FramebufferFormat::RGBA16F);
    }

    Application::~Application()
    {
        MUSE_PROFILE_FUNCTION();

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
                ImGuiRender();
                m_Window->OnUpdate();
                Render();
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
        //Profiling UI
        ImGui::Begin("Profiling");
        for (auto& result : m_ProfileResults)
        {
            char label[50];
            strcpy(label, "%.3fms ");
            strcat(label, result.m_Name);

            ImGui::Text(label, result.m_End - result.m_Start);
        }
        m_ProfileResults.clear();
        ImGui::End();
        */





        ///Start ViewPort
        static bool p_open = true;

        static bool opt_fullscreen_persistant = true;
        static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
        bool opt_fullscreen = opt_fullscreen_persistant;




        ImGui::Begin("Viewport");

        /*float posX = ImGui::GetCursorScreenPos().x;
        float posY = ImGui::GetCursorScreenPos().y;

        auto [wx, wy] = Application::Get().GetWindow().GetWindowPos();
        posX -= wx;
        posY -= wy;
        HZ_INFO("{0}, {1}", posX, posY);*/

        auto viewportSize = ImGui::GetContentRegionAvail();
        m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        //CameraComponent::GetMain()->SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f));
        //CameraComponent::GetMain()->SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), viewportSize, { 0, 1 }, { 1, 0 });
        ImGui::End();
        /*
        ImGui::PopStyleVar();

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Docking"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows, 
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 opt_flags ^= ImGuiDockNodeFlags_NoSplit;
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))                opt_flags ^= ImGuiDockNodeFlags_NoResize;
                if (ImGui::MenuItem("Flag: PassthruDockspace", "", (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))       opt_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (opt_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          opt_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                ImGui::Separator();
                if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
                    p_open = false;
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        */


        ///End ViewPort




        m_ImGuiLayer->End();
    }

    void Application::Render()
    {
        InstrumentationTimer timer("Application::Render");

        m_RenderEvent.Dispatch();
        OnRender();
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

        OnMouseScrolledEvent(a_XOffset, a_YOffset);
    }

    void Application::MouseMovedEvent(float a_X, float a_Y)
    {
        MUSE_PROFILE_FUNCTION();

        OnMouseMovedEvent(a_X, a_Y);
    }
}
