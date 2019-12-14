#include "OatPCH.h"

#include "WindowsWindow.h"
#include "Core/Utilities/Defines.h"
#include "Core/Utilities/Log.h"

namespace Muse
{
    static bool GLFWInitialized = false;

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

        LOG_ENGINE_ERROR("Creating window {0}, ({1}, {2})", properties.Title, properties.Width, properties.Height);

        if(!GLFWInitialized)
        {
            const int success = glfwInit();
            //ASSERT_ENGINE(success, "Could not initialize GLFW!");

            GLFWInitialized = true;
        }

        window = glfwCreateWindow(static_cast<int>(properties.Width), static_cast<int>(properties.Height), data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, &data);
        SetVSync(true);
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
