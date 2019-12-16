#pragma once
#include <string>
#include <functional>
#include "Core/System/EventSystem.h"

namespace Muse
{
    struct WindowProperties
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProperties(const std::string& title = "Muse Engine",
            const unsigned int width = 1280,
            const unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    // Interface representing a desktop system based Window
    class Window
    {
    public:
        //using EventCallbackFn = std::function<void(Event&)>;

        Window() = default;
        virtual ~Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = default;
        Window& operator=(Window&&) = default;

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        static Window* Create(const WindowProperties& properties = WindowProperties());
    };
}