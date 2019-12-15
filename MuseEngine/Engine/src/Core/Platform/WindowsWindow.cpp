#include "MusePCH.h"

#include "WindowsWindow.h"
#include "Core/Utilities/Defines.h"
#include "Core/Utilities/Log.h"
#include "Core/System/Event/ApplicationEvent.h"
#include "Core/System/Event/KeyEvent.h"
#include "Core/System/Event/MouseEvent.h"

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
        data.Title = properties.Title;
        data.Width = properties.Width;
        data.Height = properties.Height;


        if(!GLFWInitialized)
        {
            const int success = glfwInit();

            ASSERT_ENGINE(success, "Could not initialize GLFW!");

            glfwSetErrorCallback(GLFWErrorCallback);
            GLFWInitialized = true;
        }


        window = glfwCreateWindow(static_cast<int>(properties.Width), static_cast<int>(properties.Height), data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, &data);
        SetVSync(true);

        LOG_ENGINE_ERROR("Created window {0}, ({1}, {2})", properties.Title, properties.Width, properties.Height);

        //Set GLFW callbacks
        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.Width = width;
            data.Height = height;

            WindowResizeEvent windowResizeEvent(width, height);
            data.EventCallback(windowResizeEvent);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            WindowCloseEvent windowCloseEvent;
            data.EventCallback(windowCloseEvent);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent keyPressedEvent(key, 0);
                    data.EventCallback(keyPressedEvent);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent keyReleasedEvent(key);
                    data.EventCallback(keyReleasedEvent);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent keyPressedEvent(key, 1);
                    data.EventCallback(keyPressedEvent);
                    break;
                }
            }
        });


        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent mouseButtonPressedEvent(button);
                    data.EventCallback(mouseButtonPressedEvent);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent mouseButtonReleasedEvent(button);
                    data.EventCallback(mouseButtonReleasedEvent);
                    break;
                }
            }
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent mouseScrolledEvent(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data.EventCallback(mouseScrolledEvent);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseMovedEvent mouseMovedEvent(static_cast<float>(xPos), static_cast<float>(yPos));
            data.EventCallback(mouseMovedEvent);
        });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
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

        data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return data.VSync;
    }
}
