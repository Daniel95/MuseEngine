#pragma once
#include <string>
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"

#include <glm/glm.hpp>

namespace Muse
{
    struct WindowProperties
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProperties(const std::string& a_Title = "Muse Engine",
            const unsigned int a_Width = 512,
            const unsigned int a_Height = 512)
            : Title(a_Title), Width(a_Width), Height(a_Height)
        {
        }
    };

    // Interface representing a desktop system based Window
    class Window
    {
    public:
        WindowCloseEvent WindowCloseEvent;
        WindowResizeEvent WindowResizeEvent;
        KeyPressedEvent KeyPressedEvent;
        KeyReleasedEvent KeyReleasedEvent;
        KeyTypedEvent KeyTypedEvent;
        MouseButtonPressedEvent MouseButtonPressedEvent;
        MouseButtonReleasedEvent MouseButtonReleasedEvent;
        MouseScrolledEvent MouseScrolledEvent;
        MouseMovedEvent MouseMovedEvent;

        Window() = default;
        virtual ~Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = default;
        Window& operator=(Window&&) = default;

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual void SetWidth(unsigned int a_Width) = 0;
        virtual void SetHeight(unsigned int a_Height) = 0;
        virtual glm::vec2 GetWindowPosition() const = 0;

        // Window attributes
        virtual void SetVSync(bool enabled) = 0;
        virtual bool GetVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProperties& a_Properties = WindowProperties());
    };
}