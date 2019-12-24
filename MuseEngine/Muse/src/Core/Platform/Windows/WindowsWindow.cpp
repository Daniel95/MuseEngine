#include "MusePCH.h"

#include "WindowsWindow.h"
#include "Core/Utilities/Defines.h"
#include "Core/Utilities/Log.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Muse
{
    static bool GLFWInitialized = false;

    static void GLFWErrorCallback(const int a_Error, const char* a_Description)
    {
        LOG_ENGINE_ERROR("GLFW Error ({0}): {1}", a_Error, a_Description);
    }

    Window* Window::Create(const WindowProperties& a_Properties)
    {
        return new WindowsWindow(a_Properties);
    }

    WindowsWindow::WindowsWindow(const WindowProperties& a_Properties)
    {
        Init(a_Properties);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProperties& a_Properties)
    {
        m_Title = a_Properties.Title;
        m_Width = a_Properties.Width;
        m_Height = a_Properties.Height;


        if(!GLFWInitialized)
        {
            const int success = glfwInit();

            ASSERT_ENGINE(success, "Could not initialize GLFW!");

            glfwSetErrorCallback(GLFWErrorCallback);
            GLFWInitialized = true;
        }


        m_Window = glfwCreateWindow(static_cast<int>(a_Properties.Width), static_cast<int>(a_Properties.Height), m_Title.c_str(), nullptr, nullptr);
        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, this);

        SetVSync(true);

        LOG_ENGINE_INFO("Created window {0}, ({1}, {2})", a_Properties.Title, a_Properties.Width, a_Properties.Height);

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowsWindow* windowsWindow = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
            windowsWindow->WindowCloseEvent.Dispatch();
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
            windowsWindow.WindowResizeEvent.Dispatch(width, height);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* a_Window, int a_Key, int a_Scancode, int a_Action, int a_Mods)
        {
            a_Scancode;
            a_Mods;

            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(a_Window));

            switch (a_Action)
            {
                case GLFW_PRESS:
                {
                    windowsWindow.KeyPressedEvent.Dispatch(a_Key, 0);
                    break;
                }
                case GLFW_RELEASE:
                {
                    windowsWindow.KeyReleasedEvent.Dispatch(a_Key);
                    break;
                }
                case GLFW_REPEAT:
                {
                    windowsWindow.KeyPressedEvent.Dispatch(a_Key, 1);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* a_Window, unsigned int a_KeyCode)
        {
            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(a_Window));
            windowsWindow.KeyTypedEvent.Dispatch(a_KeyCode);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* a_Window, int a_Button, int a_Action, int a_Mods)
        {
            a_Mods;

            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(a_Window));

            switch (a_Action)
            {
                case GLFW_PRESS:
                {
                    windowsWindow.MouseButtonPressedEvent.Dispatch(a_Button);
                    break;
                }
                case GLFW_RELEASE:
                {
                    windowsWindow.MouseButtonReleasedEvent.Dispatch(a_Button);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* a_Window, double a_XOffset, double a_YOffset)
        {
            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(a_Window));
            windowsWindow.MouseScrolledEvent.Dispatch(static_cast<float>(a_XOffset), static_cast<float>(a_YOffset));
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* a_Window, double a_XPos, double a_YPos)
        {
            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(a_Window));
            windowsWindow.MouseMovedEvent.Dispatch(static_cast<float>(a_XPos), static_cast<float>(a_YPos));
        });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();

        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool a_Enabled)
    {
        if(a_Enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        vSync = a_Enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return vSync;
    }
}
