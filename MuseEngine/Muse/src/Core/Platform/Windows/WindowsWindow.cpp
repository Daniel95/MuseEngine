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

    static void GLFWErrorCallback(int error, const char* description)
    {
        LOG_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProperties& properties)
    {
        return new WindowsWindow(properties);
    }

    WindowsWindow::WindowsWindow(const WindowProperties& properties)
    {
        Init(properties);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProperties& properties)
    {
        m_Title = properties.Title;
        m_Width = properties.Width;
        m_Height = properties.Height;


        if(!GLFWInitialized)
        {
            const int success = glfwInit();

            ASSERT_ENGINE(success, "Could not initialize GLFW!");

            glfwSetErrorCallback(GLFWErrorCallback);
            GLFWInitialized = true;
        }


        m_Window = glfwCreateWindow(static_cast<int>(properties.Width), static_cast<int>(properties.Height), m_Title.c_str(), nullptr, nullptr);
        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, this);

        SetVSync(true);

        LOG_ENGINE_INFO("Created window {0}, ({1}, {2})", properties.Title, properties.Width, properties.Height);

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

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            scancode;
            mods;

            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    windowsWindow.KeyPressedEvent.Dispatch(key, 0);
                    break;
                }
                case GLFW_RELEASE:
                {
                    windowsWindow.KeyReleasedEvent.Dispatch(key);
                    break;
                }
                case GLFW_REPEAT:
                {
                    windowsWindow.KeyPressedEvent.Dispatch(key, 1);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int a_KeyCode)
        {
            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
            windowsWindow.KeyTypedEvent.Dispatch(a_KeyCode);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            mods;

            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    windowsWindow.MouseButtonPressedEvent.Dispatch(button);
                    break;
                }
                case GLFW_RELEASE:
                {
                    windowsWindow.MouseButtonReleasedEvent.Dispatch(button);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
            windowsWindow.MouseScrolledEvent.Dispatch(static_cast<float>(xOffset), static_cast<float>(yOffset));
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowsWindow& windowsWindow = *static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
            windowsWindow.MouseMovedEvent.Dispatch(static_cast<float>(xPos), static_cast<float>(yPos));
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

    void WindowsWindow::SetVSync(bool enabled)
    {
        if(enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        vSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return vSync;
    }
}
