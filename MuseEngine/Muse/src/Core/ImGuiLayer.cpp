#include "MusePCH.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Core/Platform/OpenGL/imgui_impl_opengl3.h"
#include "Application.h"

//TEMP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Muse
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {

    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        //TODO: Make MUSE keycodes
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");

        Application::Get().GetWindow().WindowResizeEvent.Subscribe(this, std::bind(&ImGuiLayer::OnWindowResize, this, std::placeholders::_1, std::placeholders::_2));
        Application::Get().GetWindow().KeyPressedEvent.Subscribe(this, std::bind(&ImGuiLayer::OnWindowResize, this, std::placeholders::_1, std::placeholders::_2));
        Application::Get().GetWindow().KeyReleasedEvent.Subscribe(this, std::bind(&ImGuiLayer::OnKeyReleasedEvent, this, std::placeholders::_1));
        Application::Get().GetWindow().KeyTypedEvent.Subscribe(this, std::bind(&ImGuiLayer::OnKeyTyped, this, std::placeholders::_1));
        Application::Get().GetWindow().MouseButtonPressedEvent.Subscribe(this, std::bind(&ImGuiLayer::OnMouseButtonPressedEvent, this, std::placeholders::_1));
        Application::Get().GetWindow().MouseButtonReleasedEvent.Subscribe(this, std::bind(&ImGuiLayer::OnMouseButtonReleasedEvent, this, std::placeholders::_1));
        Application::Get().GetWindow().MouseScrolledEvent.Subscribe(this, std::bind(&ImGuiLayer::OnMouseScrolledEvent, this, std::placeholders::_1, std::placeholders::_2));
        Application::Get().GetWindow().MouseMovedEvent.Subscribe(this, std::bind(&ImGuiLayer::OnMouseMovedEvent, this, std::placeholders::_1, std::placeholders::_2));
    }

    void ImGuiLayer::OnDetach()
    {
        Application::Get().GetWindow().WindowCloseEvent.Unsubscribe(this);
        Application::Get().GetWindow().WindowResizeEvent.Unsubscribe(this);
        Application::Get().GetWindow().KeyPressedEvent.Unsubscribe(this);
        Application::Get().GetWindow().KeyReleasedEvent.Unsubscribe(this);
        Application::Get().GetWindow().MouseButtonPressedEvent.Unsubscribe(this);
        Application::Get().GetWindow().MouseButtonReleasedEvent.Unsubscribe(this);
        Application::Get().GetWindow().MouseScrolledEvent.Unsubscribe(this);
        Application::Get().GetWindow().MouseMovedEvent.Unsubscribe(this);
    }

    void ImGuiLayer::OnUpdate(Timestep ts)
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& application = Application::Get();
        io.DisplaySize = ImVec2(application.GetWindow().GetWidth(), application.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnMouseButtonPressedEvent(int a_Button)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[a_Button] = true;
    }

    void ImGuiLayer::OnMouseButtonReleasedEvent(int a_Button)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[a_Button] = false;
    }

    void ImGuiLayer::OnMouseMovedEvent(float a_X, float a_Y)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(a_X, a_Y);
    }

    void ImGuiLayer::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += a_XOffset;
        io.MouseWheel += a_YOffset;
    }

    void ImGuiLayer::KeyPressedEvent(int a_KeyCode, int a_RepeatCount)
    {
        a_RepeatCount;
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[a_KeyCode] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    }

    void ImGuiLayer::OnKeyReleasedEvent(int a_KeyCode)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[a_KeyCode] = false;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    }

    void ImGuiLayer::OnKeyTyped(int a_KeyCode)
    {
        ImGuiIO& io = ImGui::GetIO();
        if(a_KeyCode > 0 && a_KeyCode < 0x10000)
        {
            io.AddInputCharacter((unsigned short)a_KeyCode);
        }
    }

    void ImGuiLayer::OnWindowResize(int a_Width, int a_Height)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(a_Width, a_Height);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, a_Width, a_Height);

        LOG_ENGINE_INFO("OnWindowResize: {0}, {1}", a_Width, a_Height);
    }
}
